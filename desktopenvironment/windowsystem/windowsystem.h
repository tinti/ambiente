#ifndef _WINDOWSYSTEM_H
#define _WINDOWSYSTEM_H

// Qt
#include <QHash>
#include <QDataStream>
#include <QRectF>

class QTcpServer;
class QTcpSocket;
class QGraphicsScene;

class Window;

class WindowSystem: public QObject
{
    Q_OBJECT

public:
    WindowSystem(QGraphicsScene *scene);

    // events
    void sendGeometryChangeEvent(quint32 id, const QRectF &rect);
    void sendMousePressEvent(quint32 id, const QPointF &point);
    void sendMouseReleaseEvent(quint32 id, const QPointF &point);
    void sendMouseMoveEvent(quint32 id, const QPointF &point);
    void sendKeyPressEvent(quint32 id, quint32 key, ushort unicode, uint modifiers);
    void sendKeyReleaseEvent(quint32 id, quint32 key, ushort unicode, uint modifiers);

protected slots:
    void handleRequest();
    void handleConnection();
    void handleConnectionError();
    void removeConnection();

    // requests
    quint32 createWindow(quint32 parentId);
    void destroyWindow(quint32 id);
    void showWindow(quint32 id);
    void hideWindow(quint32 id);
    void raiseWindow(quint32 id);
    void lowerWindow(quint32 id);
    void updateWindow(quint32 id, const QRectF &rect);
    void setWindowGeometry(quint32 id, const QRectF &rect);

private:
    QTcpServer *m_server;
    QHash<quint32, QTcpSocket *> m_connections;
    QHash<quint32, Window*> m_windows;
};

#endif
