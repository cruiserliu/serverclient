#ifndef CONTACT_H
#define CONTACT_H

#include "contain.h"
#include "Errorlog.h"
#include "Server.h"
#include "Receiver.h"

/*****************************************************************ContactModule***********************************************************************************/
//attempt to construct one type of class to broadcast the message
//when one connfd arrive

//held by thread create function
class MonitorThreadAccept;
class ClientListener
{
public:
    virtual void UpdateOperation(int) = 0;
};

class ThreadAcceptNotifier
{
public:
    virtual void AppendClientListenner(ClientListener *) = 0;
    virtual void RemoveClientListenner(ClientListener *) = 0;
    virtual void NotifyParticipant(int) = 0;
    virtual void GetListenFd(int) = 0;
    virtual void SocketAccept() = 0;
};

class MonitorThreadAccept : public ThreadAcceptNotifier
{
public:
    MonitorThreadAccept(){};
    ~MonitorThreadAccept(){};
    void GetListenFd(int listenfd)
    {
        m_nListenFd = listenfd;
    }
    void AppendClientListenner(ClientListener *);
    void RemoveClientListenner(ClientListener *);
    void NotifyParticipant(int);
    void SocketAccept();
    int GetConnFdRead()
    {
        return m_nConnFdThread;
    };

private:
    list<ClientListener *> listenners;
    int m_nListenFd;
    int m_nConnFdThread;
};

#endif
