#include "RawMode.h"
#include "Window.h"

EditorConfig e;

RawMode *RawMode::m_raw = nullptr;

void RawMode::disable()
{
	delwin(e.win);
}

void RawMode::editor_set_status_message(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(e.statusmsg, sizeof(e.statusmsg), fmt, ap);
	va_end(ap);
	e.statusmsg_time = time(NULL);
}

RawMode *RawMode::get()
{
	return m_raw;
}

RawMode::RawMode()
{
	e.win = newwin(e.screenrows, e.screencols, e.cy, e.cx);
	initscr();
	raw();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	Window::update_screen();
	signal(SIGWINCH, Window::handle_change);
}

RawMode::~RawMode()
{
}

void RawMode::create()
{
	if (RawMode::m_raw)
		throw "Raw mode already enabled";
	RawMode::m_raw = new RawMode();
}

void RawMode::release()
{
	if (!RawMode::m_raw)
		return;
	delete RawMode::m_raw;
}
