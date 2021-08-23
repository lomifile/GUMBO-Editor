#include "RawMode.h"
#include "Window.h"

EditorConfig e;

/** KDE Konsole can give some problems
 */

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
	Window::update_screen();
	signal(SIGWINCH, Window::handle_change);
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