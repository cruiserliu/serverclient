#include "DataCenter.h"

void DataCenter::MessageAnalyse(TCPParcel *sendarray, char **recarray, char *sep)
{
    int num = 0;
    char *token = strtok(sendarray->szData, sep);
    while (token != NULL)
    {
        recarray[num] = token;
        token = strtok(NULL, sep);
        num++;
    }
}

void DataCenter::MessageAnalyse(char *sendarray, char **recarray, char *sep)
{
    int num = 0;
    char *token = strtok(sendarray, sep);
    while (token != NULL)
    {
        recarray[num] = token;
        token = strtok(NULL, sep);
        num++;
    }
}

void DataCenter::Decipher(char *codingbuffer, char **decipherbuffer, AESCoderDecipher *aesfuction)
{
    if (codingbuffer != NULL)
    {
        aesfuction->InvCipher(codingbuffer, *decipherbuffer);
    }
}

void DataCenter::Decipher(char *codingbuffer, TCPParcel *decipherbuffer, AESCoderDecipher *aesfuction)
{
    if (codingbuffer != NULL)
    {
        TCPParcel *tmp = (TCPParcel *)codingbuffer;
        cout << "original buffer:" << tmp->szData << endl;
        aesfuction->InvCipher(tmp->szData, decipherbuffer->szData);
        decipherbuffer->dataHeader = tmp->dataHeader;
        cout << "decipher buffer:" << decipherbuffer->szData << endl;
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

void DataCenter::Coder(char *decipherbuffer, char **codingbuffer, AESCoderDecipher *aesfuction)
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
        cout << "decipher buffer:" << decipherbuffer->szData << endl;
        aesfuction->Cipher(decipherbuffer->szData, codingbuffer->szData);
        cout << "codingbuffer :" << codingbuffer->szData << endl;
        codingbuffer->dataHeader.nDataSize = strlen(codingbuffer->szData);
    }
}