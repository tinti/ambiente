#ifndef _AMBIENTEWINDOWSURFACE_H
#define _AMBIENTEWINDOWSURFACE_H

// Qt
#include <QPlatformWindow>
#include <QSharedMemory>
#include <QtGui/private/qwindowsurface_p.h>

class AmbienteWindow;

class AmbienteWindowSurface: public QWindowSurface
{
public:
    AmbienteWindowSurface(QWidget *window);
    ~AmbienteWindowSurface();

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    void resize(const QSize &size);

    void beginPaint(const QRegion &region);
    void endPaint(const QRegion &region);

private:
    AmbienteWindow *m_platformWindow;
    QSharedMemory m_shared;
    QImage m_image;
};

#endif
