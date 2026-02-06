/*
 * create_tables.c
 *
 *  Created on: Feb 5, 2026
 *      Author: abba
 */
#include <glib.h>

gchar *make_csv_filename(const gchar *directory, const gchar *csv_file) {
	gchar *csv_filename = g_malloc( 256 *  sizeof(gchar));
	gchar *pointer = g_stpcpy(csv_filename, directory);
	pointer = g_stpcpy(pointer, csv_file);
	return csv_filename;
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

