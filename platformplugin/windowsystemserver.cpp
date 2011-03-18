// Self
#include "windowsystemserver.h"

// Own
#include "ambienteintegration.h"
#include "ambientewindowsurface.h"

// Qt
#include <QHostAddress>
#include <QApplication>
#include <QDataStream>
#include <QWindowSystemInterface>

WindowSystemServer::WindowSystemServer(AmbienteIntegration *integrator)
    : QObject()
    , m_integrator(integrator)
{
}

bool WindowSystemServer::tryConnect()
{
    m_socket.connectToHost(QHostAddress::LocalHost, 2048);
    if (m_socket.waitForConnected())
    {
        connect(&m_socket, SIGNAL(readyRead()), this, SLOT(eventDispatcher()));
        connect(&m_socket, SIGNAL(disconnected()), qApp, SLOT(quit()));
        return true;
    }

    return false;
}

bool WindowSystemServer::sendRequest(const Request &request)
{
    QDataStream s(&m_socket);
    s << (*static_cast<const Message *>(&request));
    return m_socket.isValid();
}

bool WindowSystemServer::waitForResponse(Response &response)
{
    if (m_socket.waitForReadyRead(-1))
    {
        if (m_message.message == Message::ResponseMessage)
        {
            response.type = m_message.type;
            response.id = m_message.id;
            response.rect = m_message.rect;
            m_message.message = Message::InvalidMessage;
            return true;
        }
    }

    return false;
}

void WindowSystemServer::eventDispatcher()
{
    while (m_socket.bytesAvailable())
    {
        QDataStream in(&m_socket);
        in >> m_message;

        if (m_message.message == Event::EventMessage)
        {
            AmbienteWindowSurface *surface = m_integrator->surface(m_message.id);
            QWidget *window = surface ? surface->window() : 0;

            //qDebug() << "SYSTEM: received event" << m_message.type << window;
            switch (m_message.type) {
                case Event::GeometryChangeEvent: {
                    QWindowSystemInterface::handleGeometryChange(window, m_message.rect.toRect());
                    break;
                }

                case Event::MouseMoveEvent:
                case Event::MousePressEvent: {
                    // ### we don't support multiple buttons or any keyboard modifiers
                    QPoint pos = m_message.rect.topLeft().toPoint();
                    qDebug() << "=====> Mouse press/move event" << pos << window;
                    Qt::MouseButtons b = Qt::LeftButton;
                    QWindowSystemInterface::handleMouseEvent(window, pos, pos+window->pos(), b);
                    break;
                }

                case Event::MouseReleaseEvent: {
                    // ### we don't support multiple buttons or any keyboard modifiers
                    QPoint pos = m_message.rect.topLeft().toPoint();
                    qDebug() << "======> Mouse release event" << pos << window;
                    Qt::MouseButtons b = Qt::NoButton;
                    QWindowSystemInterface::handleMouseEvent(window, pos, pos + window->pos(), b);
                    break;
                }

                case Event::KeyPressEvent: {
                    QString s;
                    if (m_message.rect.x())
                        s = QChar(int(m_message.rect.x()));

                    Qt::KeyboardModifiers mods = static_cast<Qt::KeyboardModifiers>(m_message.rect.y());
                    QWindowSystemInterface::handleKeyEvent(window, QEvent::KeyPress, m_message.value, mods, s);
                    break;
                }

                case Event::KeyReleaseEvent: {
                    Qt::KeyboardModifiers mods = static_cast<Qt::KeyboardModifiers>(m_message.rect.y());
                    QWindowSystemInterface::handleKeyEvent(window, QEvent::KeyRelease, m_message.value, mods);
                    break;
                }
            }
        }
    }
}
