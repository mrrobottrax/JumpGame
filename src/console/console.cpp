#include "pch.h"
#include "console.h"

namespace Console
{
	void Log(const char message[], ...)
	{
#ifdef DEBUG
		printf("[LOG] ");

		va_list args;
		va_start(args, message);

		vprintf(message, args);

		va_end(args);

		printf("\n");
#endif // DEBUG
	}

	void Log(const wchar_t message[], ...)
	{
#ifdef DEBUG
		printf("[LOG] ");

		va_list args;
		va_start(args, message);

		vwprintf(message, args);

		va_end(args);

		printf("\n");
#endif // DEBUG
	}

	void LogWarn(const char message[], ...)
	{
#ifdef DEBUG
		printf("[WARNING] ");

		va_list args;
		va_start(args, message);

		vprintf(message, args);

		va_end(args);

		printf("\n");
#endif // DEBUG
	}

	void Log(unsigned int n)
	{
#ifdef DEBUG
		Log("%u", n);
#endif // DEBUG
	}

	void Log(void *p)
	{
#ifdef DEBUG
		Log("%p", p);
#endif // DEBUG
	}

	void Log(float f)
	{
#ifdef DEBUG
		Log("%f", f);
#endif // DEBUG
	}
}