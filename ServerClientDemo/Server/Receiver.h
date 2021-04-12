#ifndef RECEIVER_H
#define RECEIVER_H
#include "contain.h"
#include "Server.h"
#include "Errorlog.h"
#include "Sender.h"

/*****************************************************************ReceiverModule***********************************************************************************/

class Receiver
{
public:
    virtual void SocketReceive(int, ServerType) = 0;
    int m_nConnfd = 0;
    SignalType m_sigType;
    DataCenter *m_pdataCenter;
    AESCoderDecipher *m_AESCoderDecipher;
    string m_strS1;
};

class TCPReceiver : public Receiver
{
private:
    char arraycut[MAXLINE];
    char m_pBuffer[SEND_DATA_SIZE];
    char m_pExcute[MAXLINE][MAXLINEREC];
    TCPParcel m_pDecipherPacket;
    TCPParcel m_pCoderPacket;
    TCPParcel tmp;
    char szLastSaveData[MAX_NETPACK_SIZE]; //preserve the data received previously
    int m_nRemainSize;                     //length of excessive data received previously

public:
    TCPReceiver(int listenfd, AESCoderDecipher *aescode, DataCenter *data)
    {
        m_nRemainSize = 0;
        m_strS1 = "";
        m_AESCoderDecipher = aescode;
        m_pdataCenter = data;
        m_nConnfd = listenfd;
    };
    ~TCPReceiver()
    {
        delete[] m_pBuffer;
    };

    void SocketReceive(int, ServerType);

    void RecvMessageFromClient(int);
    void Deal(int, char **, int *);
    bool OddBitCheck(TCPParcel *, int);
};

class UDPReceiver : public Receiver
{
private:
    char m_pBufferUDP[MAXLINE];
    char m_pDecipherBuffer[MAXLINE];
    char m_pCoderBuffer[MAXLINE];
    char m_pExcute[MAXLINE][MAXLINEREC];
    struct sockaddr *m_sockServaddr;

public:
    UDPReceiver(int listenfd, AESCoderDecipher *aescode, struct sockaddr *udpsock, DataCenter *data)
    {

        //m_pBufferUDP = new char[MAXLINE];
        //m_pDecipherBuffer = new char[MAXLINE];
        //m_pCoderBuffer = new char[MAXLINE];
        m_AESCoderDecipher = aescode;
        m_sockServaddr = udpsock;
        m_nConnfd = listenfd;
        m_pdataCenter = data;
    };
    ~UDPReceiver();

    void SocketReceive(int, ServerType);
    char *RecvMessageFromClient(int, struct sockaddr *, socklen_t);
};

//append additional function to UDPReceiver in a secret and dynamic way
//including the multicast attribute related
class UDPMultiCastReceiverProxy : public Receiver
{
public:
    UDPMultiCastReceiverProxy(Receiver *rece)
    {
        m_recMultiCast = rece;
    };
    ~UDPMultiCastReceiverProxy();
    void SocketReceive(int, ServerType);
    void LeaveMultiCastGroup();

private:
    int m_nListenfd;
    struct ip_mreq *m_Mreq;
    unsigned char m_pBuffer[MAXLINE];
    Receiver *m_recMultiCast;
};

class ReceiverFactory
{
public:
    Receiver *CreateReceiverType(ServerType, int, AESCoderDecipher *, struct sockaddr *, DataCenter *);
};
#endif