#ifndef _AMBIENTEINTEGRATION_H
#define _AMBIENTEINTEGRATION_H

// Qt
#include <QPlatformIntegration>
#include <QPlatformScreen>

class QPlatformScreen;
class AmbienteWindowSurface;
class WindowSystemServer;

class AmbienteIntegration: public QPlatformIntegration
{
public:
    AmbienteIntegration();

    bool hasCapability(QPlatformIntegration::Capability cap) const;

    QPixmapData *createPixmapData(QPixmapData::PixelType type) const;
    QPlatformWindow *createPlatformWindow(QWidget *widget, WId winId) const;
    QWindowSurface *createWindowSurface(QWidget *widget, WId winId) const;

    inline QList<QPlatformScreen *> screens() const { return m_screens; }

    WindowSystemServer *server() const { return m_server; }

    quint32 parentWindowId(QWidget *widget) const;
    AmbienteWindowSurface *surface(quint32 id) const;

private:
    WindowSystemServer *m_server;
    mutable QHash<quint32, AmbienteWindowSurface *> m_surfaces;
    mutable QHash<QWidget *, quint32> m_ids;
    QList<QPlatformScreen *> m_screens;
};

#endif
