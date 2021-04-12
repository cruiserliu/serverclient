#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "contain.h"
#include "Process.h"

class Connecter;
class Mediator;
class MonitorAccept;
class ServerReceiverMediator;
class MediatorDecorater;

class Connecter
{
public:
    virtual void Send(int, ServerType, Connecter *) = 0;
    virtual void SetMediator(Mediator *) = 0;
    virtual void GetMessage(int, ServerType) = 0;
    Mediator *m_MedConn;
};

class Monitor
{
public:
    virtual void ServerSocketAccept() = 0;
};

//held by Select create process
class MonitorAccept : public Monitor
{
public:
    MonitorAccept(int tcplistenfd, int udpfd, int udpmultifd, Connecter *conn) : m_nTCPListenFd(tcplistenfd), m_nUDPFd(udpfd), m_nUDPMultiCastFd(udpmultifd), m_ConnSelect(conn)
    {
        /*          FD_ZERO(&m_readSet);
            FD_SET(m_nTCPListenFd, &m_readSet);
            FD_SET(m_nUDPFd, &m_readSet);
            m_nMaxfdp = max(m_nTCPListenFd, m_nUDPFd) + 1; */
        m_EpollFd = epoll_create(50);
        this->AddFd(m_EpollFd, m_nTCPListenFd); //register incident in port of TCP
        this->AddFd(m_EpollFd, m_nUDPFd);       //register incident in port of UDP
        this->AddFd(m_EpollFd, m_nUDPMultiCastFd);
        cout << "epoll fd:" << m_EpollFd << endl;
        LOG(INFO) << " IO multiplexing prepare ready !"
                  << " (TCPListenfd) = " << m_nTCPListenFd << " (UDPLIsdtenFd) = " << m_nUDPFd << "  (UDPListenfd) = " << m_nUDPMultiCastFd << endl;
    };
    ~MonitorAccept();

    void ServerSocketAccept();

private:
    void AddFd(int, int);
    int SetNonBlocking(int); //furniture the unblock descriptor
    int m_nTCPListenFd;
    int m_nUDPFd;
    int m_nUDPMultiCastFd;
    int m_EpollFd;
    /*        int m_nMaxfdp,m_nReady;
        fd_set m_readSet;*/

    epoll_event events[MAX_EVENT_NUMBER];
    Connecter *m_ConnSelect;
};

class Mediator
{
public:
    Connecter *m_RecverMed;
    Connecter *m_ServerMed;
    Connecter *m_SendMed;
    int m_StoreFd;
    ServerType m_ServerType;

public:
    virtual void SendMessage(int, ServerType, Connecter *) = 0;
    virtual void SetServerConnect(Connecter *) = 0;
    virtual void SetReceiverConnect(Connecter *) = 0;
    virtual void SetSenderConnect(char *, Connecter *) = 0;
};

class ServerReceiverMediator : public Mediator
{
public:
    ServerReceiverMediator(){};
    ~ServerReceiverMediator();
    void SetServerConnect(Connecter *serverconn)
    {
        m_ServerMed = serverconn;
    };
    void SendMessage(int listenfd, ServerType sertype, Connecter *connect)
    {
        LOG(INFO) << " Mediator start to work! " << endl;
        m_RecverMed->GetMessage(listenfd, sertype);
    };
    void SetReceiverConnect(Connecter *receiverconn)
    {
        m_RecverMed = receiverconn;
    };
    void SetSenderConnect(char *, Connecter *senderConn){};
};

class ServerConnector : public Connecter
{
public:
    ServerConnector(Server *servertcp, Server *serverudp, Server *serverudpmul)
    {
        m_serverTcpConn = servertcp;
        m_serverUdpConn = serverudp;
        m_serverUdpmulConn = serverudpmul;
    };
    ~ServerConnector();
    void SetMediator(Mediator *medator)
    {
        m_MedConn = medator;
    };
    void StartOperation();
    void Send(int listenfd, ServerType sertype, Connecter *connect)
    {
        LOG(INFO) << " ServerConnecter start to deliever message to mediator :(listenfd) " << listenfd << " (ServerType) " << sertype << " Connecter address: " << connect << endl;
        m_MedConn->SendMessage(listenfd, sertype, this);
    };
    void GetMessage(int, ServerType){};

private:
    Server *m_serverTcpConn;
    Server *m_serverUdpConn;
    Server *m_serverUdpmulConn;
};

class TCPMultiThreadReceiverAdapter
{
public:
    TCPMultiThreadReceiverAdapter(ThreadAcceptNotifier *mthread, Server *server, AESCoderDecipher *aescoding, DataCenter *data)
    {
        m_Monitorthread = mthread;
        m_Tcpser = server;
        m_Aes = aescoding;
        m_Data = data;
    };
    ~TCPMultiThreadReceiverAdapter();
    void GetListenfdAda(int listenfdAda)
    {
        m_Monitorthread->GetListenFd(listenfdAda);
        m_Listenfd = listenfdAda;
    };
    void SocketAccept()
    {
        m_Rec = new TCPReceiver(m_Listenfd, m_Aes, m_Data);
        m_threadpro = new ThreadProcess(TCP, m_Rec);
        m_Monitorthread->AppendClientListenner(m_threadpro);
        m_Monitorthread->SocketAccept();
    }

private:
    AESCoderDecipher *m_Aes;
    ThreadProcess *m_threadpro;
    ThreadAcceptNotifier *m_Monitorthread;
    Server *m_Tcpser;
    Receiver *m_Rec;
    int m_Listenfd;
    DataCenter *m_Data;
};

class ReceiverConnector : public Connecter
{
public:
    ReceiverConnector(TCPMultiThreadReceiverAdapter *servertcp, Receiver *serverudp, Receiver *serverudpmul, AESCoderDecipher *aescode)
    {
        m_paesConn = aescode;
        m_TCPReceiver = servertcp;
        m_UDPReceiver = serverudp;
        m_thredproConn = NULL;
        m_monThreadAccept = NULL;
        m_UDPMulReceiver = serverudpmul;
    };
    ~ReceiverConnector();
    void SetMediator(Mediator *medator)
    {
        m_MedConn = medator;
    };
    void Send(int, ServerType, Connecter *){};
    void GetMessage(int, ServerType);

private:
    AESCoderDecipher *m_paesConn;
    int m_nPort;
    TCPMultiThreadReceiverAdapter *m_TCPReceiver;
    Receiver *m_UDPReceiver;
    Receiver *m_UDPMulReceiver;
    MonitorThreadAccept *m_monThreadAccept;
    ThreadProcess *m_thredproConn;
};

#endif