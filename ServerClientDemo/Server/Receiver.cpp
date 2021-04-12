#include "Receiver.h"

char *UDPReceiver::RecvMessageFromClient(int connfd, struct sockaddr *m_tmp, socklen_t len)
{
    if (m_tmp != NULL)
    {
        while (true)
        {
            int recsize;
            if ((recsize = recvfrom(connfd, m_pBufferUDP, MAXLINE, 0, m_tmp, &len)) == 0)
            {
                /*LOG(INFO)<<"Thread exit "<<endl;
                pthread_exit((void *)"thread over !");*/
                LOG(INFO) << " Stop to Receive " << endl;
                break;
            }
            else if (recsize == -1)
            {
                continue;
            }
            else
            {
                cout << m_pBufferUDP << endl;
                cout << " Have received number from Client is " << recsize << endl;
                return m_pBufferUDP;
            }
        }
        return NULL;
    }
}

void TCPReceiver::RecvMessageFromClient(int connfd)
{

    //every time recving length during the proccess
    int recvlen = SEND_DATA_SIZE;

    while (true)
    {
        int recsize;
        if ((recsize = recv(connfd, m_pBuffer, recvlen, 0)) == 0)
        {
            LOG(INFO) << "Thread exit " << endl;
            pthread_exit((void *)"thread over !");
            LOG(INFO) << " Stop to Receive " << endl;
            break;
        }
        else if (recsize == -1)
        {
            recvlen = SEND_DATA_SIZE;
            continue;
        }
        else
        {
            cout << "recvsiz = " << recsize << endl;
            memcpy((char *)szLastSaveData + m_nRemainSize, m_pBuffer, recsize);
            m_nRemainSize = m_nRemainSize + recsize;

            while (m_nRemainSize >= sizeof(TCPParcelHeader))
            {
                int temp = 0xffffffff;
                if (memcmp(szLastSaveData, &temp, sizeof(int)) != 0)
                {
                    for (int i = 0; i < sizeof(int); i++)
                    {
                        cout << " " << (int)(*(szLastSaveData + i));
                    }
                    cout << endl;
                    memcpy(szLastSaveData, szLastSaveData + sizeof(char), sizeof(szLastSaveData) - sizeof(char));
                    m_nRemainSize--;
                    continue;
                }

                TCPParcelHeader *phead = (TCPParcelHeader *)szLastSaveData;
                char *tmpchar = szLastSaveData;
                int datanumber = phead->nDataSize + sizeof(char) + sizeof(TCPParcelHeader);
                if (m_nRemainSize < datanumber)
                {
                    cout << "datasize = " << phead->nDataSize << endl;
                    cout << "m_nRemainSize = " << m_nRemainSize << " datanumber = " << datanumber << endl;
                    break;
                }

                if (!OddBitCheck((TCPParcel *)phead, datanumber))
                {
                    cout << "Inact Data received is correct" << endl;
                    Deal(connfd, &tmpchar, &datanumber);
                    memcpy(szLastSaveData, szLastSaveData + datanumber, sizeof(szLastSaveData) - datanumber);
                    m_nRemainSize = m_nRemainSize - datanumber;
                    if (sizeof(szLastSaveData) - m_nRemainSize > 0)
                    {
                        memset(szLastSaveData + m_nRemainSize, 0, sizeof(szLastSaveData) - m_nRemainSize);
                    }
                    cout << "data leftover is :" << endl;
                    for (int i = 0; i < m_nRemainSize; i++)
                    {
                        cout << " " << (int)szLastSaveData[i];
                    }
                    cout << endl;
                }
                else
                {
                    memcpy(szLastSaveData, szLastSaveData + sizeof(char), sizeof(szLastSaveData) - sizeof(char));
                    m_nRemainSize = m_nRemainSize - sizeof(char);
                    continue;
                }
                memset(m_pBuffer, 0, sizeof(m_pBuffer));
                for (int i = 0; i < MAXLINEREC; i++)
                {
                    memset(m_pExcute[i], 0, sizeof(m_pExcute));
                }
            }
        }
    }
}

