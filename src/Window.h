#ifndef GUMBO_SRC_WINDOW_H
#define GUMBO_SRC_WINDOW_H

#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>

#include "RawMode.h"
#include "IO.h"

extern EditorConfig e;

namespace Window
{
	int get_window_size(int* rows, int* cols);
	int get_cursor_position(int* rows, int *cols);
	void move_cursor(int key);
	void update_screen();
	void handle_change(int unused __attribute__((unused)));
}

#endif //GUMBO_SRC_WINDOW_H
