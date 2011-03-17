// Self
#include "qambienteintegration.h"

// Own
#include "qambientewindowsurface.h"
#include "windowsystemserver.h"

#include <QPlatformWindow>
#include <QtGui/private/qpixmap_raster_p.h>

QAmbienteIntegration::QAmbienteIntegration()
{
    m_server = new WindowSystemServer(this);

    if (!m_server->tryConnect())
        qFatal("Failed to connect to server");

    QAmbienteScreen *screen = new QAmbienteScreen();
    screen->setGeometry(QRect(0, 0, 800, 600));
    screen->setDepth(32);
    screen->setFormat(QImage::Format_ARGB32);

    m_screens.append(screen);
}

bool QAmbienteIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

QPixmapData *QAmbienteIntegration::createPixmapData(QPixmapData::PixelType type) const
{
    return new QRasterPixmapData(type);
}

QPlatformWindow *QAmbienteIntegration::createPlatformWindow(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    return new QPlatformWindow(widget);
}

QWindowSurface *QAmbienteIntegration::createWindowSurface(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    QAmbienteWindowSurface *surface = new QAmbienteWindowSurface(widget, (QAmbienteIntegration *)this);
    m_surfaces.insert(surface->id(), surface);
    m_ids.insert(widget, surface->id());
    return surface;
}

quint32 QAmbienteIntegration::parentWindowId(QWidget *widget) const
{
    return m_ids.value(widget->parentWidget());
}

QAmbienteWindowSurface *QAmbienteIntegration::surface(quint32 id) const
{
    return m_surfaces.value(id);
}
