#include "window.h"
#include "server.h"
#include <qpainter.h>
#include <qevent.h>
#include <qgraphicssceneevent.h>
#include <qimage.h>
#include <qdebug.h>

Window::Window(Server *server, Window *parent, WindowType type)
    : QGraphicsItem(parent), m_type(type), m_server(server)
{
    //setFlag(QGraphicsItem::ItemIsMovable); // ### for testing
    setFlag(QGraphicsItem::ItemClipsToShape);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setFlag(QGraphicsItem::ItemIsFocusable);
}

Window::~Window()
{
}

quint32 Window::id() const
{
    return quint64(this);
}

QRectF Window::geometry() const
{
    return QRectF(pos(), m_size);
}

void Window::setGeometry(const QRectF &geometry)
{
    qDebug() << "WINDOW: set geometry" << geometry << m_surface.key();
    m_size = geometry.size();
    setPos(geometry.topLeft());
}

QRectF Window::boundingRect() const
{
    return QRectF(QPointF(), m_size);
}

void Window::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    int width = m_size.width();
    int height = m_size.height();
    if (m_type == ClientWindow) {
        // attach to the shared memory held by the client
        if (m_surface.key().isEmpty())
            m_surface.setKey(QString::number(id()));
        if (!m_surface.attach(QSharedMemory::ReadOnly))
            qDebug() << m_surface.errorString();
        if (!m_surface.lock())
            qDebug() << m_surface.errorString();
        const uchar *data = static_cast<const uchar *>(m_surface.constData());
        QImage image(data, width, height, QImage::Format_ARGB32); // ### server should hold the pixel format
        painter->drawImage(0, 0, image);
        m_surface.unlock();
        m_surface.detach();
    } else {
        painter->drawRect(0, 0, width - 1 , height - 1);
    }
}

bool Window::sceneEvent(QEvent *event)
{
    qDebug() << "SERVER: Window::sceneEvent" << event->type();
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress: {
        QPointF pos = static_cast<QGraphicsSceneMouseEvent*>(event)->pos();
        m_server->sendMousePressEvent(id(), pos); // ### button and modifiers
        return true;
        break; }
    case QEvent::GraphicsSceneMouseRelease: {
        QPointF pos = static_cast<QGraphicsSceneMouseEvent*>(event)->pos();
        m_server->sendMouseReleaseEvent(id(), pos); // ### button and modifiers
        return true;
        break; }
    case QEvent::GraphicsSceneMouseMove: {
        QPointF pos = static_cast<QGraphicsSceneMouseEvent*>(event)->pos();
        m_server->sendMouseMoveEvent(id(), pos); // ### button and modifiers
        return true;
        break; }
    case QEvent::KeyPress: {
        quint32 key = static_cast<QKeyEvent*>(event)->key();
        QString s = static_cast<QKeyEvent*>(event)->text();
        ushort unicode = s.size() ? s.utf16()[0] : 0;
        uint modifiers = static_cast<QKeyEvent*>(event)->modifiers();
        m_server->sendKeyPressEvent(id(), key, unicode, modifiers);
        return true;
        break; }
    case QEvent::KeyRelease: {
        quint32 key = static_cast<QKeyEvent*>(event)->key();
        uint modifiers = static_cast<QKeyEvent*>(event)->modifiers();

        m_server->sendKeyReleaseEvent(id(), key,0,modifiers);
        return true;
        break; }
    default:
        break;
    }
    return QGraphicsItem::sceneEvent(event);
}
