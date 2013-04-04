
/*
 * d2SendTask.cpp
 */

#include "d2SendTask.h"
#include "d2MemZone.h"
#include "d2MemBlock.h"
#include "d2SingleFactory.h"

int d2SendTask::Execute (void*)
{
    while (true)
    {
        d2MemBlock* block;
        if (D2SINGLEFACTORY->m_sendQueue.outQueue (block, 2) == false) {
            continue;
        }

        int fd = block->getFd ();
        send (fd, block->data(), block->getSize(), 0);
        cout << "FD=" << fd << " -- send success" << endl;

        block->reSet();
        D2SINGLEFACTORY->m_memZone.free (block);
    }

    return 0;
}
