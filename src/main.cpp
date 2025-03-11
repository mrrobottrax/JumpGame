#include "pch.h"
#include "window/window.h"
#include "exceptions/exceptions.h"
#include "application/application.h"
#include <console/console.h>

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow)
{
	::hInstance = hInstance;

	try
	{
		MAGE_Init();
		MAGE_FrameLoop();
		MAGE_End();
	}
	catch (WindowsException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (VulkanException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		MessageBoxA(hwnd, e.what(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (...)
	{
		printf("Unknown error");
		MessageBoxA(hwnd, "Unknown error", NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}

	return 0;
}