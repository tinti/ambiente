// Self
#include "windowsystem.h"

// Own
#include "protocol.h"
#include "window.h"

// Qt
#include <QTcpSocket>
#include <QTcpServer>
#include <QGraphicsScene>
#include <QDebug>

WindowSystem::WindowSystem(QGraphicsScene *scene)
    : QObject()
{
    // setup root window
    Window *root = new Window(this, 0, Window::ServerWindow);
    root->setGeometry(QRectF(0, 0, 800, 600));
    scene->addItem(root);
    m_windows.insert(root->id(), root);
    m_windows.insert(0, root); // 0 is an alias for the root

    // start listening
    m_server = new QTcpServer(this);
    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    m_server->listen(QHostAddress::LocalHost, 2048);
    qDebug() << "SERVER: running";
}

void WindowSystem::handleConnection()
{
    qDebug() << "SERVER: client connected";
    if (QTcpSocket *connection = m_server->nextPendingConnection()) {
        connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleConnectionError()));
        connect(connection, SIGNAL(readyRead()), this, SLOT(handleRequest()));
        connect(connection, SIGNAL(disconnected()), this, SLOT(removeConnection()));
    }
}

void WindowSystem::handleRequest()
{
    // read request(s)
    QTcpSocket *connection = qobject_cast<QTcpSocket*>(sender());
    QDataStream stream(connection);
    while (!stream.atEnd()) {
        qDebug() << "SERVER: reading request";
        Request request;
        stream >> request;
        // ### FIXME: verify that message type is request
        switch (request.type) {
        case Request::CreateWindowRequest: {
            quint32 id = createWindow(request.id);
            m_connections.insert(id, connection);
            Response response(Response::CreatedWindowResponse, id);
            qDebug() << "SERVER: sending response";
            stream << response;
            break; }
        case Request::DestroyWindowRequest:
            destroyWindow(request.id);
            break;
        case Request::ShowWindowRequest:
            showWindow(request.id);
            break;
        case Request::HideWindowRequest:
            hideWindow(request.id);
            break;
        case Request::RaiseWindowRequest:
            raiseWindow(request.id);
            break;
        case Request::LowerWindowRequest:
            lowerWindow(request.id);
            break;
        case Request::UpdateWindowRequest:
            updateWindow(request.id, request.rect);
            break;
        case Request::SetWindowGeometryRequest:
            setWindowGeometry(request.id, request.rect);
            break;
        default:
            qWarning() << "SERVER: unknown request type" << request.type;
            break;
        };
    } // while (!stream.atEnd())
}

void WindowSystem::handleConnectionError()
{
    QTcpSocket *connection = qobject_cast<QTcpSocket*>(sender());
    qWarning() << connection->errorString();
}

void WindowSystem::removeConnection()
{
    qDebug() << "SERVER: client disconnected";
    // remove connection and resources associated with that connection
    QTcpSocket *connection = qobject_cast<QTcpSocket*>(sender());
    QList<quint32> ids = m_connections.keys(connection); // ### linear
    foreach(quint32 id, ids) {
        m_connections.remove(id);
        delete m_windows.take(id);
    }
    connection->deleteLater();
}

quint32 WindowSystem::createWindow(quint32 parentId)
{
    qDebug() << "SERVER: create window";
    Window *parent = m_windows.value(parentId, 0);
    Window *window = new Window(this, parent);
    window->setVisible(false);
    m_windows.insert(window->id(), window);
    return window->id();
}

void WindowSystem::destroyWindow(quint32 id)
{
    qDebug() << "SERVER: destroy window";
    delete m_windows.take(id);
}

void WindowSystem::showWindow(quint32 id)
{
    qDebug() << "SERVER: show window";
    if (Window *window = m_windows.value(id))
        window->show();
}

void WindowSystem::hideWindow(quint32 id)
{
    if (Window *window = m_windows.value(id))
        window->hide();
}

void WindowSystem::raiseWindow(quint32 id)
{
    qDebug() << "SERVER: hide window";
    if (Window *window = m_windows.value(id))
        window->setZValue(window->zValue() + 1);
}

void WindowSystem::lowerWindow(quint32 id)
{
    qDebug() << "SERVER: lower window";
    if (Window *window = m_windows.value(id))
        window->setZValue(window->zValue() - 1);
}

void WindowSystem::updateWindow(quint32 id, const QRectF &rect)
{
    qDebug() << "SERVER: update window";
    if (Window *window = m_windows.value(id))
        window->update(rect);
}

void WindowSystem::setWindowGeometry(quint32 id, const QRectF &rect)
{
    qDebug() << "SERVER: setting geometry" << rect << id;
    if (Window *window = m_windows.value(id))
        window->setGeometry(rect);
}

void WindowSystem::sendGeometryChangeEvent(quint32 id, const QRectF &rect)
{
    qDebug() << "SERVER: sending geometry change event";
    Event event;
    event.type = Event::GeometryChangeEvent;
    event.id = id;
    event.rect = rect;
    if (QTcpSocket *connection = m_connections.value(id)) {
        QDataStream stream(connection);
        stream << event;
    }
}

void WindowSystem::sendMousePressEvent(quint32 id, const QPointF &point)
{
    qDebug() << "SERVER: sending mouse press event";
    Event event;
    event.type = Event::MousePressEvent;
    event.id = id;
    event.rect.setTopLeft(point);
    QTcpSocket *connection = m_connections.value(id);
    QDataStream stream(connection);
    stream << event;
}

void WindowSystem::sendMouseReleaseEvent(quint32 id, const QPointF &point)
{
    qDebug() << "SERVER: sending mouse release event";
    Event event;
    event.type = Event::MouseReleaseEvent;
    event.id = id;
    event.rect.setTopLeft(point);
    if (QTcpSocket *connection = m_connections.value(id)) {
        QDataStream stream(connection);
        stream << event;
    }
}

void WindowSystem::sendMouseMoveEvent(quint32 id, const QPointF &point)
{
    qDebug() << "SERVER: sending mouse move event";
    Event event;
    event.type = Event::MouseMoveEvent;
    event.id = id;
    event.rect.setTopLeft(point);
    if (QTcpSocket *connection = m_connections.value(id)) {
        QDataStream stream(connection);
        stream << event;
    }
}

void WindowSystem::sendKeyPressEvent(quint32 id, quint32 key, ushort unicode, uint modifiers)
{
    Q_UNUSED(key);
    qDebug() << "SERVER: sending key press event";
    Event event;
    event.type = Event::KeyPressEvent;
    event.id = id;
    event.value = key;

    // fishing for hack-of-the-year nominations... but at least this way we retain protocol compatibility
    event.rect = QRect(unicode, modifiers, 1, 1);

    QTcpSocket *connection = m_connections.value(id);
    QDataStream stream(connection);
    stream << event;
}

void WindowSystem::sendKeyReleaseEvent(quint32 id, quint32 key, ushort unicode, uint modifiers)
{
    Q_UNUSED(key);
    qDebug() << "SERVER: sending key release event";
    Event event;
    event.type = Event::KeyReleaseEvent;
    event.id = id;
    event.value = key;
    event.rect = QRect(unicode, modifiers, 1, 1); // same hack as above
    if (QTcpSocket *connection = m_connections.value(id)) {
        QDataStream stream(connection);
        stream << event;
    }
}
