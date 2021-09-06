#include "File.h"
#include "RawMode.h"

/* python */
char *PY_HL_extensions[] = {(char*)".py", (char*)"python", NULL};
char *PY_HL_keywords[] = {
	(char*)"from", "def", "if ", "while", "for", "break", "return", "continue", "else", "elif",
	"import", "try", "except", "in", "and", "or", "is", "not", "with", "as",
	"True", "False", "None", "class",
	/* Python types */
	"int|", "str|", "unicode|", "dict|", "float|", "repr|", "long|", "eval|",
	"tuple|", "list|", "set|", "frozenset|", "chr|", "unichr|", "ord|", "hex|",
	NULL};

char *C_HL_extensions[] = {(char *)".c", (char *)".h", (char *)".cpp", (char *)".hpp", NULL};
char *C_HL_keywords[] = {
	/* C Keywords */
	"#define", "#include", "auto", "break", "case", "continue", "default", "do", "else", "enum",
	"extern", "for", "goto", "if", "register", "return", "sizeof", "static",
	"struct", "switch", "typedef", "union", "volatile", "while", "NULL",

	/* C++ Keywords */
	"alignas", "alignof", "and", "and_eq", "asm", "bitand", "bitor", "class",
	"compl", "constexpr", "const_cast", "deltype", "delete", "dynamic_cast",
	"explicit", "export", "false", "friend", "inline", "mutable", "namespace",
	"new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq",
	"private", "protected", "public", "reinterpret_cast", "static_assert",
	"static_cast", "template", "this", "thread_local", "throw", "true", "try",
	"typeid", "typename", "virtual", "xor", "xor_eq",

	/* C types */
	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
	"void|", "short|", "auto|", "const|", "bool|", NULL};

struct EditorSyntax HLDB[] = {
	{
		(char *)"c",
		C_HL_extensions,
		C_HL_keywords,
		(char *)"//",
		(char *)"/*",
		(char *)"*/",
		HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
	},
	{
		(char *)"python",
		PY_HL_extensions,
		PY_HL_keywords,
		(char *)"#",
		(char *)"\"\"\"",
		(char *)"\"\"\"",
		HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
	}};

void File::save()
{
	if (e.filename == NULL)
	{
		e.filename = IO::editor_prompt((char *)"Save as: %s (ESC to cancel)", NULL);
		if (e.filename == NULL)
		{
			RawMode::editor_set_status_message("Save aborted");
			return;
		}
		Syntax::editor_select_syntax_highlight();
	}
    
    if(access(e.filename, F_OK) == 0)
    {
        RawMode::editor_set_status_message((char*)"File already exists!");
        return;
    }

	int len;
	char *buf = editor_rows_to_string(&len);
	int fd = open(e.filename, O_RDWR | O_CREAT, 0644);
	if (fd != -1)
	{
		if (ftruncate(fd, len) != -1)
		{
			if (write(fd, buf, len) == len)
			{
				close(fd);
				free(buf);
				e.dirty = 0;
				RawMode::editor_set_status_message("%d bytes written to disk", len);
				return;
			}
		}
		close(fd);
	}
	free(buf);
	RawMode::editor_set_status_message("Can't save! I/O error: %s", strerror(errno));
	Logger::append_log(Logger::time_now(), strerror(errno));
}

char *File::editor_rows_to_string(int *buflen)
{
	int totlen = 0;
	int j;
	for (j = 0; j < e.num_rows; j++)
		totlen += e.row[j].size + 1;
	*buflen = totlen;

	char *buf = (char *)malloc(totlen);
	char *p = buf;
	for (j = 0; j < e.num_rows; j++)
	{
		memcpy(p, e.row[j].chars, e.row[j].size);
		p += e.row[j].size;
		*p = '\n';
		p++;
	}

	return buf;
}

