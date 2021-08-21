#include "RawMode.h"
#include "IO.h"
#include "File.h"

using namespace RawMode;

int main(int argc, char *argv[])
{
	enable();
	init();
	if (argc >= 2) {
		File::editor_open(argv[1]);
	}
	IO io = IO();
	editor_set_status_message("HELP: Ctrl-S = save | Ctrl-Q = quit");
	while (1)
	{
		io.editor_refresh_screen();
		io.editor_process_keypress();
	}
}