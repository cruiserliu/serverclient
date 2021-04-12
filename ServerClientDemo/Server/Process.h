#ifndef PROCESS_H
#define PROCESS_H

#include "Receiver.h"
#include "Contact.h"
#include "Errorlog.h"

/*****************************************************************ProcessModule***********************************************************************************/
class Process
{
public:
    virtual void ProcessInit(int) = 0;
    virtual void ProcessShutdown() = 0;
    //virtual void ThreadSleep() = 0;
    //virtual void ThreadDetach() = 0;
    Receiver *m_Rec;
};

//aim at create thread when process(including TCP and UDP)
//depending on the specific situation

class ThreadProcess : public Process, public ClientListener
{
private:
    static void *ThreadPrepare(void *);

private:
    pthread_t m_threadTid;
    int m_nRes;

public:
    ThreadProcess(ServerType sertype, Receiver *recTmp)
    {
        LOG(INFO) << " Thread Process init "
                  << "(Receiver )" << recTmp << " Append" << endl;
        m_Rec = recTmp;
    };
    ~ThreadProcess()
    {
        if (m_Rec != NULL)
        {
            delete[] m_Rec;
        }
    };

    int m_ConnfdTmpThread;
    void ThreadRun(int);

    //override
    void UpdateOperation(int);

    void ProcessInit(int);
    void ProcessShutdown();
};

//attempt to establish one type of threadpool for the purpose of
//thread conjestion and storage overflow in mainframe Server
class ThreadPool
{
private:
    list<ThreadProcess> *m_pList;
    ThreadPool() { m_pList = new list<ThreadProcess>(); };
    static ThreadPool *instance;
    int m_nThreadNum = 0;

public:
    void AddThread(ThreadProcess *);
    void DeleteThread();
    int GetThreadNum();

public:
    static ThreadPool *getInstance()
    {
        return instance;
    };
};

#endif