void UDPReceiver::SocketReceive(int connfd, ServerType sertype)
{
    cout << "Fd = " << connfd << endl;
    char *test = NULL;
    socklen_t lenUD = sizeof(struct sockaddr_in);
    if (sertype == UDP)
    {
        struct sockaddr socktmp;
        char *test = RecvMessageFromClient(connfd, &socktmp, lenUD);
        if (test != NULL)
        {
            m_pdataCenter->Decipher(test, &m_pDecipherBuffer, m_AESCoderDecipher);
            cout << " DecipherBuffer = " << m_pDecipherBuffer << endl;
            LOG(INFO) << " recv sucessful" << endl;
            LOG(INFO) << " Connectfd is " << connfd << endl;
            cout << " Have received content from Client is " << m_pDecipherBuffer << endl;
            m_pdataCenter->PopenExcute(m_pDecipherBuffer, m_pExcute);
            m_pdataCenter->MergeMessage(&m_strS1, m_pExcute);
            if (m_strS1.empty())
            {
                m_strS1 = "Illegal Result from client";
            }
            m_pdataCenter->Coder((char *)m_strS1.c_str(), &m_pCoderBuffer, m_AESCoderDecipher);
            cout << m_pCoderBuffer << " size = " << strlen(m_pCoderBuffer) << endl;
            m_pdataCenter->SendExcute(connfd, UDP, m_pCoderBuffer, strlen(m_pCoderBuffer), (struct sockaddr *)&socktmp, lenUD);
        }
        else
        {
            cout << "Connection interuption" << endl;
            return;
        }
        m_strS1 = "";
        memset(m_pBufferUDP, 0, sizeof(m_pBufferUDP));
        memset(m_pDecipherBuffer, 0, sizeof(m_pDecipherBuffer));
        memset(m_pCoderBuffer, 0, sizeof(m_pCoderBuffer));
        for (int i = 0; i < MAXLINEREC; i++)
        {
            memset(m_pExcute[i], 0, sizeof(m_pExcute[i]));
        }
    }
    if (sertype == UDPMultiCast)
    {
        char *test = RecvMessageFromClient(connfd, m_sockServaddr, lenUD);
        if (test != NULL)
        {
            m_pdataCenter->Decipher(test, &m_pDecipherBuffer, m_AESCoderDecipher);
            cout << " DecipherBuffer = " << m_pDecipherBuffer << endl;
            LOG(INFO) << " recv sucessful" << endl;
            LOG(INFO) << " Connectfd is " << connfd << endl;
            cout << " Have received content from Client is " << m_pDecipherBuffer << endl;
            m_pdataCenter->PopenExcute(m_pDecipherBuffer, m_pExcute);
            m_pdataCenter->MergeMessage(&m_strS1, m_pExcute);
            if (m_strS1.empty())
            {
                m_strS1 = "Illegal Result from client";
            }
            m_pdataCenter->Coder((char *)m_strS1.c_str(), &m_pCoderBuffer, m_AESCoderDecipher);
            cout << m_pCoderBuffer << " size = " << strlen(m_pCoderBuffer) << endl;
            m_pdataCenter->SendExcute(connfd, UDP, m_pCoderBuffer, strlen(m_pCoderBuffer), m_sockServaddr, lenUD);
        }
        else
        {
            cout << "Connection interuption" << endl;
            return;
        }
        m_strS1 = "";
        memset(m_pBufferUDP, 0, sizeof(m_pBufferUDP));
        memset(m_pDecipherBuffer, 0, sizeof(m_pDecipherBuffer));
        memset(m_pCoderBuffer, 0, sizeof(m_pCoderBuffer));
        for (int i = 0; i < MAXLINEREC; i++)
        {
            memset(m_pExcute[i], 0, sizeof(m_pExcute[i]));
        }
    }
}

