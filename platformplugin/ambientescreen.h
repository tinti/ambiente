#ifndef _AMBIENTESCREEN_H
#define _AMBIENTESCREEN_H

#include <QPlatformScreen>

class AmbienteScreen: public QPlatformScreen
{
public:
    AmbienteScreen();

    virtual QRect geometry() const;
    virtual int depth() const;
    virtual QImage::Format format() const;
};

#endif
