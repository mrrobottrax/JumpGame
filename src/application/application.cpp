#include "pch.h"
#include "window/window.h"
#include "application.h"
#include "graphics/graphics.h"
#include "time/time.h"

void MAGE_Init()
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (!AllocConsole())
	{
		throw std::runtime_error("Failed to create console");
	}

	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stderr);
#endif // DEBUG

	MAGE_CreateWindow();
	MAGE_InitGraphics();
}

void MAGE_End()
{
	MAGE_EndGraphics();

#ifdef DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
	system("pause");
#endif // DEBUG
}

void MAGE_FrameLoop()
{
	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return;
			}
		}

		MAGE_Frame();
	}
}

void MAGE_Frame()
{
	MAGE_UpdateTime();
	MAGE_Render();
}