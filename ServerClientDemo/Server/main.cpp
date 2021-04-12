#include "Server.h"
#include "Receiver.h"
#include "Mediator.h"
#include "Contact.h"
#include "CoderDecipher.h"

ofstream Logger::m_error_log_file;
ofstream Logger::m_info_log_file;
ofstream Logger::m_warn_log_file;

ThreadPool *ThreadPool::instance = new ThreadPool();
class MonitorThreadAccept;
int main()
{

    initLogger("/home/ServerClientDemo/Server/info.txt", "/home/ServerClientDemo/Server/warn.txt", "/home/ServerClientDemo/Server/error.txt");
    /********************synchronise cooperation************************************/
    //进程新建TCP#UDP Server
    ServerFactory *serFac = new ServerFactory();
    Server *sertcp = serFac->CreateServerReceiverFactoryType(TCP, TCPUDPPORT, NULLADDRESS, NULLTTL, NULLLOOP);
    Server *serudp = serFac->CreateServerReceiverFactoryType(UDP, TCPUDPPORT, NULLADDRESS, NULLTTL, NULLLOOP);
    Server *serudpmulticast = serFac->CreateServerReceiverFactoryType(UDPMultiCast, UDPMULTICASTPORT, UDPMULTICASTADDRESS, TTL, LOOPPORT);

    //将TCP server与UDP server构造与表示分离，统一接口， 方便以后扩展功能
    Direcrtor *condirector = new ConDirector();
    condirector->AppendServer(sertcp);
    condirector->AppendServer(serudp);
    condirector->AppendServer(serudpmulticast);

    condirector->ServerMode();

    cout << "udpmulti addr = " << inet_ntoa(*(struct in_addr *)&serudpmulticast->GetMreqMultiCast()->imr_multiaddr) << "(port --add): " << endl;
    //新建Server处理,Receiver处理,为Receiver加入AES加解密算法处理模块

    ServerConnector *serConn = new ServerConnector(sertcp, serudp, serudpmulticast);
    /*数据处理基本模块*/
    DataCenter *datacenter = new DataCenter();
    /*AES算法基本模块*/
    unsigned char key[] = AESKEY;
    AESCoderDecipher *aescoding = new AESCoderDecipher(key);

    /*单线程的UDP与UDP组播子模块*/
    ReceiverFactory *m_RecFac = new ReceiverFactory();
    Receiver *recudp = m_RecFac->CreateReceiverType(UDP, serudp->GetListenFd(), aescoding, (struct sockaddr *)serudp->GetSockAddrIn(), datacenter);

    cout << "multicast addr= " << serudpmulticast->GetSockAddrIn()->sin_addr.s_addr << " (port)" << serudpmulticast->GetSockAddrIn()->sin_port << endl;
    Receiver *recudpmul = m_RecFac->CreateReceiverType(UDP, serudpmulticast->GetListenFd(), aescoding, (struct sockaddr *)serudpmulticast->GetSockAddrIn(), datacenter);
    Receiver *recudpmulproxy = new UDPMultiCastReceiverProxy(recudpmul);

    /*TCP多线程子模块*/
    ThreadAcceptNotifier *monitortcp = new MonitorThreadAccept();
    TCPMultiThreadReceiverAdapter *rectcp = new TCPMultiThreadReceiverAdapter(monitortcp, sertcp, aescoding, datacenter);

    /*子模块组合入Connector工作*/
    ReceiverConnector *recConn = new ReceiverConnector(rectcp, recudp, recudpmulproxy, aescoding);

    //新建协调者
    Mediator *mediator = new ServerReceiverMediator();

    //Server#Receiver加入协调者
    serConn->SetMediator(mediator);
    recConn->SetMediator(mediator);

    //协调者加入连接着与接受者
    mediator->SetServerConnect(serConn);
    mediator->SetReceiverConnect(recConn);

    //连接者开始处理

    /*Server监听模块*/
    MonitorAccept *m_monAccept = new MonitorAccept(sertcp->GetListenFd(), serudp->GetListenFd(), serudpmulticast->GetListenFd(), serConn);
    m_monAccept->ServerSocketAccept();

    delete aescoding;
    delete datacenter;

    pause();
}