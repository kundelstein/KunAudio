#pragma once

#include "engine_config.h"


#if (DEBUG_MESSAGES)

#include <string>
#include <sstream>
#include <type_traits>


/*
// @NOTE: If your time has no value and you don't care about compilation time, use the template.
// @TODO: Correct the additional character at the end of each displayed value (%s adds 's' at the end of given string).
// @TODO: Trucates floats to unreadable values. Don't blame me, I got it from some 'pro' expert from the web.


void OutputLine (const std::string& buffer);


inline void xsprintf (std::string& result, const char *s)
{
	while (*s)
	{
		if (*s == '%')
		{
			if (*(s + 1) == '%')
			{
				++s;
			}
			else
			{
				throw std::runtime_error ("invalid format string: missing arguments");
			}
		}
		result += *s++;
	}
}


template<typename T, typename... Args>
inline void xsprintf (std::string& result, const char *s, T value, Args... args)
{
	while (*s)
	{
		if (*s == '%')
		{
			if (*(s + 1) == '%')
			{
				++s;
			}
			else
			{
				std::stringstream stream;
				stream << value;
				result += stream.str ();
				xsprintf (result, s + 1, args...); // call even when *s == 0 to detect extra arguments
				return;
			}
		}
		result += *s++;
	}
	throw std::logic_error ("extra arguments provided to printf");
}


template<typename... Args>
void DebugPrint (const char *s, Args... args)
{
	std::string result = "";
	xsprintf (result, s, args...);

	OutputLine (result);
}
*/


void DebugPrint (const char* fmt, ...);


#define DBG_MSG(...)		DebugPrint ( __VA_ARGS__)


#define DBG_ERRORMSG(...)\
do\
{\
	DebugPrint ( "** %s:%i ** ERROR:  ", __FILE__, __LINE__);\
	DebugPrint ( __VA_ARGS__);\
	DebugPrint ( "\n");\
	while (1){};\
}\
while (0)


#define DBG_ASSERTMSG(a, ...)\
do\
{\
	if (!(a)) DBG_ERRORMSG( __VA_ARGS__ );\
}\
while (0)


#define DBG_WARNINGMSG(a, ...)\
do\
{\
	if (!(a)) DBG_MSG( __VA_ARGS__ );\
}\
while (0)


#define DBG_ASSERT(a)\
do\
{\
	if (!(a)) DBG_ERRORMSG("DBG ASSERTION FAILED!");\
}\
while (0)


#define DBG_STATICASSERT(a)\
do\
{\
	static_assert (a, "STATIC ASSERT FAILED");\
}\
while (0)


//static_assert (a, "** %s:%i ** STATIC ASSERT FAILED:  ", __FILE__, __LINE__); \


#define DBG_UNUSED(var)		((void)&var);


#else

// Dummy debug.
#define DBG_MSG(  ... )
#define DBG_ERRORMSG(  ... )
#define DBG_ASSERTMSG( ... )
#define DBG_WARNINGMSG(  ... )
#define DBG_ASSERT(a)
#define DBG_STATICASSERT(a)
#define DBG_UNUSED(var)

#endif

