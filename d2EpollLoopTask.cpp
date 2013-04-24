
/*
 * d2EpollLoopTask.cpp
 */

#include "d2Define.h"
#include "d2Task.h"
#include "d2EpollLoopTask.h"

struct ev_io_info d2EpollLoopTask::m_ioArray[MAXFD];
struct ev_loop* d2EpollLoopTask::m_Loop = NULL;
D2TASKQUEUE d2EpollLoopTask::m_TaskQueue;
int d2EpollLoopTask::m_clientCount = 0;

/*
=====================
  d2EpollLoopTask::d2EpollLoopTask ()
=====================
*/
d2EpollLoopTask::d2EpollLoopTask (string ip, int port)
{
    m_Ip   = ip;
    m_Port = port;

    if ((m_listenFd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR: in socket ()\n";
        return;
    }

    d2EpollLoopTask::setNonBlock (m_listenFd);
    d2EpollLoopTask::setNodelay  (m_listenFd);
    d2EpollLoopTask::setReuseAddr(m_listenFd);

    for (int i=0; i<MAXFD; ++i)
    {
        d2EpollLoopTask::m_ioArray[i].fd = -1;
    }

    m_epollFd = epoll_create (MAX_EVENTS);
    if (m_epollFd == -1) {
        perror ("epoll_create");
        exit (EXIT_FAILURE);
    }

    m_ev.events = EPOLLIN;
    m_ev.data.fd = m_listenFd;
    if (epoll_ctl (m_epollFd, EPOLL_CTL_ADD, m_listenFd, &m_ev) == -1) {
        perror ("epoll_ctl: m_listenFd");
        exit (EXIT_FAILURE);
    }
}

/*
=====================
  d2EpollLoopTask::~d2EpollLoopTask ()
=====================
*/
d2EpollLoopTask::~d2EpollLoopTask ()
{
    for (register int i=0; i<MAXFD; i++)
    {
        d2EpollLoopTask::closeFd (i);
    }

    close (m_listenFd);
    close (m_epollFd);
}

/*
=====================
  d2EpollLoopTask::Execute ()
=====================
*/
int d2EpollLoopTask::Execute (void* data)
{
    int nfds;

    for (;;) {
        nfds = epoll_wait (m_epollFd, m_events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror ("epoll_wait");
            exit (EXIT_FAILURE);
        }

        for (int n=0; n<nfds; ++n) {
            if (m_events[n].data.fd == listenFd) {
                handleAccept (...);
            }
            else {
                handleRecv (m_events[n].data.fd);
            }
        }
    }
}

/*
=====================
  d2EpollLoopTask::handleAccept ()
=====================
*/
void d2EpollLoopTask::handleAccept (int listenFd)
{
    struct sockaddr_in clientAddr;
    socklen_t sockLen = 0;
    int newFd = 0;

    newFd = accept (m_listenFd, (struct sockaddr*)&clientAddr, &sockLen);

    if (newFd == -1) {
        perror ("accept");
        exit (EXIT_FAILURE);
    }
    //
    
    d2EpollLoopTask::setNonBlock (newFd);
    d2EpollLoopTask::setReuseAddr(newFd);
    d2EpollLoopTask::setNodelay  (newFd);

    m_ev.events = EPOLLIN | EPOLLET;
    m_ev.data.fd = newFd;

    if (epoll_ctl (m_epollFd, EPOLL_CTL_ADD, newFd, &m_ev) == -1) {
        perror ("epoll_ctl: client fd");
        exit (EXIT_FAILURE);
    }

    d2EpollLoopTask::m_ioArray[newFd].fd = newFd;
    d2EpollLoopTask::m_ioArray[newFd].lasttime = time (NULL);

    d2EpollLoopTask::m_clientCount++;
    return;
}

/*
=====================
  d2EpollLoopTask::handleRecv
=====================
*/
void d2EpollLoopTask::handleRecv (int recvFd)
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
    recv_len = D2SINGLEFACTORY->recvData (recvFd, block->data(), sizeof (int));
    if (recv_len != sizeof (int))
    {
        cout << "[Warning]: Packet header length-" << sizeof (int) \
             << ", actually received length-" << recv_len <<endl;
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
        d2EpollLoopTask::closeFd(recvFd);
        return;
    }

    int *len = (int*)block->data();
    // packet body
    recv_len = D2SINGLEFACTORY->recvData (recvFd, (char*)block->data()+sizeof(int), *len-sizeof(int));
    if ((*len - sizeof (int)) != (unsigned int)recv_len)
    {
        cout << "FD=" << w->fd << ": length=" << *len - sizeof (int) << \
                ", actually recviced length=" << recv_len << endl;
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
        d2EpollLoopTask::closeFd (recvFd);
        return;
    }

    d2EpollLoopTask::m_ioArray[recvFd].lasttime = time (NULL);
    block->setFd (recvFd);
    D2SINGLEFACTORY->m_recvQueue.inQueue (block);
#endif
    return;
}

/*
=====================
  d2EpollLoopTask::handleTimeOut ()
=====================
*/
void d2EpollLoopTask::handleTimeOut ()
{
    time_t now = time (NULL);
    for (register int i=0; i<MAXFD; ++i)
    {
        if (i == -1)
        {
            if (TIMEOUT < now - m_ioArray[i].lasttime)
                d2EpollLoopTask::closeFd (i);
        }
    }
    return;
}

/*
=====================
  d2EpollLoopTask::setNonBlock ()
=====================
*/
void d2EpollLoopTask::setNonBlock (int fd)
{
    fcntl (fd, F_SETFL, fcntl (fd, F_GETFL, 0) | O_NONBLOCK);
}

/*
=====================
  d2EpollLoopTask::setReuseAddr ()
=====================
*/
void d2EpollLoopTask::setReuseAddr(int fd)
{
    int reuse = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse));
}

/*
=====================
  d2EpollLoopTask::setNodelay ()
=====================
*/
void d2EpollLoopTask::setNodelay (int fd)
{
    int nodelay = 1;
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay));
}

/*
=====================
  d2EpollLoopTask::closeFd ()
=====================
*/
void d2EpollLoopTask::closeFd (int fd)
{
    if (fd)
    {
        close (fd);
        d2EpollLoopTask::m_clientCount--;
    }
}

/*
=====================
  d2EpollLoopTask::getClientCount ()
=====================
*/
int d2EpollLoopTask::getClientCount (void)
{
    return d2EpollLoopTask::m_clientCount;
}

/*
=====================
  d2EpollLoopTask::initInetAddr ()
=====================
*/
int d2EpollLoopTask::initInetAddr (void)
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
