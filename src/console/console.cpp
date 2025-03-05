#include "pch.h"
#include "console.h"

void Log(unsigned int n)
{
#ifdef DEBUG
	printf("[LOG] %u\n", n);
#endif // DEBUG
}

void Log(void *p)
{
#ifdef DEBUG
	printf("[LOG] %p\n", p);
#endif // DEBUG
}

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