void File::editor_open(char *filename)
{
	FILE *fp;
	free(e.filename);
	e.filename = strdup(filename);

	Syntax::editor_select_syntax_highlight();

	if (access(filename, F_OK) != 0)
	{
		fp = fopen(filename, "w+");
		fclose(fp);
	}

	fp = fopen(filename, "r+");
	if (!fp)
		Error::die("File open error");
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline(&line, &linecap, fp)) != -1)
	{
		while (linelen > 0 && (line[linelen - 1] == '\n' ||
							   line[linelen - 1] == '\r'))
			linelen--;
		Row::editor_insert_row(e.num_rows, line, linelen);
	}
	free(line);
	fclose(fp);
	e.dirty = 0;
}

void File::editor_find_callback(char *query, int key)
{
	static int last_match = -1;
	static int direction = 1;

	static int saved_hl_line;
	static char *saved_hl = NULL;
	if (saved_hl)
	{
		memcpy(e.row[saved_hl_line].hl, saved_hl, e.row[saved_hl_line].rsize);
		free(saved_hl);
		saved_hl = NULL;
	}

	if (key == ENTER || key == 27)
	{
		last_match = -1;
		direction = 1;
		return;
	}
	else if (key == KEY_RIGHT || key == KEY_DOWN)
	{
		direction = 1;
	}
	else if (key == KEY_RIGHT || key == KEY_DOWN)
	{
		direction = -1;
	}
	else
	{
		last_match = -1;
		direction = 1;
	}
	if (last_match == -1)
		direction = 1;
	int current = last_match;
	int i;
	for (i = 0; i < e.num_rows; i++)
	{
		current += direction;
		if (current == -1)
			current = e.num_rows - 1;
		else if (current == e.num_rows)
			current = 0;
		erow *row = &e.row[current];
		char *match = strstr(row->render, query);
		if (match)
		{
			last_match = current;
			e.cy = current;
			e.cx = Row::editor_row_rx_to_cx(row, (int)(match - row->render));
			e.rowoff = e.num_rows;
			saved_hl_line = current;
			saved_hl = (char *)malloc(row->rsize);
			memcpy(saved_hl, row->hl, row->rsize);
			memset(&row->hl[match - row->render], (int)EditorHighlight::HL_MATCH, strlen(query));
			break;
		}
	}
}

void File::search()
{
	int saved_cx = e.cx;
	int saved_cy = e.cy;
	int saved_coloff = e.coloff;
	int saved_rowoff = e.rowoff;

	char *query = IO::editor_prompt((char *)"Search: %s (Use ESC/Arrows/Enter)",
									File::editor_find_callback);
	if (query)
	{
		free(query);
	}
	else
	{
		e.cx = saved_cx;
		e.cy = saved_cy;
		e.coloff = saved_coloff;
		e.rowoff = saved_rowoff;
	}
}

