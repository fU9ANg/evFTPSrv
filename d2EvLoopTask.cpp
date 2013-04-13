
/*
 * d2EvLoopTask.cpp
 */

#include "d2Define.h"
#include "d2Task.h"
#include "d2EvLoopTask.h"

struct ev_io_info d2EvLoopTask::m_ioArray[MAXFD];
struct ev_loop* d2EvLoopTask::m_Loop = NULL;
D2TASKQUEUE d2EvLoopTask::m_TaskQueue;
int d2EvLoopTask::m_clientCount = 0;

/*
=====================
  d2EvLoopTask::d2EvLoopTask ()
=====================
*/
d2EvLoopTask::d2EvLoopTask (string ip, int port)
{
    m_Ip   = ip;
    m_Port = port;

    if ((m_listenFd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR: in socket ()\n";
        return;
    }

    d2EvLoopTask::setNonBlock (m_listenFd);
    d2EvLoopTask::setNodelay  (m_listenFd);
    d2EvLoopTask::setReuseAddr(m_listenFd);

    for (int i=0; i<MAXFD; ++i)
    {
        d2EvLoopTask::m_ioArray[i].fd = NULL;
    }
}

/*
=====================
  d2EvLoopTask::~d2EvLoopTask ()
=====================
*/
d2EvLoopTask::~d2EvLoopTask ()
{
    for (register int i=0; i<MAXFD; i++)
    {
        d2EvLoopTask::closeFd (i);
    }

    close (m_listenFd);
}

/*
=====================
  d2EvLoopTask::Execute ()
=====================
*/
int d2EvLoopTask::Execute (void* data)
{
    initInetAddr ();
    ev_io  watcher;
    ev_timer timer;

    d2EvLoopTask::m_Loop = ev_loop_new (EVBACKEND_EPOLL);

    ev_io_init  (&watcher, handleAccept, m_listenFd, EV_READ);
    ev_io_start (d2EvLoopTask::m_Loop, &watcher);

    ev_timer_init  (&timer, handleTimeOut, 5, 5);
    ev_timer_start (d2EvLoopTask::m_Loop, &timer);

    ev_loop (d2EvLoopTask::m_Loop, 0);

    ev_loop_destroy (d2EvLoopTask::m_Loop);

    return (0);
}

/*
=====================
  d2EvLoopTask::handleAccept ()
=====================
*/
void d2EvLoopTask::handleAccept (struct ev_loop* l, ev_io* w, int revents)
{
    struct sockaddr_in clientAddr;
    socklen_t sockLen = 0;
    int newFd = 0;

    newFd = accept (w->fd, (struct sockaddr*)&clientAddr, &sockLen);
    if (newFd < 0)
    {
        cout << "ERROR: in accept ()\n";
        return;
    }
    d2EvLoopTask::setNonBlock (newFd);
    d2EvLoopTask::setReuseAddr(newFd);
    d2EvLoopTask::setNodelay  (newFd);

    d2EvLoopTask::m_ioArray[newFd].fd = (ev_io*) malloc (sizeof (ev_io));
    d2EvLoopTask::m_ioArray[newFd].lasttime = ev_time();

    ev_io_init  (d2EvLoopTask::m_ioArray[newFd].fd, handleRecv, newFd, EV_READ);
    ev_io_start (d2EvLoopTask::m_Loop, d2EvLoopTask::m_ioArray[newFd].fd);

    d2EvLoopTask::m_clientCount++;
    return;
}

/*
=====================
  d2EvLoopTask::handleRecv
=====================
*/
void d2EvLoopTask::handleRecv (struct ev_loop* l, ev_io* w, int revents)
{
    d2MemBlock* block = D2SINGLEFACTORY->m_memZone.malloc ();
    if (block == NULL)
    {
        cout << "MALLOC-ERROR: out of memory" << endl;
        return;
    }

#if 1
    int recv_len = 0;

    // packet header
    recv_len = D2SINGLEFACTORY->recvData (w->fd, block->data(), sizeof (int));
    if (recv_len != sizeof (int))
    {
        cout << "[Warning]: Packet header length-" << sizeof (int) \
             << ", actually received length-" << recv_len <<endl;
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
        d2EvLoopTask::closeFd(w->fd);
        return;
    }

    int *len = (int*)block->data();
    // packet body
    recv_len = D2SINGLEFACTORY->recvData (w->fd, (char*)block->data()+sizeof(int), *len-sizeof(int));
    if ((*len - sizeof (int)) != (unsigned int)recv_len)
    {
        cout << "FD=" << w->fd << ": length=" << *len - sizeof (int) << \
                ", actually recviced length=" << recv_len << endl;
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
        d2EvLoopTask::closeFd (w->fd);
        return;
    }

    d2EvLoopTask::m_ioArray[w->fd].lasttime = ev_time ();
    block->setFd (w->fd);
    D2SINGLEFACTORY->m_recvQueue.inQueue (block);
#endif
    return;
}

/*
=====================
  d2EvLoopTask::handleTimeOut ()
=====================
*/
void d2EvLoopTask::handleTimeOut(struct ev_loop* l, struct ev_timer* t, int revents)
{
    ev_tstamp now = ev_time ();
    for (register int i=0; i<MAXFD; ++i)
    {
        if (m_ioArray[i].fd != NULL)
        {
            if (TIMEOUT < now - m_ioArray[i].lasttime)
            {
                d2EvLoopTask::closeFd (i);
            }
        }
    }
    return;
}

/*
=====================
  d2EvLoopTask::setNonBlock ()
=====================
*/
void d2EvLoopTask::setNonBlock (int fd)
{
    fcntl (fd, F_SETFL, fcntl (fd, F_GETFL, 0) | O_NONBLOCK);
}

/*
=====================
  d2EvLoopTask::setReuseAddr ()
=====================
*/
void d2EvLoopTask::setReuseAddr(int fd)
{
    int reuse = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse));
}

/*
=====================
  d2EvLoopTask::setNodelay ()
=====================
*/
void d2EvLoopTask::setNodelay (int fd)
{
    int nodelay = 1;
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay));
}

/*
=====================
  d2EvLoopTask::closeFd ()
=====================
*/
void d2EvLoopTask::closeFd (int fd)
{
    if (fd)
    {
        close (fd);
        ev_io_stop (m_Loop, d2EvLoopTask::m_ioArray[fd].fd);
        free (d2EvLoopTask::m_ioArray[fd].fd);
        d2EvLoopTask::m_ioArray[fd].fd = NULL;
        d2EvLoopTask::m_clientCount--;
    }
}

/*
=====================
  d2EvLoopTask::getClientCount ()
=====================
*/
int d2EvLoopTask::getClientCount (void)
{
    return d2EvLoopTask::m_clientCount;
}

/*
=====================
  d2EvLoopTask::initInetAddr ()
=====================
*/
int d2EvLoopTask::initInetAddr (void)
{
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons (m_Port);
    servAddr.sin_addr.s_addr = inet_addr (m_Ip.c_str());

    if (bind (m_listenFd, (struct sockaddr*)&servAddr, \
        sizeof (struct sockaddr)) < 0)
    {
        abort ();
        return (-1);
    }
    listen (m_listenFd, LISTEN_QMAX);
    return (m_listenFd);
}
