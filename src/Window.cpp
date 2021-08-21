#include "Window.h"

int Window::get_cursor_position(int* rows, int* cols)
{
	char buf[32];
	unsigned int i = 0;
	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
	while (i < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
		if (buf[i] == 'R') break;
		i++;
	}
	buf[i] = '\0';
	if (buf[0] != '\x1b' || buf[1] != '[') return -1;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
	return 0;
}

int Window::get_window_size(int* rows, int* cols)
{
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
	{
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
		return get_cursor_position(rows, cols);
	}
	else
	{
		*cols = ws.ws_col;
		*rows = ws.ws_row;
		return 0;
	}
}

void Window::move_cursor(int key)
{
	erow* row = (RawMode::GET::get_Y() >= RawMode::GET::get_numrows()) ? NULL
																	   : RawMode::GET::get_erow_pos(RawMode::GET::get_Y());

	switch (key)
	{
	case (int)EditorKey::ARROW_LEFT:
		if (RawMode::GET::get_X() != 0)
		{
			RawMode::SET::set_x(RawMode::GET::get_X() - 1);
		} else if(RawMode::GET::get_Y() > 0) {
			RawMode::SET::set_y(RawMode::GET::get_Y() - 1);
			RawMode::SET::set_x(RawMode::GET::get_erow_size(RawMode::GET::get_Y()));
		}
		break;
	case (int)EditorKey::ARROW_RIGHT:
		if (row && RawMode::GET::get_X() < row->size)
		{
			RawMode::SET::set_x(RawMode::GET::get_X() + 1);
		} else if(row && RawMode::GET::get_X() == row->size) {
			RawMode::SET::set_y(RawMode::GET::get_Y() + 1);
			RawMode::SET::set_x(0);
		}
		break;
	case (int)EditorKey::ARROW_UP:
		if (RawMode::GET::get_Y() != 0)
		{
			RawMode::SET::set_y(RawMode::GET::get_Y() - 1);
		}
		break;
	case (int)EditorKey::ARROW_DOWN:
		if (RawMode::GET::get_Y() < RawMode::GET::get_numrows())
		{
			RawMode::SET::set_y(RawMode::GET::get_Y() + 1);
		}
		break;
	}

	row = (RawMode::GET::get_Y() >= RawMode::GET::get_numrows()) ? NULL
																 : RawMode::GET::get_erow_pos(RawMode::GET::get_Y());
	int rowlen = row ? row->size :0;
	if(RawMode::GET::get_X() > rowlen) {
		RawMode::SET::set_x(rowlen);
	}
}