#ifndef _AMBIENTEWINDOW_H
#define _AMBIENTEWINDOW_H

#include <QPlatformWindow>

class QWidget;

class AmbienteWindowSurface;

class AmbienteWindow: public QPlatformWindow
{
public:
    AmbienteWindow(QWidget *window, quint32 id);

    virtual void setVisible(bool visible);
    virtual void setGeometry(const QRect &rect);

    virtual void raise();
    virtual void lower();

    virtual WId winId() const { return (WId)m_id; }
    inline quint32 id() const { return m_id; }

    inline AmbienteWindowSurface *windowSurface() { return m_surface; }
    inline void setWindowSurface(AmbienteWindowSurface *surface) { m_surface = surface; }

private:
    quint32 m_id;
    AmbienteWindowSurface *m_surface;
};

#endif
