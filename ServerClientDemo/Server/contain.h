#ifndef CONTAIN_H
#define CONTAIN_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <queue>
#include <utility>
#include <list>
#include <vector>
#include <cstdint>
#include <ctime>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <bitset>
#include <algorithm>
#include <cmath>

#define MAXLINE 4096
#define MAXLINEREC 50
#define PORTSIZE 50
#define MAX_EVENT_NUMBER 1024
#define MAX_NETPACK_SIZE 10000
#define MAX_DATA_SIZE 4096
#define SEND_DATA_SIZE 4108
#define TCPUDPPORT 8080
#define UDPMULTICASTPORT 8081
#define NULLADDRESS "0.0.0.0"
#define UDPMULTICASTADDRESS "224.0.0.88"
#define LOOPPORT 10
#define NULLTTL 0
#define NULLLOOP 0
#define TTL 1
#define AESKEY "abcabcabc234abc"

enum ServerType
{
	UDP = 1,
	UDPMultiCast,
	TCP
};
enum SignalType
{
	NORMAL = 1,
	HANGUP,
	SLEEP,
	CLOSE
};
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