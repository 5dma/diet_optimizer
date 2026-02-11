/*
 * create_tables.c
 *
 *  Created on: Feb 5, 2026
 *      Author: abba
 */
#include <glib.h>
#include <headers.h>
#include <string.h>

gchar* make_csv_filename(const gchar *directory, const gchar *csv_file) {
	gchar *csv_filename = g_malloc(256 * sizeof(gchar));
	gchar *pointer = g_stpcpy(csv_filename, directory);
	pointer = g_stpcpy(pointer, csv_file);
	return csv_filename;
}

void make_table(gpointer filename, gpointer user_data) {
	Data_Passer *data_passer = (Data_Passer*) user_data;

	gchar *csv_filename = make_csv_filename(data_passer->csv_file_directory,
			(gchar*) filename);
	g_print("Processing %s\n", (gchar*) csv_filename);

	FILE *file = fopen(csv_filename, "r");
	if (!file) {
		g_print("Error opening csv file %s\n", (gchar*) csv_filename);
		return;
	}
	gchar line[MAX_CSV_FILE_LINE_LENGTH];
	fgets(line, sizeof(line), file);

	GMatchInfo *match_info = NULL;
	data_passer->error = NULL;

	g_regex_match(data_passer->csv_column_name_regex, line, 0, &match_info);
	if (data_passer->error) {
		g_printerr("Error matching regex: %s\n", data_passer->error->message);
		g_error_free(data_passer->error);
		data_passer->error = NULL;
		return;
	}
	guint number_columns = 0;
	Column_Definition table_columns[10];
	gint current_column = 0;
	gchar *match = NULL;
	/* Parse the column names appearing in the first row */
	do {
		match = g_match_info_fetch(match_info, 1); // Fetch the first capturing group
		if (match) {
			g_print("Found match: %s\n", match);
			Column_Definition column_definition;
			g_strlcpy(column_definition.column_name, match,	MAX_COLUMN_NAME_LENGTH);
			column_definition.column_type = NULL_S;
			table_columns[current_column] = column_definition;
			current_column++;
			g_free(match);
		}

	} while (g_match_info_next(match_info, &(data_passer->error)));

	/* Read the second to last line, deciphering the SQLlite data type. */
	while (fgets(line, sizeof(line), file) != NULL) {
		g_print("%s\n", line);

		g_regex_match(data_passer->csv_column_name_regex, line, 0, &match_info);
		if (data_passer->error) {
			g_printerr("Error matching regex: %s\n",
					data_passer->error->message);
			g_error_free(data_passer->error);
			data_passer->error = NULL;
			return;
		}
		current_column = 0;
		do {
			match = g_match_info_fetch(match_info, 1);
			gint breakpoint = g_ascii_strcasecmp (match, "2201897");
			do_sqlite_tests(match, &(table_columns[current_column]));
			g_free(match);
			current_column++;
		} while (g_match_info_next(match_info, &(data_passer->error)));

	}
	fclose(file);
	g_match_info_free(match_info);
	g_free(csv_filename);
}

/*
 gchar *csv_filename = make_csv_filename(data_passer->csv_file_directory, csv_file);
 fopen
 read first line
 parse field names
 read remaining lines, deduce field type (NULL, Integer, Real, text)
 build table name (from .csv string)
 Build create table statement
 Execute create table statement


 g_free(csv_filename);
 return TRUE;*/

