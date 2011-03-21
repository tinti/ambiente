// Self
#include "ambientewindowsurface.h"

// Own
#include "ambienteintegration.h"
#include "ambientewindow.h"
#include "windowsystemserver.h"
#include "protocol.h"

// Qt
#include <QDebug>
#include <QtGui/private/qapplication_p.h>

AmbienteWindowSurface::AmbienteWindowSurface(QWidget *window)
    : QWindowSurface(window)
{
    m_platformWindow = static_cast<AmbienteWindow *>(window->platformWindow());
    m_platformWindow->setWindowSurface(this);
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

    Request request(Request::UpdateWindowRequest, m_platformWindow->id());
    WindowSystemServer::instance()->sendRequest(request);
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
        m_shared.setKey(QString::number(m_platformWindow->id()));
        if (!m_shared.create(byteCount) && !m_shared.attach())
        {
        }
    }

    Request request(Request::SetWindowGeometryRequest, m_platformWindow->id(), 0, QRectF(QPoint(0, 0), size));
    WindowSystemServer::instance()->sendRequest(request);
}

void AmbienteWindowSurface::beginPaint(const QRegion &region)
{
    Q_UNUSED(region);
    if (m_shared.lock() && m_shared.data())
        m_image = QImage((uchar*)m_shared.data(), geometry().width(), geometry().height(), QImage::Format_RGB32);
}

void AmbienteWindowSurface::endPaint(const QRegion &region)
{
    Q_UNUSED(region);
    if (m_shared.unlock())
        m_image = QImage();
}
