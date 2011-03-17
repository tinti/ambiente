#ifndef WINDOW_H
#define WINDOW_H

#include <qgraphicsitem.h>
#include <qsharedmemory.h>

class Server;

class Window : public QGraphicsItem
{
public:
    enum WindowType {
        ServerWindow,
        ClientWindow
    };

    Window(Server *server, Window *parent = 0, WindowType type = ClientWindow);
    ~Window();

    quint32 id() const;

    QRectF geometry() const;
    void setGeometry(const QRectF &rect);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool sceneEvent(QEvent *event);

private:
    WindowType m_type;
    QSizeF m_size;
    QSharedMemory m_surface;
    Server *m_server;
};

#endif//WINDOW_H
