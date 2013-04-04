
/*
 * d2_task.h
 */

#ifndef _D2_TASK_H
#define _D2_TASK_H

#include "d2Define.h"
#include "d2Queue.h"
#include "d2MemBlock.h"

typedef d2Queue <d2MemBlock*> D2TASKQUEUE;

class d2Task
{
    public:
        //d2Task (bool isExit) : m_AutoExit (isExit)
        d2Task () : m_AutoExit (true)
        {
        }
        virtual ~d2Task () {};
        virtual int Execute (void* data) = 0;
        bool isAutoExit (void)
        {
            return m_AutoExit;
        }
    private:
        bool m_AutoExit;
        
};

#endif  //_D2_TASK_H
