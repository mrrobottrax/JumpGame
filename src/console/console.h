#pragma once

namespace Console
{
	void log(const char message[], ...);
	void log(const wchar_t message[], ...);
	void log_warn(const char message[], ...);

	void log(unsigned int n);
	void log(void *p);
	void log(float f);
}