void TCPReceiver::Deal(int connfd, char **tcptmp, int *processdatalen)
{
    if (tcptmp != NULL && processdatalen != NULL)
    {
        for (int i = 0; i < *processdatalen; i++)
        {
            cout << " " << (int)(*(((char *)((*tcptmp) + i))));
        }
        cout << endl;
        for (int i = 0; i < *processdatalen - 1; i++)
        {
            arraycut[i] = (*tcptmp)[i];
        }
        arraycut[*processdatalen - 1] = '\0';
        for (int i = 0; i < *processdatalen - 1; i++)
        {
            cout << " " << (int)(*(((char *)arraycut) + i));
        }
        m_pdataCenter->Decipher(arraycut, &m_pDecipherPacket, m_AESCoderDecipher);
        cout << " Have received content from Client is " << endl;
        cout << m_pDecipherPacket.szData << endl;

        //excute linux command towards m_pExcute
        m_pdataCenter->PopenExcute(m_pDecipherPacket.szData, m_pExcute);
        //put the result into m_Str1
        cout << "m_pExcute is" << endl;

        for (int i = 0; i < MAXLINEREC; i++)
        {
            cout << m_pExcute[i] << endl;
        }
        m_pdataCenter->MergeMessage(&m_strS1, m_pExcute);
        //create new netpacket containing the coding buffer
        cout << "m_Str1 = " << m_strS1 << endl;
        //encrypting the m_str1 into the m_pCoderBuffer
        if (m_strS1.empty())
        {
            m_strS1 = "Illegal excuting result ,Please try again";
        }
        cout << "str1 length = " << m_strS1.length() << endl;
        char *tempArray;
        if (m_strS1.length() > MAXLINE)
        {
            tempArray = (char *)m_strS1.substr(0, 4096).c_str();
        }
        else
        {
            tempArray = (char *)m_strS1.c_str();
        }
        cout << "size of :" << strlen((char *)m_strS1.c_str()) << endl;
        m_pdataCenter->Coder(tempArray, &tmp, m_AESCoderDecipher);
        tmp.dataHeader.nDataType = 0xffffffff;
        int backlen = strlen(tmp.szData);
        cout << backlen << endl;
        tmp.dataHeader.nDataSize = backlen;
        cout << "Coder buffer before send is" << tmp.szData << " size = " << strlen(tmp.szData) << endl;
        bool oddBool = OddBitCheck(&tmp, tmp.dataHeader.nDataSize + sizeof(TCPParcelHeader));
        if (oddBool)
        {
            tmp.szData[backlen] = 1;
        }
        else
        {
            tmp.szData[backlen] = 0;
        }
        for (int i = 0; i < backlen + sizeof(TCPParcelHeader) + sizeof(char); i++)
        {
            cout << " " << (int)(*(((char *)&tmp) + i));
        }
        cout << "strlen(size) = " << strlen(tmp.szData) << endl;
        m_pdataCenter->SendExcute(connfd, TCP, tmp, backlen + sizeof(TCPParcelHeader) + sizeof(char), (struct sockaddr *)0, 0);
        cout << "Sending data length is " << strlen(tmp.szData) + 9 << endl;
        memset(arraycut, 0, sizeof(arraycut));
        memset(m_pBuffer, 0, sizeof(m_pBuffer));
        memset((char *)&m_pDecipherPacket, 0, sizeof(m_pDecipherPacket));
        memset((char *)&m_pCoderPacket, 0, sizeof(m_pCoderPacket));
        memset((char *)&tmp, 0, sizeof(tmp));
        cout << "m_Decipherr array :" << m_pDecipherPacket.szData << endl;
        cout << "m_pBuffer" << m_pBuffer << endl;
        cout << "m_Coder array :" << m_pCoderPacket.szData << endl;
        m_strS1 = "";
    }
}

void TCPReceiver::SocketReceive(int connfd, ServerType sertype)
{
    cout << " TCP Start to Receive " << endl;
    RecvMessageFromClient(connfd);
}

void UDPMultiCastReceiverProxy::SocketReceive(int conn, ServerType sertype)
{
    m_recMultiCast->SocketReceive(conn, sertype);
}

void UDPMultiCastReceiverProxy::LeaveMultiCastGroup()
{
    if (setsockopt(m_nListenfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &m_Mreq, sizeof(m_Mreq)) < 0)
    {
        LOG(INFO) << " Leaving setting error(error):" << endl;
        return;
    }
    else
    {
        LOG(INFO) << " Leaving setting sucessful" << endl;
    }
}

Receiver *ReceiverFactory::CreateReceiverType(ServerType sertyp, int listenfd, AESCoderDecipher *aescode, struct sockaddr *socktmp, DataCenter *data)
{

    switch (sertyp)
    {
    case TCP:
        LOG(INFO) << " One TCPReceiver ready to construct" << endl;
        return new TCPReceiver(listenfd, aescode, data);
        break;
    case UDP:
        LOG(INFO) << " One UDPReceiver ready to construct" << endl;
        return new UDPReceiver(listenfd, aescode, socktmp, data);
        break;
    default:
        LOG(ERROR) << " Receiver create error: Could not found the catogories:" << endl;
        ;
        return NULL;
    }
}

bool TCPReceiver::OddBitCheck(TCPParcel *netpacket, int datasize)
{
    int count = 0;
    char *tmp = (char *)netpacket;
    for (int i = 0; i < datasize + 1; i++)
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
