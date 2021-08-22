#ifndef GUMBO_SRC_FILE_H
#define GUMBO_SRC_FILE_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>

#include "RawMode.h"
#include "Error.h"
#include "Row.h"
#include "IO.h"

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

struct EditorSyntax
{
	char* filetype;
	char** filematch;
	char** keywords;
	char* singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int flags;
};

extern struct EditorSyntax HLDB[];

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

enum class EditorHighlight
{
	HL_NORMAL = 0,
	HL_NUMBER,
	HL_STRING,
	HL_COMMENT,
	HL_MLCOMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_MATCH
};

namespace Syntax
{
	void editor_update_syntax(erow* row);
	int editor_syntax_color(int hl);
	int is_separator(int c);
	void editor_select_syntax_highlight();
}

namespace File
{
	void editor_open(char* filename);
	char* editor_rows_to_string(int* buflen);
	void save();
	void search();
	void editor_find_callback(char* query, int key);
}

#endif //GUMBO_SRC_FILE_H
