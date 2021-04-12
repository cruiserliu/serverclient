#include "Mediator.h"

void ReceiverConnector::GetMessage(int listenfd, ServerType sertype)
{
    LOG(INFO) << " ReceiverConneter start to Get Message (listenfd): " << listenfd << " (Servertype): " << sertype << endl;
    switch (sertype)
    {
    case TCP:
        LOG(INFO) << " ReceiverConnecter select TCP Process" << endl;
        m_TCPReceiver->GetListenfdAda(listenfd);
        m_TCPReceiver->SocketAccept();
        break;
    case UDP:
        LOG(INFO) << " ReceiverConnecter select UDP Process" << endl;
        m_UDPReceiver->SocketReceive(listenfd, UDP);
        break;
    case UDPMultiCast:
        LOG(INFO) << " ReceiverConnecter select UDP MultiCast Process" << endl;
        //m_thredproConn = new ThreadProcess(UDP, m_pUDPMultiCast);
        cout << "listenfd multicast = " << listenfd << endl;
        m_UDPMulReceiver->SocketReceive(listenfd, UDPMultiCast);
        break;
    default:
        LOG(INFO) << " Found None" << endl;
        break;
    }
}

int MonitorAccept::SetNonBlocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void MonitorAccept::AddFd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    this->SetNonBlocking(fd);
}

void MonitorAccept::ServerSocketAccept()
{
    LOG(INFO) << endl
              << "******************Wait for the client response(Select Process)******************" << endl;
    for (;;)
    {
        /*      //使用select判断是否可以读
        if((m_nReady = select(m_nMaxfdp,&m_readSet,NULL,NULL,NULL)<0)){
            if(errno == EINTR){
                continue;
            } else {
                LOG(ERROR) << "select error ." << endl;
                exit(-1);
            }
        }
       //检测listenfd
        if(FD_ISSET(m_nTCPListenFd,&m_readSet)){
            m_ConnSelect->Send(m_nTCPListenFd, TCP, m_ConnSelect);
        }
        //检测udpfd
        if(FD_ISSET(m_nUDPFd,&m_readSet)){
            m_ConnSelect->Send(m_nUDPFd, UDP, m_ConnSelect);
        }
    }*/
        cout << "epoll fd:" << m_EpollFd << endl;
        int number = epoll_wait(m_EpollFd, events, MAX_EVENT_NUMBER, -1); //wait for the occurance of the incident in infinate way
        for (int i = 0; i < number; i++)
        { //epoll ready for the incident
            int sockfd = events[i].data.fd;
            if (sockfd == m_nTCPListenFd)
            {
                m_ConnSelect->Send(m_nTCPListenFd, TCP, m_ConnSelect);
            }
            else if (sockfd == m_nUDPFd)
            {
                m_ConnSelect->Send(m_nUDPFd, UDP, m_ConnSelect);
            }
            else if (sockfd == m_nUDPMultiCastFd)
            {
                m_ConnSelect->Send(m_nUDPMultiCastFd, UDPMultiCast, m_ConnSelect);
            };
        }
    }
}
