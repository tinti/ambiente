// Self
#include "ambientewindow.h"

// Own
#include "windowsystemserver.h"

AmbienteWindow::AmbienteWindow(QWidget *window):
    QPlatformWindow(window)
{
}

void AmbienteWindow::setVisible(bool visible)
{
    QPlatformWindow::setVisible(visible);

    if (visible)
    {
        Request request(Request::ShowWindowRequest, 0);
        WindowSystemServer::instance()->sendRequest(request);
    }
    else
    {
        Request request(Request::HideWindowRequest, 0);
        WindowSystemServer::instance()->sendRequest(request);
    }
}

void AmbienteWindow::setGeometry(const QRect &rect)
{
    QPlatformWindow::setGeometry(rect);
    Request request(Request::SetWindowGeometryRequest, 0/*id*/, 0, rect);
    WindowSystemServer::instance()->sendRequest(request);
}
