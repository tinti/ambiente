#ifndef _AMBIENTEWINDOWSURFACE_H
#define _AMBIENTEWINDOWSURFACE_H

// Qt
#include <QPlatformWindow>
#include <QSharedMemory>
#include <QtGui/private/qwindowsurface_p.h>

class AmbienteIntegration;

class AmbienteWindowSurface: public QWindowSurface
{
public:
    AmbienteWindowSurface(QWidget *window, AmbienteIntegration *integrator);
    ~AmbienteWindowSurface();

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    void resize(const QSize &size);

    void beginPaint(const QRegion &region);
    void endPaint(const QRegion &region);

    inline quint32 id() const { return m_id; }

private:
    quint32 m_id;
    AmbienteIntegration *m_integrator;
    QSharedMemory m_shared;
    QImage m_image;
};

#endif
