#ifndef _WINDOW_H
#define _WINDOW_H

#include <QGraphicsItem>
#include <QSharedMemory>

class WindowSystem;

class Window: public QGraphicsItem
{
public:
    enum WindowType {
        ServerWindow,
        ClientWindow
    };

    Window(WindowSystem *server, Window *parent = 0, WindowType type = ClientWindow);

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
    WindowSystem *m_server;
};

#endif
