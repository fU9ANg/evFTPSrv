
/*
 * d2MemZone.h
 */

#ifndef _D2_MEMZONE_H
#define _D2_MEMZONE_H

#include "d2Define.h"
#include "d2MutexLock.h"

#define MEMBLOCKNUM     2048

template <typename T>
class d2MemZone
{
    typedef queue<T*> QUEUEBLOCK;

    public:
        /*
        =====================
          d2MemZone::d2MemZone ()
        =====================
        */
        d2MemZone (int iCount = MEMBLOCKNUM)
        {
            d2MutexLockGuard guard (m_mutexLock);
            this->m_blockCount = iCount;
            for (int i=0; i<this->m_blockCount; i++)
            {
                T* t = new T;
                m_queueBlock.push (t);
            }
        }

        /*
        =====================
          d2MemZone::~d2MemZone ()
        =====================
        */
        ~d2MemZone (void)
        {
            d2MutexLockGuard guard (m_mutexLock);
            for (int i=0; i<this->m_blockCount; i++)
            {
                T* t = m_queueBlock.front ();
                m_queueBlock.pop ();
                delete t;
            }
        }

        /*
        =====================
          d2MemZone::malloc ()
        =====================
        */
        T* malloc (void)
        {
            d2MutexLockGuard guard (m_mutexLock);
            if (m_queueBlock.size () == 0)
            {
                return (NULL);
            }
            T* p = m_queueBlock.front ();
            m_queueBlock.pop ();
            return (p);
        }

        /*
        =====================
          d2MemZone::free ()
        =====================
        */
        bool free (T* t)
        {
            d2MutexLockGuard guard (m_mutexLock);
            if (t == NULL) {
                return (false);
            }
            m_queueBlock.push (t);
            return (true);
        }

        /*
        =====================
          d2MemZone::getCount ()
        =====================
        */
        int getCount (void)
        {
            return (m_blockCount);
        }

        /*
        =====================
          d2MemZone::getUsedCount ()
        =====================
        */
        int getUsedCount (void)
        {
            d2MutexLockGuard guard (m_mutexLock);
            return (m_blockCount - \
                    m_queueBlock.size ());
        }

    private:
        QUEUEBLOCK   m_queueBlock;
        d2MutexLock  m_mutexLock;
        int          m_blockCount;
};

#endif  //_D2_MEMZONE_H
