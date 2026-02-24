#pragma once
#include <stdio.h>
#include <glib.h>
#include "constants.h"
/*
 * structures.h
 *
 *  Created on: Feb 4, 2026
 *      Author: abba
 */

typedef struct Run_Time {
	FILE *log_file;
	sqlite3 *db;
	GSList *csv_files;
} Run_Time;

typedef struct Foreign_Key {
	gchar *local_column;
	gchar *foreign_table;
	gchar *foreign_column;
} Foreign_Key;

typedef struct Table_Characteristic {
	gchar *table_name;
	gchar *primary_key;
	GSList *foreign_keys;
} Table_Characteristic;



typedef struct Data_Passer {
	gchar *data_directory;
	gchar *database_filename;
	gchar *log_filename;
	gchar *csv_file_directory;
	GRegex *csv_column_name_regex;
	GError *error;
	GSList *table_characteristics;
	gchar sqlite_prepare_dummies[MAX_NUMBER_TABLE_COLUMNS / 2];

	/* Toss this member? */
	sqlite3_stmt *stmt_is_table_exists;
	Run_Time run_time;
} Data_Passer;

/**
 * Enum for declaring a list store of accounts. These correspond to the SQLite data types (see [Storage Classes and Datatypes](https://sqlite.org/datatype3.html#storage_classes_and_datatypes). This enumeration is used in Field_analysis.
 */
enum sqlite_data_type {
	TRASH, /**< Artificial type for initialization. */
	NULL_S, /** SQLite `NULL` value. */
	INTEGER, /** Integer. */
	REAL, /**< Real number. */
	TEXT /**< Text string. */
};

typedef struct Column_Definition {
	gchar column_name[MAX_COLUMN_NAME_LENGTH];
	enum sqlite_data_type column_type;
	gboolean is_primary_key;
} Column_Definition;
