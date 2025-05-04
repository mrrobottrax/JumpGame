#pragma once

namespace Console
{
	void Log(const char message[], ...);
	void Log(const wchar_t message[], ...);
	void LogWarn(const char message[], ...);

	void Log(unsigned int n);
	void Log(void *p);
	void Log(float f);
}