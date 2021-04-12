#include "Process.h"

void *ThreadProcess::ThreadPrepare(void *threadObject)
{
    ThreadProcess *threadTmp = (ThreadProcess *)threadObject;
    threadTmp->ThreadRun(threadTmp->m_ConnfdTmpThread);
}

void ThreadProcess::ThreadRun(int connfdRun)
{
    m_Rec->SocketReceive(connfdRun, TCP);
}

void ThreadProcess::UpdateOperation(int connfd)
{
    this->ProcessInit(connfd);
}

void ThreadProcess::ProcessInit(int connfdpro)
{
    if (pthread_create(&m_threadTid, NULL, ThreadPrepare, (void *)this) != 0)
    {
        LOG(ERROR) << " Thread create error" << endl;
    }
    else
    {
        LOG(INFO) << " Thread create sucessful! thread number is " << m_threadTid << " (static function number):" << ThreadPrepare << endl;
        m_ConnfdTmpThread = connfdpro;
    }
}

void ThreadProcess::ProcessShutdown()
{
    LOG(INFO) << " Thread shutdown thread number is" << m_threadTid << endl;
    pthread_exit((void *)"thread over!\n");
}

void ThreadPool::AddThread(ThreadProcess *threadpro)
{
    m_pList->push_back(*threadpro);
    m_nThreadNum++;
}

void ThreadPool::DeleteThread()
{
    m_pList->pop_front();
}

int ThreadPool::GetThreadNum()
{
    return m_nThreadNum;
}