void Syntax::editor_update_syntax(erow *row)
{
	row->hl = (unsigned char *)realloc(row->hl, row->rsize);
	memset(row->hl, (int)EditorHighlight::HL_NORMAL, row->rsize);

	if (e.syntax == NULL)
		return;
	char **keywords = e.syntax->keywords;

	char *scs = e.syntax->singleline_comment_start;
	char *mcs = e.syntax->multiline_comment_start;
	char *mce = e.syntax->multiline_comment_end;

	int scs_len = scs ? (int)strlen(scs) : 0;
	int mcs_len = mcs ? (int)strlen(mcs) : 0;
	int mce_len = mce ? (int)strlen(mce) : 0;

	int prev_sep = 1;
	int in_string = 0;
	int in_comment = (row->idx > 0 && e.row[row->idx - 1].hl_open_comment);
	;
	int i = 0;
	while (i < row->rsize)
	{
		char c = row->render[i];
		unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : (unsigned char)EditorHighlight::HL_NORMAL;

		if (scs_len && !in_string && !in_comment)
		{
			if (!strncmp(&row->render[i], scs, scs_len))
			{
				memset(&row->hl[i], (int)EditorHighlight::HL_COMMENT, row->rsize - i);
				break;
			}
		}

		if (mcs_len && mce_len && !in_string)
		{
			if (in_comment)
			{
				row->hl[i] = (unsigned char)EditorHighlight::HL_MLCOMMENT;
				if (!strncmp(&row->render[i], mce, mce_len))
				{
					memset(&row->hl[i], (int)EditorHighlight::HL_MLCOMMENT, mce_len);
					i += mce_len;
					in_comment = 0;
					prev_sep = 1;
					continue;
				}
				else
				{
					i++;
					continue;
				}
			}
			else if (!strncmp(&row->render[i], mcs, mcs_len))
			{
				memset(&row->hl[i], (int)EditorHighlight::HL_MLCOMMENT, mcs_len);
				i += mcs_len;
				in_comment = 1;
				continue;
			}
		}

		if (e.syntax->flags && (int)HL_HIGHLIGHT_STRINGS)
		{
			if (in_string)
			{
				row->hl[i] = (unsigned char)EditorHighlight::HL_STRING;
				if (c == '\\' && i++ < row->rsize)
				{
					row->hl[i++] = (unsigned char)EditorHighlight::HL_STRING;
					i += 2;
					continue;
				}
				if (c == in_string)
					in_string = 0;
				i++;
				prev_sep = 1;
				continue;
			}
			else
			{
				if (c == '"' || c == '\'')
				{
					in_string = c;
					row->hl[i] = (unsigned char)EditorHighlight::HL_STRING;
					i++;
					continue;
				}
			}
		}

		if (e.syntax->flags && (int)HL_HIGHLIGHT_NUMBERS)
		{
			if (isdigit(c) && (prev_sep || prev_hl == (unsigned char)EditorHighlight::HL_NORMAL) ||
				(c == '.' && prev_hl == (unsigned char)EditorHighlight::HL_NORMAL))
			{
				row->hl[i] = (int)EditorHighlight::HL_NUMBER;
				i++;
				prev_sep = 0;
				continue;
			}
		}

		if (prev_sep)
		{
			int j;
			for (j = 0; keywords[j]; j++)
			{
				int klen = (int)strlen(keywords[j]);
				int kw2 = keywords[j][klen - 1] == '|';
				if (kw2)
					klen--;
				if (!strncmp(&row->render[i], keywords[j], klen) &&
					is_separator(row->render[i + klen]))
				{
					memset(&row->hl[i],
						   kw2 ? (int)EditorHighlight::HL_KEYWORD1 : (int)EditorHighlight::HL_KEYWORD2,
						   klen);
					i += klen;
					break;
				}
			}
			if (keywords[j] != NULL)
			{
				prev_sep = 0;
				continue;
			}
		}

		prev_sep = is_separator(c);
		i++;
	}
	int changed = (row->hl_open_comment != in_comment);
	row->hl_open_comment = in_comment;
	if (changed && row->idx + 1 < e.num_rows)
		editor_update_syntax(&e.row[row->idx + 1]);
}

int Syntax::is_separator(int c)
{
	return isspace(c) || c == '\0' || strchr(",.()+-/*=~%<>[];", c) != NULL;
}

int Syntax::editor_syntax_color(int hl)
{
	switch (hl)
	{
	case (int)EditorHighlight::HL_MLCOMMENT:
	case (int)EditorHighlight::HL_COMMENT:
		return 36;
	case (int)EditorHighlight::HL_NUMBER:
		return 31;
	case (int)EditorHighlight::HL_MATCH:
		return 34;
	case (int)EditorHighlight::HL_STRING:
		return 35;
	case (int)EditorHighlight::HL_KEYWORD1:
		return 33;
	case (int)EditorHighlight::HL_KEYWORD2:
		return 32;
	default:
		return 37;
	}
}

void Syntax::editor_select_syntax_highlight()
{
	e.syntax = NULL;

	if (e.filename == NULL)
		return;

	char *ext = strchr(e.filename, '.');

	for (auto &j : HLDB)
	{
		struct EditorSyntax *s = &j;
		unsigned int i = 0;
		while (s->filematch[i])
		{
			int is_ext = (s->filematch[i][0] == '.');
			if ((is_ext && ext && !strcmp(ext, s->filematch[i])) || (!is_ext && strstr(e.filename, s->filematch[i])))
			{
				e.syntax = s;
				int filerrow;
				for (filerrow = 0; filerrow < e.num_rows; filerrow++)
				{
					editor_update_syntax(&e.row[filerrow]);
				}
				return;
			}
			i++;
		}
	}
}
