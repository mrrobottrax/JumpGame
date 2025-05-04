#include "pch.h"
#include "input.h"
#include "window/window.h"

static void set_key(unsigned long long key, bool value);

void input_keydown(unsigned long long key)
{
	set_key(key, true);
}

void input_keyup(unsigned long long key)
{
	set_key(key, false);
}

static void set_key(unsigned long long key, bool value)
{
	switch (key)
	{
		case 'W':
			key_w = value;
			break;

		case 'S':
			key_s = value;
			break;

		case 'A':
			key_a = value;
			break;

		case 'D':
			key_d = value;
			break;

		case 'R':
			if (key_ctrl && value && !key_r)
			{
				//ReloadGameDLL();
			}
			key_r = value;
			break;

		case VK_CONTROL:
			key_ctrl = value;
			break;

		case VK_UP:
			key_up = value;
			break;

		case VK_DOWN:
			key_down = value;
			break;

		case VK_SPACE:
			key_space = value;
			break;

		case VK_ESCAPE:
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;

		case VK_F11:
			if (value)
				MAGE_ToggleFullscreen();
			break;
	}
}