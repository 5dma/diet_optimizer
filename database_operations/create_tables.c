/*
 * create_tables.c
 *
 *  Created on: Feb 5, 2026
 *      Author: abba
 */
#include <glib.h>
#include <headers.h>

gchar* make_csv_filename(const gchar *directory, const gchar *csv_file) {
	gchar *csv_filename = g_malloc(256 * sizeof(gchar));
	gchar *pointer = g_stpcpy(csv_filename, directory);
	pointer = g_stpcpy(pointer, csv_file);
	return csv_filename;
}

void make_table(gpointer filename, gpointer user_data) {
	GError *error = NULL;
	Data_Passer *data_passer = (Data_Passer*) user_data;
	// The regex pattern is "([^"]+)"
	GRegex *regex = g_regex_new("\"([^\"]+)\"", G_REGEX_DEFAULT,
			G_REGEX_MATCH_DEFAULT, &error);

	if (regex == NULL) {
		g_print("Error creating regex: %s", error->message);
		g_error_free(error);
		return;
	}

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
	g_print("%s\n", line);
	fclose(file);
	g_free(csv_filename);

	GMatchInfo *match_info = NULL;
	g_regex_match(regex, line, 0, &match_info);
	if (error) {
		g_printerr("Error matching regex: %s\n", error->message);
		g_error_free(error);
		g_regex_unref(regex);
		return;
	}

    do {
        gchar *match = g_match_info_fetch(match_info, 1);  // Fetch the first capturing group
        if (match) {
            g_print("Found match: %s\n", match);
        }
        g_free(match);
        g_match_info_free(match_info);
    } while (g_match_info_next(match_info, &error));
    g_regex_unref(regex);
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

