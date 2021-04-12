#include "Sender.h"

void DataCenter::MergeMessage(string *s1, char **mchTmp)
{
    if (mchTmp != NULL && s1 != NULL)
    {
        for (int i = 0; i < MAXLINEREC; i++)
        {
            *s1 += string(mchTmp[i]);
        }
    }
}

void DataCenter::MergeMessage(string *s1, char (*mchTmp)[MAXLINEREC])
{
    if (mchTmp != NULL && s1 != NULL)
    {
        for (int i = 0; i < MAXLINEREC; i++)
        {
            *s1 += string(mchTmp[i]);
        }
    }
}

void DataCenter::MergeMessage(string s1, char (*mchTmp)[MAXLINEREC])
{
    if (mchTmp != NULL)
    {
        for (int i = 0; i < MAXLINEREC; i++)
        {
            s1 += string(mchTmp[i]);
        }
    }
}

void DataCenter::PopenExcute(const char *cmdstring, char **bufferTmp)
{
    if (bufferTmp != NULL)
    {
        int num = 1;
        try
        {
            FILE *fp = popen(cmdstring, "r");
            if (fp != NULL)
            {
                while (fgets(bufferTmp[num], MAXLINE, fp) != NULL)
                {
                    cout << "Process command element:" << bufferTmp[num] << endl;
                    num++;
                }
            }
            else
            {

                LOG(INFO) << " popen error" << endl;
                return;
            }
            pclose(fp);
        }
        catch (...)
        {
            bufferTmp[0] = "Excute command error";
        }
    }
};

void DataCenter::PopenExcute(const char *cmdstring, char (*bufferTmp)[MAXLINEREC])
{
    int num = 1;
    if (bufferTmp != NULL)
    {
        try
        {
            FILE *fp = popen(cmdstring, "r");
            if (fp != NULL)
            {
                while (fgets(bufferTmp[num], MAXLINE, fp) != NULL)
                {
                    cout << "Process command element:" << bufferTmp[num] << endl;
                    num++;
                }
            }
            else
            {

                LOG(INFO) << " popen error" << endl;
                return;
            }

            pclose(fp);
        }
        catch (...)
        {
            bufferTmp = (char(*)[50]) "Excute command error";
        }
    }
};

void DataCenter::Decipher(char *codingbuffer, char **decipherbuffer, AESCoderDecipher *aesfuction)
{
    if (codingbuffer != NULL)
    {
        aesfuction->InvCipher(codingbuffer, *decipherbuffer);
    }
}

void DataCenter::Decipher(char *codingbuffer, char (*decipherbuffer)[MAXLINE], AESCoderDecipher *aesfuction)
{
    if (codingbuffer != NULL)
    {
        aesfuction->InvCipher(codingbuffer, *decipherbuffer);
    }
}

void DataCenter::Coder(char *decipherbuffer, char **codingbuffer, AESCoderDecipher *aesfuction)
{
    if (decipherbuffer != NULL)
    {
        aesfuction->Cipher(decipherbuffer, *codingbuffer);
    }
}

void DataCenter::Coder(char *decipherbuffer, char (*codingbuffer)[MAXLINE], AESCoderDecipher *aesfuction)
{
    if (decipherbuffer != NULL)
    {
        aesfuction->Cipher(decipherbuffer, *codingbuffer);
    }
}
void DataCenter::Coder(TCPParcel *decipherbuffer, TCPParcel *codingbuffer, AESCoderDecipher *aesfuction)
{
    codingbuffer->dataHeader = decipherbuffer->dataHeader;
    if (decipherbuffer != NULL)
    {
        aesfuction->Cipher(decipherbuffer->szData, codingbuffer->szData);
        codingbuffer->dataHeader.nDataSize = strlen(codingbuffer->szData);
    }
}

void DataCenter::Coder(char *decipherbuffer, TCPParcel *codingbuffer, AESCoderDecipher *aesfuction)
{
    if (decipherbuffer != NULL)
    {
        aesfuction->Cipher(decipherbuffer, codingbuffer->szData);
        cout << "sizeof(codingbuffer)" << strlen(codingbuffer->szData);
    }
}

void DataCenter::Decipher(TCPParcel *codingbuffer, TCPParcel *decipherbuffer, AESCoderDecipher *aesfuction)
{
    codingbuffer->dataHeader = decipherbuffer->dataHeader;
    if (codingbuffer != NULL)
    {
        aesfuction->InvCipher(codingbuffer->szData, decipherbuffer->szData);
    }
}

void DataCenter::Decipher(char *codingbuffer, TCPParcel *decipherbuffer, AESCoderDecipher *aesfuction)
{
    if (codingbuffer != NULL)
    {
        TCPParcel *tmp = (TCPParcel *)codingbuffer;
        aesfuction->InvCipher(tmp->szData, decipherbuffer->szData);
        decipherbuffer->dataHeader = tmp->dataHeader;
    }
}

void DataCenter::SendExcute(int connfd, ServerType servertype, char *buffer, int buffsize, struct sockaddr *socktmp, socklen_t addrlen)
{
    if (buffer != NULL && socktmp != NULL)
    {
        if (servertype == UDP)
        {
            LOG(INFO) << " UDP Ready to send " << endl;
            if ((nSend = sendto(connfd, buffer, buffsize, 0, socktmp, addrlen)) < 0)
            {
                cout << "UDPSend error"
                     << "(errono)" << errno << endl;
                LOG(ERROR) << " UDP send message failed: error" << endl;
                CHECK_LT(nSend, 0);
            }
            else
            {
                LOG(INFO) << " UDP send message sucessful! Sending content: " << buffer << " Sending number :" << nSend << endl;
                cout << "UDP send sucessful" << endl;
            }
        }
    }
};

void DataCenter::SendExcute(int connfd, ServerType servertype, TCPParcel packet, int buffsize, struct sockaddr *socktmp, int addrlen)
{
    if (servertype == TCP)
    {
        LOG(INFO) << " TCP Ready to send " << endl;
        if ((nSend = send(connfd, &packet, buffsize, 0)) < 0)
        {
            cout << "TCPSend error"
                 << "(errono)" << errno << endl;
            LOG(ERROR) << " TCP send message failed: error" << endl;
            CHECK_LT(nSend, 0);
        }
        else
        {
            LOG(INFO) << " TCP send message sucessful! Sending content: " << endl;
            cout << "TCP send sucessful" << endl;
            LOG(INFO) << packet.szData << endl;
            LOG(INFO) << " Sending number :" << nSend << endl;
        }
    }
};
