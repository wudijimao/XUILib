//
//  ThreardSafeQueue.cpp
//  XDUILib
//
//  Created by ximiao on 16/6/6.
//  Copyright © 2016年 wudijimao. All rights reserved.
//

#include "ThreardSafeQueue.hpp"


SingleRWSafeQueue::SingleRWSafeQueue(int nSize)
:m_pBuffer(NULL)
,m_nSize(nSize)
,m_nIn(0)
,m_nOut(0)
{
    //round up to the next power of 2
    if (!is_power_of_2(nSize))
    {
        m_nSize = roundup_power_of_two(nSize);
    }
}

SingleRWSafeQueue::~SingleRWSafeQueue()
{
    if(NULL != m_pBuffer)
    {
        delete[] m_pBuffer;
        m_pBuffer = NULL;
    }
}

bool SingleRWSafeQueue::Initialize()
{
    m_pBuffer = new unsigned char[m_nSize];
    if (!m_pBuffer)
    {
        return false;
    }
    
    m_nIn = m_nOut = 0;
    
    return true;
}

unsigned long SingleRWSafeQueue::roundup_power_of_two(unsigned long val)
{
    if((val & (val-1)) == 0)
        return val;
    
    unsigned long maxulong = (unsigned long)((unsigned long)~0);
    unsigned long andv = ~(maxulong&(maxulong>>1));
    while((andv & val) == 0)
        andv = andv>>1;
    
    return andv<<1;
}



unsigned int SingleRWSafeQueue::Put(const unsigned char *buffer, unsigned int len)
{
    unsigned int l;
    
    len = std::min(len, m_nSize - m_nIn + m_nOut);
    
    /*
     * Ensure that we sample the m_nOut index -before- we
     * start putting bytes into the UnlockQueue.
     */
    __sync_synchronize();
    
    /* first put the data starting from fifo->in to buffer end */
    l = std::min(len, m_nSize - (m_nIn  & (m_nSize - 1)));
    memcpy(m_pBuffer + (m_nIn & (m_nSize - 1)), buffer, l);
    
    /* then put the rest (if any) at the beginning of the buffer */
    memcpy(m_pBuffer, buffer + l, len - l);
    
    /*
     * Ensure that we add the bytes to the kfifo -before-
     * we update the fifo->in index.
     */
    __sync_synchronize();
    
    m_nIn += len;
    
    return len;
}

unsigned int SingleRWSafeQueue::Get(unsigned char *buffer, unsigned int len)
{
    unsigned int l;
    
    len = std::min(len, m_nIn - m_nOut);
    
    /*
     * Ensure that we sample the fifo->in index -before- we
     * start removing bytes from the kfifo.
     */
    __sync_synchronize();
    
    /* first get the data from fifo->out until the end of the buffer */
    l = std::min(len, m_nSize - (m_nOut & (m_nSize - 1)));
    memcpy(buffer, m_pBuffer + (m_nOut & (m_nSize - 1)), l);
    
    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + l, m_pBuffer, len - l);
    
    /*
     * Ensure that we remove the bytes from the kfifo -before-
     * we update the fifo->out index.
     */
    __sync_synchronize();
    
    m_nOut += len;
    
    return len;
}