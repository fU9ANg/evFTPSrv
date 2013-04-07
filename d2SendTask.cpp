
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

        unsigned int written_bytes;
        unsigned int left_bytes = block->getSize();
        char* ptr = (char*) block->data();

        while (left_bytes > 0)
        {
            written_bytes = send (fd, ptr, left_bytes, 0);
            if (errno == EINTR)
            {
                if (written_bytes < 0) written_bytes = 0;
                continue;
            }
            if (errno == EAGAIN)
            {
                if (written_bytes < 0) written_bytes = 0;
                usleep (200);
                continue;
            }
            else
            {
                break;
            }
            ptr += written_bytes;
            left_bytes -= written_bytes;
        }

        cout << "FD=" << fd << " -- send success" << endl;

        block->reSet();
        D2SINGLEFACTORY->m_memZone.free (block);
    }

    return 0;
}
