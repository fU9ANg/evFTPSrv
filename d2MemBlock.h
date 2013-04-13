
/*
 * d2MemBlock.h
 */

#ifndef _D2_MEMBLOCK_H
#define _D2_MEMBLOCK_H

#include "d2Define.h"

#define MEMBLOCKSIZE    1500

class d2MemBlock
{
    public:

         d2MemBlock (unsigned int iSize = MEMBLOCKSIZE);
        ~d2MemBlock (void);

        int             getFd       (void);
        void            setFd       (int);

        void            setSize     (unsigned int i);
        void            setIndex    (unsigned int i);
        unsigned int    getUsedSize (void);
        unsigned int    getSize     (void);
        unsigned int    getIndex    (void);

        void*           data        (void);
        bool            isUsed      (void);
        void            reSet       (void);

        void            setIsUsed   (bool b);

    private:
        void*           m_memPtr;
        int             m_fd;
        int             m_index;
        unsigned int    m_size;
        unsigned int    m_usedSize;
        bool            m_isUsed;
};

#endif  //_D2_MEMBLOCK_H
