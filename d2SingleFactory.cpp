
/*
 * d2d2SingleFactory.cpp
 */

#include "d2Define.h"
#include "d2SingleFactory.h"

class d2SingleFactory* d2SingleFactory::m_staticObject = NULL;

class d2SingleFactory* d2SingleFactory::instance ()
{
    if (m_staticObject == NULL)
        m_staticObject = new d2SingleFactory ();

    return m_staticObject;
}

d2SingleFactory::d2SingleFactory ()
{
}

d2SingleFactory::~d2SingleFactory ()
{
}
