#pragma once
#include <stdio.h>
#include <glib.h>
#include "constants.h"
/**
 * @file structures.h
 * @brief Data structures for the Diet Optimizer application.
 *
 * This file contains type definitions used throughout the Diet Optimizer application.
 */

/**
 * @brief Structure to hold runtime information.
 *
 * This structure encapsulates the runtime information required by the
 * Diet Optimizer application, including the log file, database,
 * and list of CSV files ingested into the database.
 */
typedef struct Run_Time {
	FILE *log_file;  /**< Pointer to the log file for application logging. */
	sqlite3 *db;  /**< Pointer to the SQLite database connection. */
	GSList *csv_files; /**< List of CSV files to be processed. */
} Run_Time;

/**
 * @brief Structure to define a foreign key relationship.
 *
 * This structure represents a foreign key relationship between
 * tables in the database, specifying the local column, the
 * foreign table, and the corresponding foreign column.
 */
typedef struct Foreign_Key {
	gchar *local_column; /**< Local column name that references the foreign table. */
	gchar *foreign_table;  /**< Name of the foreign table. */
	gchar *foreign_column; /**< Column name in the foreign table. */
} Foreign_Key;


/**
 * @brief Structure to hold characteristics of a database table.
 *
 * This structure captures the essential characteristics of a table,
 * including the table name, its primary key, and a list of
 * foreign keys associated with it.
 */
typedef struct Table_Characteristic {
	gchar *table_name; /**< Name of the table. */
	gchar *primary_key; /**< Primary key column name for the table. */
	GSList *foreign_keys; /**< List of foreign keys associated with the table. */
} Table_Characteristic;


/**
 * @brief Structure to pass data configuration and state throughout the application.
 *
 * This structure manages various parameters related to data handling,
 * including file paths, regex for column names, error management,
 * and characteristics of tables.
 */
typedef struct Data_Passer {
	gchar *data_directory; /**< Directory where ingested CSV files are stored. */
	gchar *database_filename;  /**< Name of the SQLite database file. */
	gchar *log_filename;  /**< Name of the log file. */
	gchar *csv_file_directory; /**< Directory where CSV files are located. */
	GRegex *csv_column_name_regex;  /**< Regex pattern for capturing CSV column names. */
	GError *error; /**< Pointer to capture GLib errors encountered. */
	GSList *table_characteristics; /**< List of table characteristics, one entry corresponding to one data file. */
	gchar sqlite_prepare_dummies[MAX_NUMBER_TABLE_COLUMNS / 2];  /**< Array for SQLite preparation dummies. */
	/* Toss this member? */
	sqlite3_stmt *stmt_is_table_exists; /**< Prepared statement to check if a table exists. */
	Run_Time run_time;  /**< Runtime information for the application. */
} Data_Passer;

/**
 * @brief Enum for SQLite data types.
 *
 * This enum lists the various SQLite data types, in addition to a
 * TRASH entry indicating the column type is uninitialized.
 */
enum sqlite_data_type {
	TRASH, /**< Artificial type for initialization. */
	NULL_S, /** SQLite `NULL` value. */
	INTEGER, /** Integer. */
	REAL, /**< Real number. */
	TEXT /**< Text string. */
};

/**
 * @brief Structure describing the characteristics of a SQLite column.
 *
 * This structure describes a SQLite table's column, including the column
 * name, data type, and a flag indicating the column is the table's primary key.
 */
typedef struct Column_Definition {
	gchar column_name[MAX_COLUMN_NAME_LENGTH];  /**< Column name. */
	enum sqlite_data_type column_type;  /**< Column's data type. */
	gboolean is_primary_key;  /**< TRUE or FALSE, indicating the column is a primary key. */
} Column_Definition;
