#include "RawMode.h"
#include "Window.h"

EditorConfig e;

void RawMode::init()
{
	e.cx = 0;
	e.cy = 0;
	e.rx = 0;
	e.num_rows = 0;
	e.row = NULL;
	e.dirty = 0;
	e.rowoff = 0;
	e.coloff = 0;
	e.filename = NULL;
	e.statusmsg[0] = '\0';
	e.statusmsg_time = 0;
	e.syntax = NULL;
	if (Window::get_window_size(&e.screenrows, &e.screencols) == -1) Error::die("getWindowSize");
	e.screenrows -= 2;
}

void RawMode::disable()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &e.orig_termios) == -1)
	{
		Error::die("tcsetattr");
	}
}

void RawMode::enable()
{
	if (tcgetattr(STDIN_FILENO, &e.orig_termios) == -1) Error::die("tcgetattr");
	atexit(disable);

	struct termios raw = e.orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) Error::die("tcsetattr");
}

void RawMode::editor_set_status_message(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(e.statusmsg, sizeof(e.statusmsg), fmt, ap);
	va_end(ap);
	e.statusmsg_time = time(NULL);
}

void RawMode::free_filename()
{
	free(e.filename);
}

int RawMode::get_screen_rows()
{
	return e.screenrows;
}

int RawMode::get_screen_cols()
{
	return e.screencols;
}

int RawMode::GET::get_X()
{
	return e.cx;
}

int RawMode::GET::get_Y()
{
	return e.cy;
}

char* RawMode::GET::get_erow_chars(int at)
{
	return e.row[at].chars;
}

int RawMode::GET::get_erow_size(int at)
{
	return e.row[at].size;
}

int RawMode::GET::get_numrows()
{
	return e.num_rows;
}

int RawMode::GET::get_rowoff()
{
	return e.rowoff;
}

erow* RawMode::GET::get_erow()
{
	return e.row;
}

erow* RawMode::GET::get_erow_pos(int pos)
{
	return &e.row[pos];
}

int RawMode::GET::get_coloff()
{
	return e.coloff;
}

char* RawMode::GET::get_erow_chars_pos(int at, int pos)
{
	return &e.row[at].chars[pos];
}

char* RawMode::GET::get_filename()
{
	return e.filename;
}

int RawMode::GET::get_rsize_pos(int pos)
{
	return e.row[pos].rsize;
}

char* RawMode::GET::get_render_pos(int pos)
{
	return e.row[pos].render;
}

char* RawMode::GET::get_render_pos_pos(int pos, int render_pos)
{
	return &e.row[pos].render[render_pos];
}

int RawMode::GET::get_rx()
{
	return e.rx;
}

char* RawMode::GET::get_statusmsg()
{
	return e.statusmsg;
}

time_t RawMode::GET::get_time()
{
	return e.statusmsg_time;
}

int RawMode::GET::get_dirty()
{
	return e.dirty;
}

void RawMode::SET::set_x(int x)
{
	e.cx = x;
}

void RawMode::SET::set_y(int y)
{
	e.cy = y;
}

void RawMode::SET::set_erow_size(ssize_t linelen, int at)
{
	e.row[at].size = linelen;
}

void RawMode::SET::set_erow_chars(ssize_t linelen, char* data, int at)
{
	e.row[at].chars = (char*)malloc(linelen + 1);
	memcpy(e.row[at].chars, data, linelen);
}

void RawMode::SET::set_last_erow_char(ssize_t linelen, int at)
{
	e.row[at].chars[linelen] = '\0';
}

void RawMode::SET::set_num_rows(int num)
{
	e.num_rows = num;
}

void RawMode::SET::set_erow()
{
	e.row = (erow*)realloc(e.row, sizeof(erow) * (e.num_rows + 1));
}

void RawMode::SET::set_rowoff(int num)
{
	e.rowoff = num;
}

void RawMode::SET::set_coloff(int num)
{
	e.coloff = num;
}

void RawMode::SET::set_rsize_pos(int pos, int data)
{
	e.row[pos].rsize = data;
}

void RawMode::SET::set_render_pos(int pos, char* data)
{
	e.row[pos].render = data;
}

void RawMode::SET::set_rx(int data)
{
	e.rx = data;
}

void RawMode::SET::set_filename(char* data)
{
	e.filename = strdup(data);
}

void RawMode::SET::set_dirty(int data)
{
	e.dirty = data;
}