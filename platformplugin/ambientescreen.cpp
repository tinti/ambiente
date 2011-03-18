// Self
#include "ambientescreen.h"

AmbienteScreen::AmbienteScreen():
    QPlatformScreen()
{
}

QRect AmbienteScreen::geometry() const
{
    return QRect(0, 0, 800, 600);
}

int AmbienteScreen::depth() const
{
    return 32;
}

QImage::Format AmbienteScreen::format() const
{
    return QImage::Format_RGB32;
}
