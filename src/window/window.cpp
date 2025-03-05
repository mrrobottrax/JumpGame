#include "pch.h"
#include "window.h"
#include "input/input.h"

extern HINSTANCE hInstance;

HWND hwnd;
HCURSOR cursor;
HICON icon;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void FullscreenEnable();

void MAGE_CreateWindow()
{
	cursor = (HCURSOR)LoadImage(NULL,
								IDC_ARROW,
								IMAGE_CURSOR,
								0,
								0,
								LR_DEFAULTSIZE | LR_SHARED);

	/*icon = (HICON)LoadImage(hInstance,
							MAKEINTRESOURCE(IDI_ICON1),
							IMAGE_ICON,
							0,
							0,
							LR_DEFAULTSIZE | LR_SHARED);*/

	const wchar_t CLASS_NAME[] = L"Mage Window";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hIcon = icon;

	RegisterClass(&wc);

	// Get size
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	hwnd = CreateWindow(
		CLASS_NAME,
		L"JUMPMAN!",

		WS_SYSMENU | WS_POPUP,

		0,
		0,
		screenWidth,
		screenHeight,

		NULL,
		NULL,

		hInstance,

		NULL
	);

	if (hwnd == NULL)
	{
		throw std::runtime_error("Failed to create window");
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	MAGE_SetFullscreen(g_WindowData.fullscreen);

	//ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}

void MAGE_ToggleFullscreen()
{
	MAGE_SetFullscreen(!g_WindowData.fullscreen);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool resizing = false;

	switch (uMsg)
	{
		case WM_CLOSE:
			DestroyWindow(::hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			g_WindowData.width = LOWORD(lParam);
			g_WindowData.height = HIWORD(lParam);
			break;

		case WM_KEYDOWN:
			Input_Keydown(wParam);
			break;

		case WM_KEYUP:
			Input_Keyup(wParam);
			break;

		case WM_ENTERSIZEMOVE:
			resizing = true;
			break;

		case WM_EXITSIZEMOVE:
			resizing = false;
			break;

		case WM_PAINT:
			if (resizing)
			{
				// Continue rendering while resizing
				//MAGE_Frame();
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);

		case WM_SETCURSOR:
			if (LOWORD(lParam) == HTCLIENT)
			{
				if (g_WindowData.fullscreen)
				{
					SetCursor(NULL);
				}
				else
				{
					SetCursor(cursor);
				}
			}
			else
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			break;

		case WM_GETMINMAXINFO:
		{
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

			RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			lpMMI->ptMinTrackSize.x = rc.right - rc.left;
			lpMMI->ptMinTrackSize.y = rc.bottom - rc.top;
			break;
		}

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}


static void FullscreenEnable()
{
	g_WindowData.fullscreen = true;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);
}

static void FullscreenDisable()
{
	g_WindowData.fullscreen = false;

	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);

	int w = SCREEN_WIDTH * 3;
	int h = SCREEN_HEIGHT * 3;

	if (w > sw) w = sw;
	if (h > sh) h = sh;

	SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);

	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

	RECT rc = { 0, 0, w, h };
	AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

	w = rc.right - rc.left;
	h = rc.bottom - rc.top;

	unsigned int x = (sw - w) / 2;
	unsigned int y = (sh - h) / 2;

	SetWindowPos(hwnd, HWND_TOP, x, y, w, h, SWP_FRAMECHANGED);
}

void MAGE_SetFullscreen(bool val)
{
	if (val)
	{
		FullscreenEnable();
	}
	else
	{
		FullscreenDisable();
	}
}