
/*
 * d2MemBlock.cpp
 */

#include "d2MemBlock.h"

/*
=====================
  d2MemBlock::d2MemBlock ()
=====================
*/
d2MemBlock::d2MemBlock (unsigned int iSize)
{
    this->m_size = iSize;
    m_memPtr = malloc (iSize);
}

/*
=====================
  d2MemBlock::~d2MemBlock ()
=====================
*/
d2MemBlock::~d2MemBlock (void)
{
    if (m_memPtr) {
        free (m_memPtr);
        m_memPtr = NULL;
    }
}

/*
=====================
  d2MemBlock::setSize ()
=====================
*/
void d2MemBlock::setSize (unsigned int i)
{
    m_size = i;
}

/*
=====================
  d2MemBlock::getSize ()
=====================
*/
unsigned int d2MemBlock::getSize (void)
{
    return m_size;
}

/*
=====================
  d2MemBlock::getUsedSize ()
=====================
*/
unsigned int d2MemBlock::getUsedSize (void)
{
    return m_usedSize;
}

/*
=====================
  d2MemBlock::reSet ()
=====================
*/
void d2MemBlock::reSet (void)
{
    (void) memset (m_memPtr, 0x00, m_size);
}

/*
=====================
  d2MemBlock::data ()
=====================
*/
void* d2MemBlock::data (void)
{
    return m_memPtr;
}

/*
=====================
  d2MemBlock::getIndex ()
=====================
*/
unsigned int d2MemBlock::getIndex (void)
{
    return m_index;
}

/*
=====================
  d2MemBlock::isUsed ()
=====================
*/
bool d2MemBlock::isUsed (void)
{
    return m_isUsed;
}

/*
=====================
  d2MemBlock::setIndex ()
=====================
*/
void d2MemBlock::setIndex (unsigned int i)
{
    m_index = i;
}

/*
=====================
  d2MemBlock::setIsUsed ()
=====================
*/
void d2MemBlock::setIsUsed (bool b)
{
    m_isUsed = b;
}

/*
=====================
  d2MemBlock::getFd ()
=====================
*/
int d2MemBlock::getFd (void)
{
    return m_fd;
}

/*
=====================
  d2MemBlock::setFd ()
=====================
*/
void d2MemBlock::setFd(int fd)
{
    this->m_fd = fd; 
}
