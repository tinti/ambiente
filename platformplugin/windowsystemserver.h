#ifndef _WINDOWSYSTEMSERVER_H
#define _WINDOWSYSTEMSERVER_H

#include <QObject>
#include <QTcpSocket>

#include "protocol.h"

class QAmbienteIntegration;

class WindowSystemServer: public QObject
{
    Q_OBJECT

public:
    WindowSystemServer(QAmbienteIntegration *integrator);

    bool tryConnect();
    bool sendRequest(const Request &request);
    bool waitForResponse(Response &response);

private slots:
    void eventDispatcher();

private:
    QTcpSocket m_socket;
    Message m_message;
    QAmbienteIntegration *m_integrator;
};

#endif
