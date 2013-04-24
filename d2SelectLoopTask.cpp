
/*
 * d2SelectLoopTask.cpp
 */

#include "d2Define.h"
#include "d2Task.h"
#include "d2SelectLoopTask.h"

struct ev_io_info d2SelectLoopTask::m_ioArray[MAXFD];
D2TASKQUEUE d2SelectLoopTask::m_TaskQueue;
int d2SelectLoopTask::m_clientCount = 0;

/*
=====================
  d2SelectLoopTask::d2SelectLoopTask ()
=====================
*/
d2SelectLoopTask::d2SelectLoopTask (string ip, int port)
{
    m_Ip   = ip;
    m_Port = port;

    if ((m_listenFd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "ERROR: in socket ()\n";
        return;
    }

    d2SelectLoopTask::setNonBlock (m_listenFd);
    d2SelectLoopTask::setNodelay  (m_listenFd);
    d2SelectLoopTask::setReuseAddr(m_listenFd);

    for (int i=0; i<MAXFD; ++i)
    {
        d2SelectLoopTask::m_ioArray[i].fd = -1;
    }
}

/*
=====================
  d2SelectLoopTask::~d2SelectLoopTask ()
=====================
*/
d2SelectLoopTask::~d2SelectLoopTask ()
{
    for (register int i=0; i<MAXFD; i++)
    {
        d2SelectLoopTask::closeFd (i);
    }

    close (m_listenFd);
}

/*
=====================
  d2SelectLoopTask::InIoArray()
=====================
*/
int d2SelectLoopTask::InIoArray (int fd)
{
    return (0);
}

/*
=====================
  d2SelectLoopTask::Execute ()
=====================
*/
int d2SelectLoopTask::Execute (void* data)
{
    fd_set readfds, testfds;
    int    result, connfd, fd;
    // initialize read fds array and add server-fd to that..
    FD_ZERO (&readfds);
    FD_SET (m_listenFd, &readfds);

    // loop and accept request from clients
    while (1) {
        testfds = readfds;

        printf ("server waiting...\n");
        result = select (FD_SETSIZE, &testfds, (fd_set*)NULL, (fd_set*)NULL, (struct timeval*)0);
        if (result < 1) {
            perror ("evFTPSrv: select");
            return (1);
        }

        // check fd, if already then goto accpet request from client
        for (fd=0; fd < FD_SETSIZE; fd++)
        {
            if (FD_ISSET (fd, &testfds))
            {
                // is listen-fd
                if (fd == m_listenFd)
                {
                    connfd = handleAccept (m_listenFd);
                    FD_SET (connfd, &readfds);
                }
                // is client-fd
                else if (InIoArray (fd))
                {
                    if (handleRecv (fd) == 1)
                    {
                        FD_CLR (fd, &readfds);
                    }
                }
            }
        }
    }
}

/*
=====================
  d2SelectLoopTask::handleAccept ()
=====================
*/
int d2SelectLoopTask::handleAccept (int listenFd)
{
    struct sockaddr_in clientAddr;
    socklen_t sockLen = 0;
    int newFd = 0;

    newFd = accept (listenFd, (struct sockaddr*)&clientAddr, &sockLen);
    if (newFd < 0)
    {
        cout << "ERROR: in accept ()\n";
        abort ();
        return (-1);
    }
    d2SelectLoopTask::setNonBlock (newFd);
    d2SelectLoopTask::setReuseAddr(newFd);
    d2SelectLoopTask::setNodelay  (newFd);

    d2SelectLoopTask::m_ioArray[newFd].fd = newFd;
    d2SelectLoopTask::m_ioArray[newFd].lasttime = time (NULL);

    d2SelectLoopTask::m_clientCount++;
    return (newFd);
}

/*
=====================
  d2SelectLoopTask::handleRecv
=====================
*/
int d2SelectLoopTask::handleRecv (int recvFd)
{
    d2MemBlock* block = D2SINGLEFACTORY->m_memZone.malloc ();
    if (block == NULL)
    {
        cout << "MALLOC-ERROR: out of memory" << endl;
        return (-1);
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
        d2SelectLoopTask::closeFd(recvFd);
        return (-1);
    }

    int *len = (int*)block->data();
    // packet body
    recv_len = D2SINGLEFACTORY->recvData (recvFd, (char*)block->data()+sizeof(int), *len-sizeof(int));
    if ((*len - sizeof (int)) != (unsigned int)recv_len)
    {
        cout << "FD=" << recvFd << ": length=" << *len - sizeof (int) << \
                ", actually recviced length=" << recv_len << endl;
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
        d2SelectLoopTask::closeFd (recvFd);
        return (-1);
    }

    d2SelectLoopTask::m_ioArray[recvFd].lasttime = time (NULL);
    block->setFd (recvFd);
    D2SINGLEFACTORY->m_recvQueue.inQueue (block);
#endif
    return (1);
}

/*
=====================
  d2SelectLoopTask::handleTimeOut ()
=====================
*/
void d2SelectLoopTask::handleTimeOut (int i)
{
    time_t now = time (NULL);
    for (register int i=0; i<MAXFD; ++i)
    {
        if (i == -1)
        {
            if (TIMEOUT < now - m_ioArray[i].lasttime)
                d2SelectLoopTask::closeFd (i);
        }
    }
    return;
}

/*
=====================
  d2SelectLoopTask::setNonBlock ()
=====================
*/
void d2SelectLoopTask::setNonBlock (int fd)
{
    fcntl (fd, F_SETFL, fcntl (fd, F_GETFL, 0) | O_NONBLOCK);
}

/*
=====================
  d2SelectLoopTask::setReuseAddr ()
=====================
*/
void d2SelectLoopTask::setReuseAddr(int fd)
{
    int reuse = 1;
    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (reuse));
}

/*
=====================
  d2SelectLoopTask::setNodelay ()
=====================
*/
void d2SelectLoopTask::setNodelay (int fd)
{
    int nodelay = 1;
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof (nodelay));
}

/*
=====================
  d2SelectLoopTask::closeFd ()
=====================
*/
void d2SelectLoopTask::closeFd (int fd)
{
    if (fd)
    {
        close (fd);
        d2SelectLoopTask::m_clientCount--;
    }
}

/*
=====================
  d2SelectLoopTask::getClientCount ()
=====================
*/
int d2SelectLoopTask::getClientCount (void)
{
    return d2SelectLoopTask::m_clientCount;
}

/*
=====================
  d2SelectLoopTask::initInetAddr ()
=====================
*/
int d2SelectLoopTask::initInetAddr (void)
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
