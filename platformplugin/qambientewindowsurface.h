#ifndef _QAMBIENTEWINDOWSURFACE_H
#define _QAMBIENTEWINDOWSURFACE_H

// Qt
#include <QPlatformWindow>
#include <QSharedMemory>
#include <QtGui/private/qwindowsurface_p.h>

class QAmbienteIntegration;

class QAmbienteWindowSurface: public QWindowSurface
{
public:
    QAmbienteWindowSurface(QWidget *window, QAmbienteIntegration *integrator);
    ~QAmbienteWindowSurface();

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    void resize(const QSize &size);

    void beginPaint(const QRegion &region);
    void endPaint(const QRegion &region);

    inline quint32 id() const { return m_id; }

private:
    quint32 m_id;
    QAmbienteIntegration *m_integrator;
    QSharedMemory m_shared;
    QImage m_image;
};

#endif
