//
// Created by filip on 19. 08. 2021..
//

#ifndef EDITOR_SRC_RAWMODE_H
#define EDITOR_SRC_RAWMODE_H

#include <iostream>
#include <termios.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

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
	struct EditorSyntax *syntax;
	struct termios orig_termios;
};

extern EditorConfig e;

namespace RawMode
{
	void init();
	void enable();
	void disable();

	void editor_set_status_message(const char* fmt, ...);

	int get_screen_rows();
	int get_screen_cols();

	void free_filename();

	namespace GET
	{
		int get_X();
		int get_Y();
		char* get_erow_chars(int at);
		char* get_erow_chars_pos(int at, int pos);
		int get_erow_size(int at);
		int get_numrows();
		int get_rowoff();
		erow* get_erow();
		erow* get_erow_pos(int pos);
		int get_coloff();
		int get_rsize_pos(int pos);
		char* get_render_pos(int pos);
		char* get_render_pos_pos(int pos, int render_pos);
		int get_rx();
		int get_dirty();
		char* get_filename();
		char* get_statusmsg();
		time_t get_time();
	};

	namespace SET
	{
		void set_x(int x);
		void set_y(int y);
		void set_erow_size(ssize_t linelen, int at);
		void set_erow_chars(ssize_t linelen, char* data, int at);
		void set_last_erow_char(ssize_t linelen, int at);
		void set_num_rows(int num);
		void set_erow();
		void set_rowoff(int num);
		void set_coloff(int num);
		void set_rsize_pos(int pos, int data);
		void set_render_pos(int pos, char* data);
		void set_rx(int data);
		void set_filename(char* data);
		void set_dirty(int data);
	};
};

#endif //EDITOR_SRC_RAWMODE_H
