#ifndef CONTAIN_H
#define CONTAIN_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <bitset>

#define MAXLINE 4096
#define SEND_DATA_SIZE 4108
#define MAX_NETPACK_SIZE 10000
#define MAX_DATA_SIZE 4096
#define MAX_RECV_SIZE 4108
enum DataType
{
	Static = 1,
	Dynamic
};
/*Type of header of TCP structure*/
typedef struct TCPParcelHeader
{
	int nDataType; //identify actual type of data
	unsigned int nDataSize; //genuine size of data
} TCPParcelHeader;

/*Type of TCP packet*/
typedef struct TCPParcel
{
	TCPParcelHeader dataHeader; //header of Tcp packet
	char szData[MAX_DATA_SIZE]; //genuine date of delievering
} TCPParcel;

using namespace std;

#endif
