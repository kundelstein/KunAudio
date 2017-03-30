#include "debug.h"


#if (DEBUG_MESSAGES)

#if (SYSTEM_WINDOWS)
#include <windows.h>
#else
#import <UIKit/UIKit.h>
#endif


namespace
{
	int debug_cnt = 0;

	const int DEBUG_BUFFER_LENGTH = 1024 * 4;
	char buffer[DEBUG_BUFFER_LENGTH];
	char number[8];
}


void DebugPrint (const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	vsnprintf (buffer, DEBUG_BUFFER_LENGTH - 1, fmt, ap);
	va_end (ap);


#if (SYSTEM_WINDOWS)
	sprintf_s (number, "%06d\t", debug_cnt++);
	OutputDebugStringA (number);
	OutputDebugStringA (buffer);
	OutputDebugStringA ("\n");
#else
	NSLog (@"%@", @(buffer));
#endif
}


#endif

