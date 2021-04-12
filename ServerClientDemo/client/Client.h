#ifndef CLIENT_H
#define CLIENT_H
#include "contain.h"
#include "CoderDecipher.h"
#include "DataCenter.h"

class Client
{
public:
    virtual void InitService() = 0;
    virtual int GetListenfd() = 0;
    virtual struct sockaddr_in *GetSockAddrStructure() = 0;
    virtual void LinkServer() = 0;
    virtual void HandleService() = 0;
    virtual void StartService() = 0;
    virtual void StopService() = 0;
    AESCoderDecipher *m_aesCodDec;
    DataCenter *m_dataCenter;
    char *m_pBuffer[50];
};

class TCPClient : public Client
{
private:
    int m_nSocketFd;
    char *m_pRecLine; //content of sending
    char *m_nServerAddr;
    char arraycut[MAXLINE];
    int m_nPort;
    struct sockaddr_in m_sockAddr;
    TCPParcel *m_pCoderPacket;
    TCPParcel *m_pDecipherPacket;
    void ErrorClient(const char *);
    char szLastSaveData[MAX_NETPACK_SIZE]; //preserve the data received previously
    int m_nRemainSize;                     //length of excessive data received previously
public:
    TCPClient(char *m_nServerAddr, int Po, AESCoderDecipher *aescodrec) : m_nServerAddr(m_nServerAddr), m_nPort(Po)
    {
        m_nRemainSize = 0;
        m_dataCenter = new DataCenter();
        m_aesCodDec = aescodrec;
        m_pRecLine = new char[4108];

        for (int i = 0; i < 50; i++)
        {
            m_pBuffer[i] = (char *)malloc(4096);
        }
        m_pDecipherPacket = new TCPParcel;
        m_pCoderPacket = new TCPParcel;
    };

    void InitService();
    int GetListenfd()
    {
        return m_nSocketFd;
    };
    struct sockaddr_in *GetSockAddrStructure()
    {
        return &m_sockAddr;
    };
    void LinkServer();
    void HandleService();
    void StartService();
    void StopService();
    void RecvMessageFromClient(int);
    void Deal(char *, int);
    bool OddBitCheck(TCPParcel *, int);
};

class UDPClient : public Client
{
private:
    int m_nSocketFd;
    char m_pRecLine[MAXLINE];
    char *m_pSendline; //content of sending
    char *m_nServerAddr;
    int m_nPort;
    char *m_pCoderBuffer;
    char *m_pDecipherBuffer;
    struct sockaddr_in sockaddr_dest;

    void ErrorClient(const char *){};

public:
    UDPClient(char *m_nServerAddr, int Po, AESCoderDecipher *aescodrec) : m_nServerAddr(m_nServerAddr), m_nPort(Po)
    {
        m_aesCodDec = aescodrec;
        m_pSendline = new char[MAXLINE];
        m_pCoderBuffer = new char[MAXLINE];
        m_pDecipherBuffer = new char[MAXLINE];
        for (int i = 0; i < 50; i++)
        {
            m_pBuffer[i] = new char[MAXLINE];
        }
        //m_pDecipherBuffer = new char [MAXLINE];
    };

    ~UDPClient()
    {
        if (m_pSendline != NULL)
        {
            delete[] m_pSendline;
        }
        if (m_pCoderBuffer != NULL)
        {
            delete[] m_pSendline;
        }
        if (m_pDecipherBuffer != NULL)
        {
            delete[] m_pSendline;
        }
        for (int i = 0; i < 50; i++)
        {
            if (m_pBuffer[i] != NULL)
            {
                delete[] m_pBuffer[i];
            }
        };
    }
    void InitService();
    int GetListenfd()
    {
        return m_nSocketFd;
    };
    void LinkServer(){};
    void HandleService();
    void StartService();
    void StopService(){};
    struct sockaddr_in *GetSockAddrStructure()
    {
        return &sockaddr_dest;
    };
};

class UDPMultiCastClient : public Client
{
public:
    UDPMultiCastClient(char *m_chServerAddress, int port, AESCoderDecipher *aescodrec, char *localip)
    {
        m_pSendline = new char[MAXLINE];
        m_pCoderBuffer = new char[MAXLINE];
        m_pDecipherBuffer = new char[MAXLINE];
        for (int i = 0; i < 50; i++)
        {
            m_pBuffer[i] = new char[MAXLINE];
        };
        m_LocalIP = localip;
        m_nPort = port;
        m_aesCodDec = aescodrec;
        m_nServerAddr = m_chServerAddress;
    };
    ~UDPMultiCastClient()
    {
        if (m_pSendline != NULL)
        {
            delete[] m_pSendline;
        }
        if (m_pCoderBuffer != NULL)
        {
            delete[] m_pSendline;
        }
        if (m_pDecipherBuffer != NULL)
        {
            delete[] m_pSendline;
        }
        for (int i = 0; i < 50; i++)
        {
            if (m_pBuffer[i] != NULL)
            {
                delete[] m_pBuffer[i];
            }
        }
    };

    void InitService();
    int GetListenfd(){};
    void LinkServer()
    {
        if (setsockopt(m_nSocketFd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&sockaddr_local, sizeof(sockaddr_local)) < 0)
        {
            perror("Setting local interface error");
            exit(1);
        }
        else
        {
            printf("Setting the local interface...OK\n");
        }
    };
    void HandleService();
    struct sockaddr_in *GetSockAddrStructure()
    {
        return &sockaddr_dest;
    };

    void StartService()
    {
        this->InitService();
        this->LinkServer();
        this->HandleService();
    };

    void StopService(){};

private:
    char *m_pSendline; //content of sending
    char *m_pCoderBuffer;
    char *m_pDecipherBuffer;
    char m_pRecLine[MAXLINE];

    char *m_nServerAddr;
    char *m_LocalIP;
    int m_nPort;
    int m_nSocketFd;
    struct sockaddr_in sockaddr_dest;
    struct sockaddr_in sockaddr_local;
};
#endif
