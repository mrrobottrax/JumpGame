#include "pch.h"
#include "window/window.h"
#include "exceptions/exceptions.h"
#include "application/application.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow)
{
	Application::hInstance = hInstance;

	try
	{
		throw_if_failed(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));

		Application::init();
		Application::frame_loop();
		Application::shutdown();

		CoUninitialize();
	}
	catch (WindowsException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(Window::hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (VulkanException &e)
	{
		printf(e.msg().c_str());
		MessageBoxA(Window::hwnd, e.msg().c_str(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (std::exception &e)
	{
		printf("%s\n", e.what());
		MessageBoxA(Window::hwnd, e.what(), NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}
	catch (...)
	{
		printf("Unknown error");
		MessageBoxA(Window::hwnd, "Unknown error", NULL, MB_SYSTEMMODAL);
		system("pause");
		return 1;
	}

	return 0;
}