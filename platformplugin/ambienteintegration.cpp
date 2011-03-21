// Self
#include "ambienteintegration.h"
#include "ambientewindow.h"

// Own
#include "ambientewindowsurface.h"
#include "ambientescreen.h"
#include "windowsystemserver.h"

#include <QPlatformWindow>
#include <QtGui/private/qpixmap_raster_p.h>

AmbienteIntegration::AmbienteIntegration()
{
    m_server = WindowSystemServer::instance();

    if (!m_server->tryConnect())
        qFatal("Failed to connect to server");

    AmbienteScreen *screen = new AmbienteScreen();
    m_screens.append(screen);
}

bool AmbienteIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
    switch (cap) {
    case ThreadedPixmaps: return true;
    default: return QPlatformIntegration::hasCapability(cap);
    }
}

QPixmapData *AmbienteIntegration::createPixmapData(QPixmapData::PixelType type) const
{
    return new QRasterPixmapData(type);
}

QPlatformWindow *AmbienteIntegration::createPlatformWindow(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    AmbienteWindow *window = new AmbienteWindow(widget);
    return window;
}

QWindowSurface *AmbienteIntegration::createWindowSurface(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    AmbienteWindowSurface *surface = new AmbienteWindowSurface(widget, (AmbienteIntegration *)this);
    m_surfaces.insert(surface->id(), surface);
    m_ids.insert(widget, surface->id());
    return surface;
}

quint32 AmbienteIntegration::parentWindowId(QWidget *widget) const
{
    QWidget *parent = NULL;
    QWidget *test = widget;
    while (test)
    {
        Qt::WindowFlags type = test->windowFlags() & Qt::WindowType_Mask;
        if (type == Qt::Window)
        {
            parent = test;
            break;
        }
        else
            test = test->parentWidget();
    }

    return m_ids.value(parent);
}

AmbienteWindowSurface *AmbienteIntegration::surface(quint32 id) const
{
    return m_surfaces.value(id);
}
