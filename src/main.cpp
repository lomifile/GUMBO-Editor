#include "RawMode.h"
#include "IO.h"
#include "File.h"

int main(int argc, char *argv[])
{
	RawMode::enable();
	RawMode::init();
	if (argc >= 2) {
		File::editor_open(argv[1]);
	}
	IO io = IO();
	RawMode::editor_set_status_message("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-N = Line numbers");
	while (1)
	{
		io.editor_refresh_screen();
		io.editor_process_keypress();
	}
}