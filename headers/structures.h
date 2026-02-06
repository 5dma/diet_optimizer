#pragma once
#include <gtk/gtk.h>
/*
 * structures.h
 *
 *  Created on: Feb 4, 2026
 *      Author: abba
 */

typedef struct Run_Time {
	FILE *log_file;
	sqlite3 *db;
} Run_Time;

typedef struct Data_Passer {
	gchar *data_directory;
	gchar *database_filename;
	gchar *log_filename;
	gchar *csv_file_directory;
	Run_Time run_time;
} Data_Passer;
