#ifndef _AMBIENTEWINDOW_H
#define _AMBIENTEWINDOW_H

#include <QPlatformWindow>

class QWidget;

class AmbienteWindow: public QPlatformWindow
{
public:
    AmbienteWindow(QWidget *window);

    virtual void setVisible(bool visible);
    virtual void setGeometry(const QRect &rect);
};

#endif
