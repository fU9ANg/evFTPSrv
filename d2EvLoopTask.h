
/*
 * d2EvLoopTask.h
 */

#ifndef	_D2_EVLOOPTASK_H
#define _D2_EVLOOPTASK_H

#include "d2Define.h"
#include "d2Task.h"
#include "d2MemBlock.h"
#include "d2MemZone.h"
#include "d2SingleFactory.h"
#include "d2Queue.h"

typedef struct ev_io_info {
    public:
        struct ev_io*   fd;
        ev_tstamp       lasttime;
}   ev_io_info;

class d2EvLoopTask : public d2Task
{

    public:
         d2EvLoopTask (string p, int port);
        ~d2EvLoopTask ();

        virtual int Execute (void* data);
        int  getClientCount ();
        static void handleAccept (struct ev_loop* l, ev_io* w, int revents);
        static void handleRecv   (struct ev_loop* l, ev_io* w, int revents);
        static void handleTimeOut(struct ev_loop* l, struct ev_timer* t, int revents);

        static void setNonBlock (int fd);
        static void setNodelay  (int fd);
        static void setReuseAddr(int fd);
        static void closeFd     (int fd);

    public:
        static struct ev_loop* m_Loop;
        static struct ev_io_info m_ioArray[MAXFD];
        static D2TASKQUEUE m_TaskQueue;

    private:
        static int  m_clientCount;
        string m_Ip;
        int m_Port;
        int m_listenFd;
        int initInetAddr (void);
};


#endif	//_D2_EVLOOPTASK_H
