#ifndef SERVER_H
#define SERVER_H

#include <qset.h>
#include <qdatastream.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>

class QTcpServer;
class QTcpSocket;
class Window;

class Server : public QObject
{
    Q_OBJECT
public:
    Server(QObject *parent = 0);
    ~Server();

    // events
    void sendGeometryChangeEvent(quint32 id, const QRectF &rect);
    void sendMousePressEvent(quint32 id, const QPointF &point);
    void sendMouseReleaseEvent(quint32 id, const QPointF &point);
    void sendMouseMoveEvent(quint32 id, const QPointF &point);
    void sendKeyPressEvent(quint32 id, quint32 key, ushort unicode, uint modifiers);
    void sendKeyReleaseEvent(quint32 id, quint32 key, ushort unicode, uint modifiers);

protected Q_SLOTS:
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
    // connections
    QTcpServer *m_server;
    QHash<quint32, QTcpSocket *> m_connections;
    // resources
    QHash<quint32, Window*> m_windows;
    // scenegraph
    QGraphicsScene m_scene;
    QGraphicsView m_view;
};

#endif//SERVER_H
