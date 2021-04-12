#include "Server.h"

void UDPServer::InitSocket()
{
    if ((m_nListenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    { //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
        LOG(ERROR) << "UDP create socket ErrorServer: " << strerror(errno) << " (errno)" << errno << endl;
        // 返回值==-1，报错
    }
    else
    {
        LOG(INFO) << " UDP create socket sucessful: "
                  << "(listenFd: )" << m_nListenfd << endl;
    }
    memset(&m_sockServaddr, 0, sizeof(m_sockServaddr));
    //将servaddr结构体的内容置0
    m_sockServaddr.sin_family = AF_INET;
    //规定协议族为SOCK_STREAM
    m_sockServaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //这里表示的是需要监听的IP地址，也就是接受哪些IP发过来的数据包，INADDR_ANY任意，即0.0.0.0  如果是本地通信，那么就是127.0.0.1
    m_sockServaddr.sin_port = htons(m_nPort);
    //绑定端口
};
void UDPServerMltiCast::InitSocket()
{
    if ((m_nListenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    { //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
        LOG(ERROR) << "UDP MultiCasting create socket ErrorServer: " << strerror(errno) << " (errno)" << errno << endl;
        // 返回值==-1，报错
    }
    else
    {
        LOG(INFO) << " UDP MultiCasting create socket sucessful: "
                  << "(listenFd: )" << m_nListenfd << endl;
    }
    //设置socket复用
    int yes = 1;
    if (setsockopt(m_nListenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    {
        LOG(ERROR) << " reusing ADDR failed" << endl;
    }
    else
    {
        LOG(INFO) << " Set MultiCasting addr multiplexing sucessful!" << endl;
    }

    memset(&m_SockGroupMultiCast, 0, sizeof(struct sockaddr_in));
    m_SockGroupMultiCast.sin_family = AF_INET;
    m_SockGroupMultiCast.sin_port = htons(m_nPort);
    m_SockGroupMultiCast.sin_addr.s_addr = inet_addr("224.0.0.88");

    memset(&m_SockLocalMultiCast, 0, sizeof(m_SockLocalMultiCast));
    m_SockLocalMultiCast.sin_family = AF_INET;
    m_SockLocalMultiCast.sin_addr.s_addr = htonl(INADDR_ANY);
    m_SockLocalMultiCast.sin_port = htons(m_nPort);
    cout << "Mul Address :" << m_SockLocalMultiCast.sin_addr.s_addr << " (port) " << htons(m_SockLocalMultiCast.sin_port) << endl;
};

void TCPServer::InitSocket()
{
    if ((m_nListenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    { //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
        LOG(ERROR) << " TCP create socket ErrorServer: " << strerror(errno) << " (errno)" << errno << endl;
        // 返回值==-1，报错
    }
    else
    {
        LOG(INFO) << " TCP create socket sucessful: "
                  << "(listenFd: )" << m_nListenfd << endl;
    }
    memset(&m_sockServaddr, 0, sizeof(m_sockServaddr));
    //将servaddr结构体的内容置0
    m_sockServaddr.sin_family = AF_INET;
    //规定协议族为SOCK_STREAM
    m_sockServaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //这里表示的是需要监听的IP地址，也就是接受哪些IP发过来的数据包，INADDR_ANY任意，即0.0.0.0  如果是本地通信，那么就是127.0.0.1
    m_sockServaddr.sin_port = htons(m_nPort);
    //绑定端口
}

void UDPServer::SocketBind()
{
    if (bind(m_nListenfd, (struct sockaddr *)&m_sockServaddr, sizeof(m_sockServaddr)) == -1) //参数分别是之前的文件描述符，网络通信地址及其大小
    {
        LOG(ERROR) << " UDP Socket bind error:  " << strerror(errno) << " (errno)" << errno << endl;
    }
    else
    {
        LOG(INFO) << " UDP Socket bind sucessful:  "
                  << "(Listenfd: )" << m_nListenfd << endl;
    }
};
void UDPServerMltiCast::SocketBind()
{
    //绑定socket
    int err = bind(m_nListenfd, (struct sockaddr *)&m_SockLocalMultiCast, sizeof(m_SockLocalMultiCast));
    if (err < 0)
    {
        LOG(ERROR) << " Bind UDP MultiCasting error:" << endl;
        exit(1);
    }
    else
    {
        LOG(INFO) << " Bind UDP MultiCasting sucessful!"
                  << "(Listenfd: )" << m_nListenfd << endl;
        cout << "bind Mul Address :" << m_SockLocalMultiCast.sin_addr.s_addr << " (port) " << m_SockLocalMultiCast.sin_port << endl;
    }

    //加入组播组

    memset(&m_Mreq, 0, sizeof(m_Mreq));
    m_Mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.88");
    cout << "Mul Address :" << m_Mreq.imr_multiaddr.s_addr << " (local address) " << inet_ntoa(*(struct in_addr *)&m_Mreq.imr_interface.s_addr) << endl;
    m_Mreq.imr_interface.s_addr = htonl(INADDR_ANY); //本机地址
                                                     //多播地址

    cout << "Mul Address :" << inet_ntoa(*(struct in_addr *)&m_Mreq.imr_multiaddr.s_addr) << " (local address) " << inet_ntoa(*(struct in_addr *)&m_Mreq.imr_interface.s_addr) << endl;

    //初始化组播属性
    this->InitMultiCastAttribute();
    this->MultiCastTTLPassageInit();
    this->MultiCastLoopPassageInit();
};
void UDPServerMltiCast::BindEstablish()
{
    this->InitSocket();
    this->SocketBind();
    //this->SocketListen();
};
void UDPServer::BindEstablish()
{
    this->InitSocket();
    this->SocketBind();
    //this->SocketListen();
};

void TCPServer::SocketBind()
{
    if (bind(m_nListenfd, (struct sockaddr *)&m_sockServaddr, sizeof(m_sockServaddr)) == -1) //参数分别是之前的文件描述符，网络通信地址及其大小
    {
        LOG(ERROR) << " TCP Socket bind error:  " << strerror(errno) << " (errno)" << errno << endl;
    }
    else
    {
        LOG(INFO) << " TCP Socket bind sucessful:  "
                  << "(Listenfd: )" << m_nListenfd << endl;
    }
}

void TCPServer::SocketListen()
{
    if (listen(m_nListenfd, 5) == -1) //文件描述符以及最大连接数
    {
        LOG(ERROR) << " listen socket ErrorServer: " << strerror(errno) << " " << errno << endl;
    }
    else
    {
        LOG(INFO) << " Socket Listening establish" << endl;
    }
}

int TCPServer::GetListenFd()
{
    return m_nListenfd;
}

int UDPServer::GetListenFd()
{
    return m_nListenfd;
}

int UDPServerMltiCast::GetListenFd()
{
    return m_nListenfd;
}

struct sockaddr_in *UDPServer::GetSockAddrIn()
{
    return &m_sockServaddr;
};

struct sockaddr_in *UDPServerMltiCast::GetSockAddrIn()
{
    return &m_SockGroupMultiCast;
};

void TCPServer::ListenEstablish()
{
    this->InitSocket();
    this->SocketBind();
    this->SocketListen();
}

Server *ServerFactory::CreateServerReceiverFactoryType(ServerType servercatgories, int port, char *multicastIPAddr, int ttl, int loop)
{
    switch (servercatgories)
    {
    case TCP:
        LOG(INFO) << " One TCPServer ready to construct" << endl;
        return new TCPServer(port);
        break;
    case UDP:
        LOG(INFO) << " One UDPServer ready to construct" << endl;
        return new UDPServer(port);
        break;
    case UDPMultiCast:
        LOG(INFO) << " One UDPMultiCast ready to construct" << endl;
        return new UDPServerMltiCast(port, multicastIPAddr, ttl, loop);
        break;
    default:
        LOG(ERROR) << " Server create error: Could not found the catogories:" << endl;
        return NULL;
    }
}

void ConDirector::ServerMode()
{
    list<Server *>::iterator it;
    for (it = m_Server.begin(); it != m_Server.end(); it++)
    {
        (*it)->InitSocket();
        (*it)->SocketBind();
        (*it)->SocketListen();
    }
}

void UDPServerMltiCast::MultiCastTTLPassageInit()
{
    //设置多播的TTL值
    if (setsockopt(m_nListenfd, IPPROTO_IP, IP_MULTICAST_TTL, &m_nTTL, sizeof(m_nTTL)) < 0)
    {
        LOG(INFO) << "Multicast TTL setting error(error):" << endl;
        return;
    }
    else
    {
        LOG(INFO) << " Multicast TTL setting sucessful" << endl;
    }
}

void UDPServerMltiCast::MultiCastLoopPassageInit()
{
    //设置数据是否发送到本地回环接口
    if (setsockopt(m_nListenfd, IPPROTO_IP, IP_MULTICAST_LOOP, &m_nLoop, sizeof(m_nLoop)) < 0)
    {
        LOG(INFO) << "Multicast Loop setting error(error):" << endl;
        return;
    }
    else
    {
        LOG(INFO) << " Multicast Loop setting sucessful" << endl;
    }
}

void UDPServerMltiCast::InitMultiCastAttribute()
{
    int err = setsockopt(m_nListenfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &m_Mreq, sizeof(m_Mreq));
    if (err < 0)
    {
        LOG(INFO) << "setsockopt():IP_ADD_MEMBERSHIP"
                  << " err :" << errno << endl;
    }
    else
    {
        LOG(INFO) << "Set ip req sucessful" << endl;
        cout << "Mul Address :" << inet_ntoa(*(struct in_addr *)&m_Mreq.imr_multiaddr.s_addr) << " (local address) " << inet_ntoa(*(struct in_addr *)&m_Mreq.imr_interface.s_addr) << endl;
    }
}