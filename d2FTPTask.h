
/*
 * d2FTPTask.h
 */

#ifndef	_D2_FTPTASK_H
#define	_D2_FTPTASK_H

#include "d2Define.h"
#include "d2Task.h"

class d2FTPTask : public d2Task
{
    public:
        //d2FTPTask (bool exit) : d2Task (exit)
        d2FTPTask ()
        {
        }

        ~d2FTPTask () {};
        virtual int Execute (void* data);

    public:
        static D2TASKQUEUE m_TaskQueue;
};

#endif	//_D2_FTPTASK_H
