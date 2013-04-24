
/*
 * d2EpollLoopTask.h
 */

#ifndef	_D2_EPOLLLOOPTASK_H
#define _D2_EPOLLLOOPTASK_H

#include "d2Define.h"
#include "d2Task.h"
#include "d2MemBlock.h"
#include "d2MemZone.h"
#include "d2SingleFactory.h"
#include "d2Queue.h"

#define MAX_EVENTS  10240
// define epoll io information
typedef struct ev_io_info
{
    public:
        int             fd;
        time_t          lasttime;
}   ev_io_info;

class d2EpollLoopTask : public d2Task
{

    public:
         d2EpollLoopTask (string p, int port);
        ~d2EpollLoopTask ();

        virtual int Execute (void* data);
        int  getClientCount ();
        static void handleAccept (int listenFd);
        static void handleRecv   (int recvFd);
        static void handleTimeOut(int recvFd);

        static void setNonBlock  (int fd);
        static void setNodelay   (int fd);
        static void setReuseAddr (int fd);
        static void closeFd      (int fd);

    public:
        static struct epoll_event m_ev;
        static struct epoll_event m_events[MAX_EVENTS];
        static int    m_epollFd;
        static int    m_listenFd;
        static struct ev_io_info m_ioArray[MAXFD];
        static D2TASKQUEUE m_TaskQueue;

    private:
        static int  m_clientCount;
        string m_Ip;
        int m_Port;
        int initInetAddr (void);
};


#endif	//_D2_EPOLLLOOPTASK_H
