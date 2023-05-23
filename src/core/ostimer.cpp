
// See License.md for license

#include "includes.h"

timespec OSTimer::mZeroTime;
long     OSTimer::mTotalTimeMS;
    
int OSTimer::Create( )
{   
    clock_gettime( CLOCK_MONOTONIC, &mZeroTime );
    
    mTotalTimeMS = 0;
    
    return 0;
}

void OSTimer::Destroy( )
{
}

void OSTimer::Update( )
{
    timespec currTime;
    clock_gettime( CLOCK_MONOTONIC, &currTime );
    
    timespec delta = Diff( mZeroTime, currTime );
    
    mTotalTimeMS = delta.tv_sec * SEC_TO_MILLISECONDS + (delta.tv_nsec / NANO_TO_MILLISECONDS);
}

long OSTimer::GetTotalTimeMS( )
{
    return mTotalTimeMS;
}

timespec OSTimer::Diff( timespec start, timespec end )
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } 
    else 
    {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}
