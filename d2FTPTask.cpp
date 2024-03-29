
/*
 * d2FTPTask.cpp
 */

#include "d2Define.h"
#include "d2FTPTask.h"
#include "d2MemZone.h"
#include "d2MemBlock.h"
#include "d2SingleFactory.h"

D2TASKQUEUE d2FTPTask::m_TaskQueue;

int d2FTPTask::Execute (void* data)
{
    while (1)
    {
        d2MemBlock* block = NULL;

        if (D2SINGLEFACTORY->m_recvQueue.outQueue (block, 2) == false)
        {
            continue;
        }

        cout << "---------------- Task Execute ----------------" << endl;
        cout << "RecvData: " << (char* )((char *)block->data() + sizeof (int)) << endl;
        cout << "RecvLen : " <<  (((MSG_HEAD*) block->data())->iLen) << endl << endl;

#ifdef _ONLY_TEST
        block->reSet ();
        D2SINGLEFACTORY->m_memZone.free (block);
#else
        // TODO:
        // here: add your code please.
        //
#if 0
        d2MemBlock* pblock = D2SINGLEFACTORY->m_memZone.malloc ();
        if (pblock == NULL)
        {
            cout << "d2FTPTask::Execute: out of memory\n";
            return -1;
        }
        pblock->setFd (block->getFd ());
        MSG_HEAD* sendData = (MSG_HEAD*) pblock->data();
        sendData->iLen = sizeof (MSG_HEAD) + 10;
        char      buffer[11] = "0987654321";
        memcpy (sendData->pData(), &buffer, sizeof (buffer));

        pblock->setSize (sendData->iLen);
        D2SINGLEFACTORY->m_sendQueue.inQueue (pblock);
#endif
        usleep (100);
#endif
    }
    return 0;
}
