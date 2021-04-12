#ifndef SERVER_H
#define SERVER_H
#include "contain.h"
#include "Errorlog.h"

/*****************************************************************ServerModule***********************************************************************************/
class Server
{
public:
    virtual void InitSocket() = 0;
    virtual void SocketBind() = 0;
    virtual void BindEstablish() = 0;
    virtual struct sockaddr_in *GetSockAddrIn() = 0;
    virtual struct ip_mreq *GetMreqMultiCast() = 0;
    virtual void SocketListen() = 0;
    virtual int GetListenFd() = 0;
    virtual void ListenEstablish() = 0;
    bool m_bConnected = false;
};

class TCPServer : public Server
{
private:
    int m_nListenfd;
    struct sockaddr_in m_sockServaddr;
    int m_nPort;
    int m_nMaxsize;

public:
    TCPServer(int Po, int Size = PORTSIZE) : m_nPort(Po), m_nMaxsize(Size){};

    void InitSocket();
    void SocketBind();
    void BindEstablish(){};
    void SocketListen();
    int GetListenFd();
    void ListenEstablish();
    struct sockaddr_in *GetSockAddrIn(){};
    struct ip_mreq *GetMreqMultiCast()
    {
        return NULL;
    };

    ~TCPServer();
};

class UDPServer : public Server
{
public:
    UDPServer(int Po) : m_nPort(Po){};
    ~UDPServer();
    void InitSocket();
    void SocketBind();
    struct sockaddr_in *GetSockAddrIn();
    void BindEstablish();
    virtual void SocketListen(){};
    virtual int GetListenFd();
    virtual void ListenEstablish(){};
    struct ip_mreq *GetMreqMultiCast()
    {
        return NULL;
    }

private:
    int m_nListenfd;
    int m_nPort;
    struct sockaddr_in m_sockServaddr;
};

class UDPServerMltiCast : public Server
{
public:
    UDPServerMltiCast(int Po, char *mlticastIPAddr, int ttl, int loop, int Size = 50) : m_nPort(Po), m_chmulIP(mlticastIPAddr), m_nMaxsize(Size)
    {
        m_nTTL = ttl;
        m_nLoop = loop;
    };
    void MultiCastTTLPassageInit();
    void MultiCastLoopPassageInit();
    void InitMultiCastAttribute();

    void InitSocket();
    void SocketBind();
    void BindEstablish();
    int GetListenFd();
    struct sockaddr_in *GetSockAddrIn();
    struct ip_mreq *GetMreqMultiCast()
    {
        return &m_Mreq;
    };
    virtual void SocketListen(){};
    virtual void ListenEstablish(){};

private:
    int m_nListenfd;
    int m_nPort;
    int m_nTTL;
    int m_nLoop;
    struct sockaddr_in m_SockLocalMultiCast;
    struct sockaddr_in m_SockGroupMultiCast;
    struct ip_mreq m_Mreq;
    char *m_chmulIP;
    int m_nMaxsize;
};
//establish serverfactory to finish the work of selecting the Server catogories
//when constructing,as an interface for selecting.
class ServerFactory
{
public:
    Server *CreateServerReceiverFactoryType(ServerType, int, char *, int, int);
};

//establish one type of class to discriminate the connecting details between TCP mode and UDP mode
//becasuse of the difference of connection.
class Direcrtor
{
public:
    virtual void AppendServer(Server *) = 0;
    virtual void DeleteServer(Server *) = 0;
    virtual void ServerMode() = 0;
};

class ConDirector : public Direcrtor
{
public:
    ConDirector(){};
    ~ConDirector()
    {
        if (m_Server.empty())
        {
            delete &m_Server;
        }
    };
    void AppendServer(Server *server)
    {
        m_Server.push_back(server);
    };
    void DeleteServer(Server *server){};
    void ServerMode();

private:
    list<Server *> m_Server;
};

#endif
