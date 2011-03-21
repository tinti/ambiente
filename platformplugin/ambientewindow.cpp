// Self
#include "ambientewindow.h"

// Own
#include "windowsystemserver.h"

AmbienteWindow::AmbienteWindow(QWidget *window, quint32 id):
    QPlatformWindow(window),
    m_id(id)
{
}

void AmbienteWindow::setVisible(bool visible)
{
    QPlatformWindow::setVisible(visible);

    if (visible)
    {
        Request request(Request::ShowWindowRequest, m_id);
        WindowSystemServer::instance()->sendRequest(request);
    }
    else
    {
        Request request(Request::HideWindowRequest, m_id);
        WindowSystemServer::instance()->sendRequest(request);
    }
}

void AmbienteWindow::raise()
{
    Request request(Request::RaiseWindowRequest, m_id);
    WindowSystemServer::instance()->sendRequest(request);
}

void AmbienteWindow::lower()
{
    Request request(Request::LowerWindowRequest, m_id);
    WindowSystemServer::instance()->sendRequest(request);
}

void AmbienteWindow::setGeometry(const QRect &rect)
{
    QPlatformWindow::setGeometry(rect);
//    Request request(Request::SetWindowGeometryRequest, 0/*id*/, 0, rect);
//    WindowSystemServer::instance()->sendRequest(request);
}
