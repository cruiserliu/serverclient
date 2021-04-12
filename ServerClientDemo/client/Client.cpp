#include "Client.h"

void TCPClient::ErrorClient(const char *message)
{
    printf(message, strerror(errno), errno);
    exit(0);
}

void UDPClient::InitService()
{
    if ((m_nSocketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
    {
        ErrorClient("create socket ErrorClient: %s(errno: %d)\n"); // 返回值小于0，报错
    }
    else
    {
        cout << "create socket sucessful !" << endl;
    }
    memset(&sockaddr_dest, 0, sizeof(sockaddr_dest)); //将servaddr结构体的内容置0
    sockaddr_dest.sin_family = AF_INET;               //规定协议族为SOCK_STREAM
    sockaddr_dest.sin_addr.s_addr = inet_addr(m_nServerAddr);
    sockaddr_dest.sin_port = htons(m_nPort); //绑定端口
}

void UDPMultiCastClient::InitService()
{
    if ((m_nSocketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
    {
        cout << "create socket error:" << endl;
    }
    else
    {

        cout << "create socket sucessful !" << endl;
    }
    memset(&sockaddr_dest, 0, sizeof(sockaddr_dest)); //将servaddr结构体的内容置0
    sockaddr_dest.sin_family = AF_INET;               //规定协议族为SOCK_STREAM
    sockaddr_dest.sin_addr.s_addr = inet_addr(m_nServerAddr);
    sockaddr_dest.sin_port = htons(m_nPort); //绑定端口
    cout << "socket :(addresss)" << inet_ntoa(*(struct in_addr *)&sockaddr_dest.sin_addr.s_addr) << " (port) " << htons(sockaddr_dest.sin_port) << endl;

    sockaddr_local.sin_addr.s_addr = inet_addr(m_LocalIP);

    cout << "local socket :(addresss)" << inet_ntoa(*(struct in_addr *)&sockaddr_local.sin_addr.s_addr) << " (port) " << htons(sockaddr_local.sin_port) << endl;
}
void UDPClient::StartService()
{
    this->InitService();
    this->LinkServer();
    this->HandleService();
}
void UDPMultiCastClient::HandleService()
{
    socklen_t len = sizeof(struct sockaddr);
    while (1)
    {
        printf("send msg to server: \n");

        if (fgets(m_pSendline, MAXLINE, stdin) != NULL)
        {
            char *find = strchr(m_pSendline, '\n');
            if (find)
            {
                *find = '\0';
            }
        }
        else
        {
            cout << "Wrong input" << endl;
            continue;
        };

        int sendlen = strlen(m_pSendline);
        printf("sendline = %d\n", sendlen);
        m_dataCenter->Coder(m_pSendline, &m_pCoderBuffer, m_aesCodDec);
        for (int i = 0; i < strlen(m_pCoderBuffer); i++)
        {
            cout << " " << m_pCoderBuffer[i];
        }
        int sendsize = sendto(m_nSocketFd, m_pCoderBuffer, strlen(m_pCoderBuffer), 0, (struct sockaddr *)&sockaddr_dest, sizeof(struct sockaddr));
        cout << "Address :" << inet_ntoa(*(struct in_addr *)&sockaddr_dest.sin_addr.s_addr) << " (port) " << htons(sockaddr_dest.sin_port) << endl;
        printf("CoderBuffer = %d\n", strlen(m_pCoderBuffer));
        printf("sendsize = %d\n", sendsize);
        printf("m_pSendline = %s\n", m_pCoderBuffer);
        cout << "UDP MultiCast Ready to receive" << endl;
        memset(m_pRecLine, 0, sizeof(m_pRecLine));
        int recsize;
        if ((recsize = recvfrom(m_nSocketFd, m_pRecLine, MAXLINE, 0, NULL, NULL)) <= 0)
        {
            //cout<<"Thread exit "<<endl;
            //pthread_exit((void *)"thread over !");
            cout << " Stop to Receive " << endl;
            return;
        }
        else
        {
            cout << " recv sucessful" << endl;
            cout << " Connectfd is " << m_nSocketFd << endl;
            ;
            cout << " Have received number from Client is " << recsize << endl;
            //recvlen = recvlen - recsize;
            cout << " Have received content from Client is " << m_pRecLine << endl;
            m_dataCenter->Decipher(m_pRecLine, &m_pDecipherBuffer, m_aesCodDec);
            m_dataCenter->MessageAnalyse(m_pDecipherBuffer, m_pBuffer, "\n");

            for (int i = 0; i < 50; i++)
            {
                cout << m_pBuffer[i] << endl;
            }
            memset(m_pCoderBuffer, 0, sizeof(m_pCoderBuffer));
            memset(m_pRecLine, 0, sizeof(m_pRecLine));
            memset(m_pDecipherBuffer, 0, sizeof(m_pDecipherBuffer));
            memset(m_pSendline, 0, sizeof(m_pSendline));
            //memset storage
            for (int i = 0; i < 50; i++)
            {
                memset(m_pBuffer[i], 0, sizeof(m_pBuffer[i]));
            }
        }
    }
}

void UDPClient::HandleService()
{
    socklen_t len = sizeof(struct sockaddr);
    while (1)
    {
        printf("send msg to server: \n");

        if (fgets(m_pSendline, MAXLINE, stdin) != NULL)
        {
            char *find = strchr(m_pSendline, '\n');
            if (find)
            {
                *find = '\0';
            }
        }
        else
        {
            cout << "Wrong input" << endl;
            continue;
        }

        int sendlen = strlen(m_pSendline);
        printf("sendline = %d\n", sendlen);
        m_dataCenter->Coder(m_pSendline, &m_pCoderBuffer, m_aesCodDec);
        for (int i = 0; i < strlen(m_pCoderBuffer); i++)
        {
            cout << " " << m_pCoderBuffer[i];
        }
        cout << endl;
        int sendsize = sendto(m_nSocketFd, m_pCoderBuffer, strlen(m_pCoderBuffer), 0, (struct sockaddr *)&sockaddr_dest, sizeof(struct sockaddr));
        printf("CoderBuffer = %d\n", strlen(m_pCoderBuffer));
        printf("sendsize = %d\n", sendsize);
        printf("m_pSendline = %s\n", m_pCoderBuffer);
        cout << "UDP Ready to receive" << endl;
        memset(m_pRecLine, 0, sizeof(m_pRecLine));
        int recsize;
        if ((recsize = recvfrom(m_nSocketFd, m_pRecLine, MAXLINE, 0, NULL, NULL)) <= 0)
        {
            //cout<<"Thread exit "<<endl;
            //pthread_exit((void *)"thread over !");
            cout << " Stop to Receive " << endl;
            return;
        }
        else
        {
            cout << " recv sucessful" << endl;
            cout << " Connectfd is " << m_nSocketFd << endl;
            ;
            cout << " Have received number from Client is " << recsize << endl;
            //recvlen = recvlen - recsize;
            cout << " Have received content from Client is " << m_pRecLine << endl;
            m_dataCenter->Decipher(m_pRecLine, &m_pDecipherBuffer, m_aesCodDec);
            m_dataCenter->MessageAnalyse(m_pDecipherBuffer, m_pBuffer, "\n");

            for (int i = 0; i < 50; i++)
            {
                cout << m_pBuffer[i] << endl;
            }
            memset(m_pCoderBuffer, 0, sizeof(m_pCoderBuffer));
            memset(m_pRecLine, 0, sizeof(m_pRecLine));
            memset(m_pDecipherBuffer, 0, sizeof(m_pDecipherBuffer));
            memset(m_pSendline, 0, sizeof(m_pSendline));
            //memset storage
            for (int i = 0; i < 50; i++)
            {
                memset(m_pBuffer[i], 0, sizeof(m_pBuffer[i]));
            }
        }
    }
}

void TCPClient::InitService()
{
    if ((m_nSocketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
    {
        ErrorClient("create socket ErrorClient: %s(errno: %d)\n"); // 返回值小于0，报错
    }
    else
    {
    }
    memset(&m_sockAddr, 0, sizeof(m_sockAddr)); //将servaddr结构体的内容置0
    m_sockAddr.sin_family = AF_INET;            //规定协议族为SOCK_STREAM
    m_sockAddr.sin_port = htons(m_nPort);       //绑定端口
}

void TCPClient::LinkServer()
{
    printf("m_nServerAddr:%s\n", m_nServerAddr);
    if (inet_pton(AF_INET, m_nServerAddr, &m_sockAddr.sin_addr) <= 0) //ip转化函数，将ip转化后赋值给servaddr.sin_addr
    {
        printf("inet_pton ErrorClient for %s\n", m_nServerAddr);
        exit(0);
    }
    else
    {
    }
    printf("m_nPort:%d\n", m_nPort);
    if ((connect(m_nSocketFd, (struct sockaddr *)&m_sockAddr, sizeof(m_sockAddr))) < 0) //用connet函数进行连接
    {
        ErrorClient("connect ErrorClient: %s(errno: %d)\n");
    }
    else
    {
        cout << "connect sucessful" << endl;
    }
}

bool TCPClient::OddBitCheck(TCPParcel *netpacket, int datasize)
{
    int count = 0;
    char *tmp = (char *)netpacket;
    for (int i = 0; i < datasize; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            if ((tmp[i] & 1 << j) != 0)
            {
                count++;
            }
        }
    }

    if (count % 2 == 0)
    {
        return false;
    }

    return true;
}

void TCPClient::RecvMessageFromClient(int connfd)
{

    //every time recving length during the proccess
    int recvlen = SEND_DATA_SIZE;

    while (true)
    {
        int recsize;
        if ((recsize = recv(connfd, m_pRecLine, recvlen, 0)) == 0)
        {
            cout << "Thread exit " << endl;
            pthread_exit((void *)"thread over !");
            cout << " Stop to Receive " << endl;
            break;
        }
        else if (recsize == -1)
        {
            recvlen = SEND_DATA_SIZE;
            continue;
        }
        else
        {
            cout << "Receive line :" << endl;
            for (int i = 0; i < recsize; i++)
            {
                cout << " " << (int)(*(((char *)m_pRecLine) + i));
            }
            cout << "recsize = " << recsize << endl;
            memcpy((char *)szLastSaveData + m_nRemainSize, m_pRecLine, recsize);
            m_nRemainSize = m_nRemainSize + recsize;

            while (m_nRemainSize >= sizeof(TCPParcelHeader))
            {
                int temp = 0xffffffff;
                if (memcmp(szLastSaveData, &temp, sizeof(int)) != 0)
                {
                    memcpy(szLastSaveData, szLastSaveData + sizeof(char), strlen(szLastSaveData) - sizeof(char));
                    m_nRemainSize--;
                    continue;
                }

                TCPParcelHeader *phead = (TCPParcelHeader *)szLastSaveData;
                int datanumber = phead->nDataSize + sizeof(char) + sizeof(TCPParcelHeader);
                if (m_nRemainSize < datanumber)
                {
                    break;
                }

                if (!OddBitCheck((TCPParcel *)phead, datanumber))
                {
                    Deal(szLastSaveData, datanumber);
                    memcpy(szLastSaveData, szLastSaveData + datanumber, sizeof(szLastSaveData) - datanumber);
                    m_nRemainSize = m_nRemainSize - datanumber;
                    if (sizeof(szLastSaveData) - m_nRemainSize > 0)
                    {
                        memset(szLastSaveData + m_nRemainSize, 0, sizeof(szLastSaveData) - m_nRemainSize);
                    }
                    return;
                }
                else
                {
                    memcpy(szLastSaveData, szLastSaveData + sizeof(char), strlen(szLastSaveData) - sizeof(char));
                    m_nRemainSize = m_nRemainSize - sizeof(char);
                    continue;
                }
            }
        }
    }
}

void TCPClient::Deal(char *tcptmp, int processdatalen)
{
    if (tcptmp != NULL)
    {

        for (int i = 0; i < processdatalen - 1; i++)
        {
            arraycut[i] = tcptmp[i];
        }
        arraycut[processdatalen - 1] = '\0';
        cout << "After array cut:" << endl;
        for (int i = 0; i < processdatalen - 1; i++)
        {
            cout << " " << (int)(*(((char *)arraycut) + i));
        }

        cout << "Recline :" << m_pRecLine << endl;
        m_dataCenter->Decipher(arraycut, m_pDecipherPacket, m_aesCodDec);
        m_dataCenter->MessageAnalyse(m_pDecipherPacket, m_pBuffer, "\n");
        cout << " Connectfd is " << m_nSocketFd << endl;
        cout << " Have received content from Client is " << endl;

        //display the result from Server
        for (int i = 0; i < 50; i++)
        {
            cout << m_pBuffer[i] << endl;
        }
        memset((char *)m_pCoderPacket, 0, sizeof(m_pCoderPacket));
        memset(m_pRecLine, 0, sizeof(m_pRecLine));
        memset((char *)m_pDecipherPacket, 0, sizeof(m_pDecipherPacket));
        for (int i = 0; i < 50; i++)
        {
            memset(m_pBuffer[i], 0, sizeof(m_pBuffer[i]));
        }
        cout << "After memset Recline:" << m_pRecLine << endl;
    }
}
void TCPClient::HandleService()
{
    TCPParcel netpacket;
    while (true)
    {
        printf("send msg to server: \n");
        fflush(stdin);
        memset(netpacket.szData, 0, sizeof(netpacket.szData));

        if (fgets(netpacket.szData, MAXLINE, stdin) != NULL)
        {
            char *find = strchr(netpacket.szData, '\n');
            if (find)
            {
                *find = '\0';
            }
            netpacket.dataHeader.nDataType = 0xffffffff;
            cout << "input content :" << netpacket.szData << " length = " << strlen(netpacket.szData) << endl;
        }
        else
        {
            cout << "Wrong input" << endl;
            continue;
        }
        m_dataCenter->Coder(&netpacket, m_pCoderPacket, m_aesCodDec);
        int length = strlen(m_pCoderPacket->szData);
        bool judBool = OddBitCheck(m_pCoderPacket, m_pCoderPacket->dataHeader.nDataSize + 8);
        if (judBool)
        {
            cout << "odd" << endl;
            m_pCoderPacket->szData[length] = 1;
        }
        else
        {
            cout << "even" << endl;
            m_pCoderPacket->szData[length] = 0;
        }

        int sendlen = length + 9;
        printf("sendline = %d\n", sendlen);
        cout << "Sending header = " << m_pCoderPacket->dataHeader.nDataType;
        int sendsize = send(m_nSocketFd, m_pCoderPacket, sendlen, 0);
        int sendsize1 = send(m_nSocketFd, m_pCoderPacket, sendlen - 5, 0);
        for (int i = 0; i < sendsize; i++)
        {
            cout << " " << (int)(*(((char *)m_pCoderPacket) + i));
        }
        cout << endl;
        if (sendsize < 0)
        {
            continue;
        }
        printf("sendsize = %d\n", sendsize);
        sendlen = sendlen - sendsize;
        printf("m_pSendline = %d, %d, %s, sizeof(m_pSendline) = %d\n", m_pCoderPacket->dataHeader.nDataType, m_pCoderPacket->dataHeader.nDataSize, m_pCoderPacket->szData, sizeof(m_pCoderPacket->dataHeader) + strlen(m_pCoderPacket->szData));
        //memset(m_pSendline, 0, sizeof(m_pSendline));
        int recvlen = MAXLINE;
        cout << " Receive setting from Client is " << recvlen << endl;
        RecvMessageFromClient(m_nSocketFd);
    }
}

void TCPClient::StartService()
{
    this->InitService();
    this->LinkServer();
    this->HandleService();
}

void TCPClient::StopService()
{
    close(m_nSocketFd);
}
