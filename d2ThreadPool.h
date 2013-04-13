
/*
 * d2ThreadPool.h
 */

#ifndef	_D2_THREADPOOL_H
#define _D2_THREADPOOL_H

#include "d2Define.h"
#include "d2Queue.h"
#include "d2Task.h"
#include "d2MutexLock.h"

class d2ThreadPool
{

    typedef list <pthread_t> D2THREADLIST;
    typedef d2Queue<d2Task*> D2TASKQUEUE;

    public:
        d2ThreadPool (unsigned int num) : m_threadCount(num)
        {
            m_isExit = false;
        }

        ~d2ThreadPool ()
        {
        }

        bool initThreads (void);
        bool runAll (void);
        bool joinTask (d2Task* t);
        bool stopPool (void);
        bool reSize (unsigned int num);

        static void  sigHandler (int sig);
        static void* threadFunc (void* data);

        int killAll (void);
        int getThreadCount (void);
        int getTaskCount (void);

    private:
        D2THREADLIST    m_threadList;
        D2TASKQUEUE     m_taskQueue;
        unsigned int    m_threadCount;
        d2MutexLock     m_mutexLock;
        bool            m_isExit;
};

#endif  //_D2_THREADPOOL_H
