
#include "d2Define.h"
#include "d2ThreadPool.h"
#include "d2FTPTask.h"
#include "d2SendTask.h"
#include "d2MutexLock.h"
#include "d2Task.h"
#include "d2EvLoopTask.h"
#include "d2Queue.h"
#include "d2MemZone.h"
#include "d2MemBlock.h"

void sig1 (int sig)
{
    printf ("get exit signal\n");
    raise (SIGKILL);
    return;
}

int main (int argc, char** argv)
{
    signal (SIGINT,  SIG_IGN);
    signal (SIGPIPE, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTERM, sig1);

#if 1
    d2ThreadPool* pool = new d2ThreadPool(3);
    pool->runAll ();

    d2EvLoopTask *    evLoopTask = new d2EvLoopTask ("127.0.0.1", 9999);
    d2FTPTask *       ftpTask    = new d2FTPTask ();
    d2SendTask *      sendTask   = new d2SendTask ();

    pool->joinTask (evLoopTask);
    pool->joinTask (ftpTask);
    pool->joinTask (sendTask);


    pool->reSize ((unsigned int)3);

    while (true)
    {
        cout << "<<<<<<< main loop >>>>>>>" << endl;
        sleep (5);
    }
    pool->killAll ();
#endif

    return (0);
}
