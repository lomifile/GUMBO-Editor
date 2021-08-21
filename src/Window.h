//
// Created by filip on 19. 08. 2021..
//

#ifndef EDITOR_SRC_WINDOW_H
#define EDITOR_SRC_WINDOW_H

#include <unistd.h>
#include <sys/ioctl.h>
#include <cstdio>

#include "RawMode.h"
#include "IO.h"

namespace Window
{
	int get_window_size(int* rows, int* cols);
	int get_cursor_position(int* rows, int *cols);
	void move_cursor(int key);
}

#endif //EDITOR_SRC_WINDOW_H
