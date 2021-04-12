#include "Contact.h"

void MonitorThreadAccept::AppendClientListenner(ClientListener *ilistener)
{
    if (ilistener != NULL)
    {
        listenners.push_back(ilistener);
    }
}

void MonitorThreadAccept::RemoveClientListenner(ClientListener *ilistener)
{
    if (ilistener != NULL)
    {
        list<ClientListener *>::iterator it;
        for (it = listenners.begin(); it != listenners.end(); it++)
        {
            if (*it == ilistener)
            {
                listenners.remove(ilistener);
                break;
            }
        }
    }
}

void MonitorThreadAccept::NotifyParticipant(int connfd)
{
    list<ClientListener *>::iterator it;
    for (it = listenners.begin(); it != listenners.end(); it++)
    {
        (*it)->UpdateOperation(connfd);
    }
}

void MonitorThreadAccept::SocketAccept()
{
    LOG(INFO) << endl
              << "******************Wait for the client response(Thread Process)******************" << endl;

    if ((m_nConnFdThread = accept(m_nListenFd, (struct sockaddr *)NULL, NULL)) == -1)
    { //accept用来接受端口发来的数据，connfd文件描述符
        LOG(ERROR) << "Thread Socket accept from listenfd error, listenfd = " << m_nListenFd << endl;
        sleep(10);
    }
    else
    {
        LOG(INFO) << "Thread Socket accept from listenfd Sucess, connfd = " << m_nConnFdThread << endl;
        NotifyParticipant(m_nConnFdThread);
    }
}
