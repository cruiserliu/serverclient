#include "contain.h"
#include "Client.h"
#include "DataCenter.h"

int main(int argc, char **argv)
{
    /*          if (argc != 4)
    {
        printf("usage: ./client <ipaddress>\n");
        return 0;
    }
    unsigned char key[] = "abcabcabc234abc";
    AESCoderDecipher *aescode = new AESCoderDecipher(key);
    UDPMultiCastClient *cli = new UDPMultiCastClient(argv[1], atoi(argv[2]), aescode, argv[3]);
    cli->StartService();  */
    if (argc != 3)
    {
        printf("usage: ./client <ipaddress>\n");
        return 0;
    }

    unsigned char key[] = "abcabcabc234abc";
    AESCoderDecipher *aescode = new AESCoderDecipher(key);
    TCPClient *cli = new TCPClient(argv[1], atoi(argv[2]), aescode);
    cli->StartService();

    //pause();
}