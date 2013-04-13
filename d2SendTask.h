

/*
 * d2FTPTask.h
 */

#ifndef	_D2_SENDTASK_H
#define	_D2_SENDTASK_H

#include "d2Define.h"
#include "d2Task.h"

class d2SendTask : public d2Task
{
    public:
        //d2SendTask (bool exit) : d2Task (exit)
        d2SendTask ()
        {
        }

        ~d2SendTask () {};
        virtual int Execute (void* data);

    public:
        static D2TASKQUEUE m_TaskQueue;
};

#endif	//_D2_SENDTASK_H
