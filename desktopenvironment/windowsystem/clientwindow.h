#ifndef _CLIENTWINDOW_H
#define _CLIENTWINDOW_H

#include <QGraphicsItem>
#include <QSharedMemory>

class WindowSystem;

class ClientWindow: public QGraphicsItem
{
public:
    ClientWindow(WindowSystem *server, ClientWindow *parent = 0);

    quint32 id() const;

    void setGeometry(const QRectF &geometry);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF m_geometry;
    QSharedMemory m_surface;
};

#endif
