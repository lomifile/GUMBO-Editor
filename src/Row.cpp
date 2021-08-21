#include "Row.h"

void Row::editor_row_insert_char(erow* row, int at, int c)
{
	if (at < 0 || at > row->size) at = row->size;
	row->chars = (char*)realloc(row->chars, row->size + 2);
	memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
	row->size++;
	row->chars[at] = c;
	update_row(row);
}

int Row::editor_row_rx_to_cx(erow* row, int rx)
{
	int cur_rx = 0;
	int cx;
	for (cx = 0; cx < row->size; cx++) {
		if (row->chars[cx] == '\t')
			cur_rx += (7 - 1) - (cur_rx % 7);
		cur_rx++;
		if (cur_rx > rx) return cx;
	}
	return cx;
}

int Row::editor_row_cx_to_rx(erow* row, int cx)
{
	int rx = 0;
	int j;
	for (j = 0; j < cx; j++) {
		if (row->chars[j] == '\t')
			rx += (7 - 1) - (rx % 7);
		rx++;
	}
	return rx;
}

void Row::update_row(erow* row)
{
	int tabs = 0;
	int j;
	for (j = 0; j < row->size; j++)
		if (row->chars[j] == '\t') tabs++;
		free(row->render);
		row->render = (char*)malloc(row->size + tabs*7 + 1);
		int idx = 0;
		for (j = 0; j < row->size; j++) {
			if (row->chars[j] == '\t') {
				row->render[idx++] = ' ';
				while (idx % 8 != 0) row->render[idx++] = ' ';
			} else {
				row->render[idx++] = row->chars[j];
			}
		}
		row->render[idx] = '\0';
		row->rsize = idx;

		Syntax::editor_update_syntax(row);
}

void Row::editor_insert_row(int at, char* s, size_t len)
{
	if (at < 0 || at > e.num_rows) return;
	e.row = (erow*)realloc(e.row, sizeof(erow) * (e.num_rows + 1));
	memmove(&e.row[at + 1], &e.row[at], sizeof(erow) * (e.num_rows - at));
	for (int j = at + 1; j <= e.num_rows; j++) e.row[j].idx++;
	e.row[at].idx = at;

	RawMode::SET::set_erow();

	RawMode::SET::set_erow_size(len, at);
	RawMode::SET::set_erow_chars(len, s, at);
	RawMode::SET::set_last_erow_char(len, at);
	RawMode::SET::set_rsize_pos(at, 0);
	RawMode::SET::set_render_pos(at, NULL);
	e.row[at].hl = NULL;
	e.row[at].hl_open_comment = 0;
	update_row(RawMode::GET::get_erow_pos(at));
	RawMode::SET::set_num_rows(RawMode::GET::get_numrows() + 1);
	RawMode::SET::set_dirty(RawMode::GET::get_dirty() + 1);
}

void Row::editor_row_delete_cahr(erow* row, int at)
{
	if (at < 0 || at >= row->size) return;
	memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
	row->size--;
	update_row(row);
	RawMode::SET::set_dirty(RawMode::GET::get_dirty() + 1);
}

void Row::free_row(erow* row)
{
	free(row->render);
	free(row->chars);
	free(row->hl);
}

void Row::delete_row(int at)
{
	if (at < 0 || at >= e.num_rows) return;
	free_row(&e.row[at]);
	memmove(&e.row[at], &e.row[at + 1], sizeof(erow) * (e.num_rows - at - 1));
	for (int j = at; j < e.num_rows - 1; j++) e.row[j].idx--;
	e.num_rows--;
	e.dirty++;
}

void Row::append_string_(erow* row, char* s, size_t len)
{
	row->chars = (char*)realloc(row->chars, row->size + len + 1);
	memcpy(&row->chars[row->size], s, len);
	row->size += len;
	row->chars[row->size] = '\0';
	update_row(row);
	e.dirty++;
}
