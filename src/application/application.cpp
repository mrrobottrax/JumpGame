#include "pch.h"
#include "window/window.h"
#include "application.h"
#include "graphics/graphics.h"
#include "game/game.h"
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

	FreeConsole();
#endif // DEBUG
}

static chrono::system_clock::time_point prevFrame = chrono::system_clock::now();
void MAGE_FrameLoop()
{
	MSG msg;
	while (true)
	{
		chrono::system_clock::time_point currentTime = chrono::system_clock::now();

		bool ranCatchupFrame = false;
		bool shouldRender = false;
		constexpr chrono::system_clock::duration delta((int)(10000000.0 / ticksPerSecond));
		while (currentTime - prevFrame > delta)
		{
			prevFrame += delta;

			if (!ranCatchupFrame && currentTime - prevFrame > delta)
			{
#ifndef DEBUG
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);

					if (msg.message == WM_QUIT)
					{
						return;
					}
				}

				Game_Tick();
				ranCatchupFrame = true; // < 30fps = TOO BAD!
#endif // !DEBUG
			}

			shouldRender = true;
		}

		if (shouldRender)
		{
			MAGE_WaitForNextFrame();

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
				{
					return;
				}
			}

			Game_Tick();
			MAGE_Render();
		}
	}
}