// Self
#include "ambientewindowsurface.h"

// Own
#include "ambienteintegration.h"
#include "windowsystemserver.h"
#include "protocol.h"

// Qt
#include <QDebug>
#include <QtGui/private/qapplication_p.h>

AmbienteWindowSurface::AmbienteWindowSurface(QWidget *window, AmbienteIntegration *integrator)
    : QWindowSurface(window)
    , m_integrator(integrator)
{
    WindowSystemServer *server = integrator->server();

    Request request(Request::CreateWindowRequest, integrator->parentWindowId(window));
    server->sendRequest(request);

    Response response;
    server->waitForResponse(response);

    m_id = response.id;
}

AmbienteWindowSurface::~AmbienteWindowSurface()
{
}

QPaintDevice *AmbienteWindowSurface::paintDevice()
{
    return &m_image;
}

void AmbienteWindowSurface::flush(QWidget *widget, const QRegion &region, const QPoint &offset)
{
    Q_UNUSED(widget);
    Q_UNUSED(region);
    Q_UNUSED(offset);

    Request request(Request::UpdateWindowRequest, m_id);
    m_integrator->server()->sendRequest(request);
}

void AmbienteWindowSurface::resize(const QSize &size)
{
    QWindowSurface::resize(size);

    QImage::Format format = QApplicationPrivate::platformIntegration()->screens().first()->format();
    if (m_image.size() != size)
        m_image = QImage(size, format);

    qint32 byteCount = size.width() * size.height() * sizeof(quint32);
    if (byteCount > m_shared.size())
    {
        if (!m_shared.key().isEmpty())
            m_shared.setKey(QString());
        m_shared.setKey(QString::number(m_id));
        if (!m_shared.create(byteCount) && !m_shared.attach())
        {
        }
    }

    Request request(Request::SetWindowGeometryRequest, m_id, 0, QRectF(QPoint(0, 0), size));
    m_integrator->server()->sendRequest(request);
}

void AmbienteWindowSurface::beginPaint(const QRegion &region)
{
    Q_UNUSED(region);
    if (m_shared.lock() && m_shared.data())
        m_image = QImage((uchar*)m_shared.data(), geometry().width(), geometry().height(), m_integrator->screens().first()->format());
}

void AmbienteWindowSurface::endPaint(const QRegion &region)
{
    Q_UNUSED(region);
    if (m_shared.unlock())
        m_image = QImage();
}
