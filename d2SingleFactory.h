
/*
 * d2SingleFactory.h
 */

#ifndef _D2_SINGLEFACTORY
#define _D2_SINGLEFACTORY

#include <stdio.h>
#include "d2Task.h"
#include "d2Queue.h"
#include "d2MemZone.h"
#include "d2MemBlock.h"

// only instance (global)
class d2SingleFactory
{
    public:
        ~d2SingleFactory ();
        static class d2SingleFactory* instance();

    public:
        // receive queue (evLoop ----> FTPTask)
        D2TASKQUEUE m_recvQueue;
        // send queue (FTPTask ----> SendTask)
        D2TASKQUEUE m_sendQueue;
        // memory-manager
        d2MemZone <d2MemBlock > m_memZone;

        // receive data handler
        static int recvData (int fd, void* buffer, unsigned int len);
        // send data handler
        static int sendData (int fd, void* buffer, unsigned int len);

    private:
        d2SingleFactory ();
        // single pattern
        static class d2SingleFactory* m_staticObject;
};

#define D2SINGLEFACTORY d2SingleFactory::instance()

#endif  //_D2_SINGLEFACTORY_H
