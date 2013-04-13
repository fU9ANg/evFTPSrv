
/*
 * d2d2SingleFactory.cpp
 */

#include "d2Define.h"
#include "d2SingleFactory.h"

class d2SingleFactory* d2SingleFactory::m_staticObject = NULL;

class d2SingleFactory* d2SingleFactory::instance ()
{
    if (m_staticObject == NULL)
        m_staticObject = new d2SingleFactory ();

    return m_staticObject;
}

d2SingleFactory::d2SingleFactory ()
{
}

d2SingleFactory::~d2SingleFactory ()
{
}

int d2SingleFactory::recvData (int fd, void* buffer, unsigned int len)
{
    if (buffer == NULL)
        return (-1);

    int recv_len = 0;
    int l = 0;
    while (len != 0)
    {
        l = recv (fd, (char*)buffer + recv_len, len, 0);
        if (errno == EINTR)
        {
            usleep (100);
            continue;
        }
        else if (errno == EAGAIN)
        {
            usleep (100);
            continue;
        }

        if (l == 0)
        {
            break;
        }
        else if (l == -1)
        {
            return (-1);
        }

        len -= l;
        recv_len += l;
    }

    return (recv_len);
}

int d2SingleFactory::sendData (int fd, void* buffer, unsigned int len)
{
    if (buffer == NULL)
        return (-1);

    return (0);
}
