
// See License.md for license

#include "includes.h"

void flushPrintf( const char *pStr, ... )
{
	va_list argptr;
    va_start( argptr, pStr );
	vprintf( pStr, argptr );
	fflush(stdout);
	va_end( argptr );
}
