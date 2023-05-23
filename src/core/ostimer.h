
// See License.md for license

#ifndef OSTIMER_H_
#define OSTIMER_H_

class OSTimer
{
public:
    static int  Create( );
    static void Destroy( );
    static void Update( );
    static long GetTotalTimeMS( );
    
private:
    static timespec Diff( timespec start, timespec end );

    static timespec mZeroTime;
    static long     mTotalTimeMS;
};

#endif
