#pragma once

constexpr unsigned int SCREEN_WIDTH = 8 * 40;
constexpr unsigned int SCREEN_HEIGHT = 8 * 27;

void MAGE_CreateWindow();
void MAGE_ToggleFullscreen();
void MAGE_SetFullscreen(bool val);

extern HWND hwnd;

inline struct WindowData
{
	unsigned int width;
	unsigned int height;
	bool fullscreen;
} g_WindowData;