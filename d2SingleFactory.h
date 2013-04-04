
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

class d2SingleFactory
{
    public:
        ~d2SingleFactory ();
        static class d2SingleFactory* instance();

    public:
        D2TASKQUEUE m_recvQueue;
        D2TASKQUEUE m_sendQueue;
        d2MemZone <d2MemBlock > m_memZone;

    private:
        d2SingleFactory ();
        static class d2SingleFactory* m_staticObject;
};

#define D2SINGLEFACTORY d2SingleFactory::instance()

#endif  //_D2_SINGLEFACTORY_H
