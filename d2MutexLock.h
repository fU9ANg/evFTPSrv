
/*
 * d2MutexLock.h
 */

#ifndef _D2_MUTEXLOCK_H
#define _D2_MUTEXLOCK_H

#include "d2Define.h"
#include <pthread.h>

class d2MutexLock
{
    public:
        /*
        =====================
          d2MutexLock::d2MutexLock ()
        =====================
        */
        d2MutexLock ()
        {
            pthread_mutex_init (&m_mutex, NULL);
        }

        /*
        =====================
          d2MutexLock::~d2MutexLock ()
        =====================
        */
        ~d2MutexLock ()
        {
            pthread_mutex_destroy (&m_mutex);
        }

        /*
        =====================
          d2MutexLock::enLock ()
        =====================
        */
        void enLock (void)
        {
            pthread_mutex_lock (&m_mutex);
        }

        /*
        =====================
          d2MutexLock::deLock ()
        =====================
        */
        void deLock (void)
        {
            pthread_mutex_unlock (&m_mutex);
        }

    private:
        d2MutexLock (const d2MutexLock&);
        const d2MutexLock& operator=(const d2MutexLock&);
        pthread_mutex_t m_mutex;
};


class d2MutexLockGuard
{
    public:
        /*
        =====================
          d2MutexLockGuard::d2MutexLockGuard ()
        =====================
        */
        explicit d2MutexLockGuard (d2MutexLock& lock) : m_lock(lock)
        {
            m_lock.enLock ();
        }

        /*
        =====================
          d2MutexLockGuard::~d2MutexLockGuard ()
        =====================
        */
        ~d2MutexLockGuard (void)
        {
            m_lock.deLock ();
        }

    private:
        d2MutexLockGuard (const d2MutexLockGuard&);
        const d2MutexLockGuard& operator=(const d2MutexLockGuard&);
        d2MutexLock& m_lock;
};

#endif //_D2_MUTEXLOCK_H
