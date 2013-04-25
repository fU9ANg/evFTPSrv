
/*
 * d2SelectLoopTask.h
 */

#ifndef	_D2_SELECTLOOPTASK_H
#define _D2_SELECTLOOPTASK_H

#include "d2Define.h"
#include "d2Task.h"
#include "d2MemBlock.h"
#include "d2MemZone.h"
#include "d2SingleFactory.h"
#include "d2Queue.h"

// define select io information
typedef struct ev_io_info
{
    public:
        int              fd;
        time_t           lasttime;
}   ev_io_info;

class d2SelectLoopTask : public d2Task
{

    public:
         d2SelectLoopTask (string p, int port);
        ~d2SelectLoopTask ();

        virtual int Execute (void* data);
        int InIoArray (int fd);
        int getClientCount ();
        static int  handleAccept (int listenFd);
        static int  handleRecv   (int recvFd);
        static void handleTimeOut(int recvFd);

        static void setNonBlock  (int fd);
        static void setNodelay   (int fd);
        static void setReuseAddr (int fd);
        static void closeFd      (int fd);

    public:
        static struct ev_io_info m_ioArray[MAXFD];
        static D2TASKQUEUE m_TaskQueue;

    private:
        static int  m_clientCount;
        string m_Ip;
        int    m_Port;
        int    m_listenFd;

        int    initInetAddr (void);
};


#endif	//_D2_SELECTLOOPTASK_H
