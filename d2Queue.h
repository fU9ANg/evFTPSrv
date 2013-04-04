
/*
 * d2Queue.h
 */

#ifndef _D2_QUEUE_H
#define _D2_QUEUE_H

#include "d2Define.h"

template <typename TYPE>
class d2Queue
{
    public:
        /*
        =====================
          d2Queue::d2Queue ()
        =====================
        */
        d2Queue (void)
        {
            pthread_mutex_init (&m_mutex, NULL);
            pthread_cond_init  (&m_cond,  NULL);
        }

        /*
        =====================
          d2Queue::~d2Queue ()
        =====================
        */
        ~d2Queue (void)
        {
            pthread_mutex_destroy (&m_mutex);
            pthread_cond_destroy  (&m_cond);
        }

        /*
        =====================
          d2Queue::inQueue ()
        =====================
        */
        bool inQueue (TYPE t)
        {
            pthread_mutex_lock (&m_mutex);
            cout << "=========" << endl << "in Queue: " << pthread_self () << endl;
            if (m_queue.empty ()) {
                cout << "in Queue_send_siganl() : " << pthread_self () << endl;
                m_queue.push (t);
                cout << "===========" << endl;
                pthread_mutex_unlock (&m_mutex);
                pthread_cond_signal (&m_cond);
            }
            else {
                cout << "in Queue_!empty() : " << pthread_self () <<endl<<"=========="<< endl;
                m_queue.push (t);
                pthread_mutex_unlock (&m_mutex);
            }
            return true;
        }

        /*
        =====================
          d2Queue::outQueue ()
        =====================
        */
        bool outQueue (TYPE& out, unsigned int iTimeOut)
        {
            struct timespec t;
            t.tv_sec = time (NULL) + iTimeOut;
            t.tv_nsec = 0;
            pthread_mutex_lock (&m_mutex);
            cout << "-------------" << endl << "outQueue: " << pthread_self () << endl;
            while (m_queue.empty ()) {
                cout << "outQueue.empty(): " << pthread_self () << endl;
                if (pthread_cond_timedwait (&m_cond, &m_mutex, &t) != 0) {
                    cout << "timewait" << endl << "-------------" << endl;
                    pthread_mutex_unlock (&m_mutex);
                    return false;
                }
            }
            out = m_queue.front ();
            m_queue.pop ();
            cout << "outQueue.get_task: " << pthread_self () << endl <<
                    "---------------" << endl;
            pthread_mutex_unlock (&m_mutex);

            return true;
        }

        /*
        =====================
          d2Queue::getCount ()
        =====================
        */
        size_t getCount (void)
        {
            pthread_mutex_lock (&m_mutex);
            size_t size = m_queue.size ();
            pthread_mutex_unlock (&m_mutex);
            return (size);
        }

    private:
        pthread_mutex_t m_mutex;
        pthread_cond_t  m_cond;
        queue<TYPE>     m_queue;
};

#endif //_D2_QUEUE_H
