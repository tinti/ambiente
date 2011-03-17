#ifndef _QAMBIENTEINTEGRATION_H
#define _QAMBIENTEINTEGRATION_H

// Qt
#include <QPlatformIntegration>
#include <QPlatformScreen>

class QAmbienteWindowSurface;
class WindowSystemServer;

class QAmbienteScreen: public QPlatformScreen
{
public:
    QAmbienteScreen(): QPlatformScreen() { }

    QRect geometry() const { return m_geometry; }
    inline void setGeometry(const QRect &r) { m_geometry = r; }
    inline int depth() const { return m_depth; }
    inline void setDepth(int d) { m_depth = d; }
    inline QImage::Format format() const { return m_format; }
    inline void setFormat(QImage::Format f) { m_format = f; }

private:
    QRect m_geometry;
    int m_depth;
    QImage::Format m_format;
};

class QAmbienteIntegration: public QPlatformIntegration
{
public:
    QAmbienteIntegration();

    bool hasCapability(QPlatformIntegration::Capability cap) const;

    QPixmapData *createPixmapData(QPixmapData::PixelType type) const;
    QPlatformWindow *createPlatformWindow(QWidget *widget, WId winId) const;
    QWindowSurface *createWindowSurface(QWidget *widget, WId winId) const;

    inline QList<QPlatformScreen *> screens() const { return m_screens; }

    WindowSystemServer *server() const { return m_server; }

    quint32 parentWindowId(QWidget *widget) const;
    QAmbienteWindowSurface *surface(quint32 id) const;

private:
    WindowSystemServer *m_server;
    mutable QHash<quint32, QAmbienteWindowSurface *> m_surfaces;
    mutable QHash<QWidget *, quint32> m_ids;
    QList<QPlatformScreen *> m_screens;
};

#endif
