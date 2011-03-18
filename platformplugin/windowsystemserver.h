#ifndef _WINDOWSYSTEMSERVER_H
#define _WINDOWSYSTEMSERVER_H

#include <QObject>
#include <QTcpSocket>

#include "protocol.h"

class AmbienteIntegration;

class WindowSystemServer: public QObject
{
    Q_OBJECT

private:
    WindowSystemServer();

public:
    bool tryConnect();
    bool sendRequest(const Request &request);
    bool waitForResponse(Response &response);

    static WindowSystemServer *instance();

private slots:
    void eventDispatcher();

private:
    QTcpSocket m_socket;
    Message m_message;
    AmbienteIntegration *m_integrator;
};

#endif
