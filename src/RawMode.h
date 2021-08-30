#ifndef GUMBO_SRC_RAWMODE_H
#define GUMBO_SRC_RAWMODE_H

#include <iostream>
#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <csignal>
#include <curses.h>

#include "Error.h"

typedef struct erow
{
	int idx;
	int size;
	int rsize;
	char* chars;
	char* render;
	unsigned char* hl;
	int hl_open_comment;
} erow;

struct EditorConfig
{
	int cx, cy;
	int rx;
	int rowoff;
	int coloff;
	int screenrows;
	int screencols;
	int num_rows;
	erow* row;
	int dirty;
	char* filename;
	char statusmsg[80];
	time_t statusmsg_time;
	struct EditorSyntax* syntax;
	struct termios orig_termios;
};

extern EditorConfig e;
extern WINDOW *win;

namespace RawMode
{
	void init();
	void enable();
	void disable();
	void editor_set_status_message(const char* fmt, ...);
};

#endif //GUMBO_SRC_RAWMODE_H
