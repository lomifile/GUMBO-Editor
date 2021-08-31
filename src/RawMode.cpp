#include "RawMode.h"
#include "Window.h"

EditorConfig e;
WINDOW *win;

/** 
 * Curses lib init
 */

void RawMode::init()
{
	initscr();
	raw();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	Window::update_screen();
	signal(SIGWINCH, Window::handle_change);
}

void RawMode::disable()
{
	delwin(win);
}

void RawMode::enable()
{
	win = newwin(e.screenrows, e.screencols, e.cy, e.cx);
}

void RawMode::editor_set_status_message(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(e.statusmsg, sizeof(e.statusmsg), fmt, ap);
	va_end(ap);
	e.statusmsg_time = time(NULL);
}