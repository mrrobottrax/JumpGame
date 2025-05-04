#pragma once

namespace Window
{
	constexpr unsigned int TILE_SIZE = 8;
	constexpr unsigned int LEVEL_WIDTH = 40;
	constexpr unsigned int LEVEL_HEIGHT = 27;

	// 320x216
	constexpr unsigned int SCREEN_WIDTH = LEVEL_WIDTH * TILE_SIZE;
	constexpr unsigned int SCREEN_HEIGHT = LEVEL_HEIGHT * TILE_SIZE;

	extern HWND hwnd;

	inline struct WindowData
	{
		unsigned int width;
		unsigned int height;
		bool fullscreen;
	} g_windowData;

	void create_window();
	void toggle_fullscreen();
	void set_fullscreen(bool val);
}