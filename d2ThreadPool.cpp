
/*
 * d2ThreadPool.cpp
 */

#include "d2Define.h"
#include "d2ThreadPool.h"

using namespace std;

bool d2ThreadPool::initThreads ()
{
    for (unsigned int i=0; i<m_threadCount; i++) {
        pthread_t id = 0;
        pthread_create (&id, 0, d2ThreadPool::threadFunc, this);
        m_threadList.push_back (id);
    }
    return true;
}

bool d2ThreadPool::reSize (unsigned int num)
{
    d2MutexLockGuard guard (m_mutexLock);
    int count = (int) (num-m_threadCount);
    if (count > 0) {
        for (int i=0; i<count; ++i) {
            cout << i << ":::reSize of thread list:::" << count << endl;
            pthread_t id = 0;
            pthread_create (&id, 0, d2ThreadPool::threadFunc, this);
            m_threadList.push_back (id);
        }
    }
    m_threadCount = num;
    return true;
}

bool d2ThreadPool::runAll ()
{
    return (initThreads ());
}

bool d2ThreadPool::joinTask (d2Task* t)
{
    return this->m_taskQueue.inQueue (t);
}

void* d2ThreadPool::threadFunc (void* data)
{
    cout << "threadfunc\n";
    (void)signal (SIGINTERNAL, d2ThreadPool::sigHandler);
    d2ThreadPool* tp = static_cast<d2ThreadPool*> (data);
    d2Task* t = NULL;
    while (!tp->m_isExit) {
        if (tp->m_taskQueue.outQueue (t, 2) == true && t)
        {
            t->Execute (NULL);
            if (t->isAutoExit ()) {
                delete t;
                t = NULL;
            }
        }
#if 1
        else {
            d2MutexLockGuard* guard = new d2MutexLockGuard (tp->m_mutexLock);
            if (tp->m_threadCount < tp->m_threadList.size ()) {
                cout << "...........................................:" << pthread_self () << endl;
                tp->m_threadList.remove (pthread_self ());
                delete guard;
                return NULL;
            }
            delete guard;
        }
#endif
    }
    d2MutexLockGuard guard (tp->m_mutexLock);
    tp->m_threadList.remove (pthread_self ());
    return NULL;
}
bool d2ThreadPool::stopPool (void)
{
    m_isExit = true;
    while (this->getThreadCount() != 0)
    {
        usleep (100);
    }
    return false;
}
int d2ThreadPool::killAll (void)
{
    d2MutexLockGuard guard (this->m_mutexLock);
    while (getThreadCount() != 0) {
        pthread_kill (m_threadList.front (), SIGINTERNAL);
        m_threadList.pop_front ();
    }
    return (0);
}
void d2ThreadPool::sigHandler (int sig)
{
    printf ("[%ld]exit\n", pthread_self ());
    pthread_exit (NULL);
    return;
}
int d2ThreadPool::getThreadCount ()
{
    d2MutexLockGuard guard (m_mutexLock);
    return (m_threadList.size ());
}
int d2ThreadPool::getTaskCount ()
{
    return (m_taskQueue.getCount ());
}
