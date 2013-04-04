
#include "d2Define.h"
#include "d2ThreadPool.h"
#include "d2FTPTask.h"
#include "d2SendTask.h"
//#include "d2HTTPTask.h"
//#include "d2CaptureTask.h"
//#include "d2DispatchTask.h"
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
    d2ThreadPool* threadPool = new d2ThreadPool(100);
    threadPool->runAll ();
    /*
    d2DispatchTask * dispatchTask = new d2DispatchTask ();
    d2CaptureTask *  captureTask  = new d2CaptureTask ();
    d2HTTPTask *     httpTask = new d2HTTPTask ();
    */

    d2EvLoopTask *   evLoopTask = new d2EvLoopTask ("127.0.0.1", 2300);
    d2FTPTask *      ftpTask    = new d2FTPTask ();
    d2SendTask *      sendTask    = new d2SendTask ();
    //threadPool->joinTask (dispatchTask);
    //threadPool->joinTask (captureTask);
    //threadPool->joinTask (ftpTask);
    //threadPool->joinTask (ftpTask);
    //threadPool->joinTask (httpTask);
    threadPool->joinTask (evLoopTask);
    threadPool->joinTask (ftpTask);
    threadPool->joinTask (sendTask);


    threadPool->reSize ((unsigned int)3);

    while (true)
    {
        cout << "<<<<<<< main loop >>>>>>>" << endl;
        sleep (3);
    }
    threadPool->killAll ();
#endif
    return (0);
}
