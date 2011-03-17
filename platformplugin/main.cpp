#include <QtGui/QPlatformIntegrationPlugin>
#include "qambienteintegration.h"

QT_BEGIN_NAMESPACE

class QAmbienteIntegrationPlugin : public QPlatformIntegrationPlugin
{
public:
    QStringList keys() const;
    QPlatformIntegration *create(const QString&, const QStringList&);
};

QStringList QAmbienteIntegrationPlugin::keys() const
{
    QStringList list;
    list << "Ambiente";
    return list;
}

QPlatformIntegration *QAmbienteIntegrationPlugin::create(const QString& system, const QStringList& paramList)
{
    Q_UNUSED(paramList);
    if (system.toLower() == "ambiente")
        return new QAmbienteIntegration;

    return 0;
}

Q_EXPORT_PLUGIN2(ambiente, QAmbienteIntegrationPlugin)

QT_END_NAMESPACE
