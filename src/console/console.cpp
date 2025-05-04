#include "pch.h"
#include "console.h"

namespace Console
{
	void log(const char message[], ...)
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

	void log(const wchar_t message[], ...)
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

	void log_warn(const char message[], ...)
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

	void log(unsigned int n)
	{
#ifdef DEBUG
		log("%u", n);
#endif // DEBUG
	}

	void log(void *p)
	{
#ifdef DEBUG
		log("%p", p);
#endif // DEBUG
	}

	void log(float f)
	{
#ifdef DEBUG
		log("%f", f);
#endif // DEBUG
	}
}