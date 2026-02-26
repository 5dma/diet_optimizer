#include <sqlite3.h>
#include <stddef.h>
#include <glib.h>
#include "../headers/headers.h"
/**
 * @file create_database.c
 * @brief Functions for opening and creating an SQLite database.
 *
 * This file contains functions to create and open an SQLite database based on the FoodData CSV downloads.
 */

/**
 * @brief Creates a new SQLite database.
 *
 * This function attempts to create an SQLite database by opening
 * a connection to the specified file. It also enables foreign key support,
 * checks for the existence of required tables, and processes CSV files.
 *
 * @param data_passer A pointer to the Data_Passer structure containing
 *                    database configuration and state information.
 * @param database_file The path to the database file to be created.
 *
 * @return TRUE if the database was successfully created and initialized,
 *         FALSE otherwise.
 */
gboolean create_database(Data_Passer *data_passer, gchar *database_file) {

	int rc = sqlite3_open(database_file, &(data_passer->run_time.db));
	if (rc != SQLITE_OK) {
		write_log_message(G_LOG_LEVEL_CRITICAL,
				"Could not create the database, exiting.",
				data_passer->run_time.log_file);
		return FALSE;
	}

	gchar *errmsg = NULL;
	rc = sqlite3_exec(data_passer->run_time.db, "PRAGMA foreign_keys = ON", 0, 0,
			&errmsg);
	if (rc != SQLITE_OK) {
		g_print("SQL error: %s\n", errmsg);
		sqlite3_free(errmsg);
	}

	if (sqlite3_prepare_v2(data_passer->run_time.db, "SELECT count(*) FROM sqlite_master WHERE type='table' AND name=?", -1, &(data_passer->stmt_is_table_exists), NULL) != SQLITE_OK) {
		g_print("Error preparing check: %s\n", sqlite3_errmsg(data_passer->run_time.db));
	}

	g_slist_foreach(data_passer->table_characteristics, get_csv_files, data_passer);

	if (data_passer->run_time.csv_files == NULL) {
		write_log_message(G_LOG_LEVEL_CRITICAL,
				"Could not find the list of CSV files, exiting.",
				data_passer->run_time.log_file);
		return FALSE;
	}
	// The regex pattern is "([^"]*(?:""[^"]*)*)"
	data_passer->csv_column_name_regex = g_regex_new("\"([^\"]*(?:\"\"[^\"]*)*)\"", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &(data_passer->error));

	 if (data_passer->csv_column_name_regex == NULL) {
		 g_print("Error creating regex: %s, will not be able to create the database", data_passer->error->message);
		 g_error_free(data_passer->error);
	 }

	g_slist_foreach(data_passer->run_time.csv_files, make_table, data_passer);
	g_regex_unref(data_passer->csv_column_name_regex);
	return TRUE;

}

/**
 * @brief Opens an existing SQLite database or creates a new one.
 *
 * This function checks if the database file exists. If it does not,
 * it attempts to create it using the `create_database` function.
 * If the file exists, it opens the database connection.
 *
 * @param data_passer A pointer to the Data_Passer structure containing
 *                    database configuration and state information.
 *
 * @return TRUE if the database was successfully opened or created,
 *         FALSE otherwise.
 */
gboolean open_database(Data_Passer *data_passer) {

	gchar database_file[256];
	gint rc;
	gchar *pointer = g_stpcpy(database_file, data_passer->data_directory);
	pointer = g_stpcpy(pointer, data_passer->database_filename);

	if (!g_file_test(database_file, G_FILE_TEST_EXISTS)) {
		write_log_message(G_LOG_LEVEL_INFO, "Database not found, creating it",
				data_passer->run_time.log_file);

		if (create_database(data_passer, database_file)) {
			write_log_message(G_LOG_LEVEL_INFO, "Successfully created the database",
							data_passer->run_time.log_file);
			return TRUE;
		} else {
			write_log_message(G_LOG_LEVEL_CRITICAL, "Could not create the database, exiting.",
							data_passer->run_time.log_file);

			return FALSE;
		}

	} else {
		rc = sqlite3_open(database_file, &(data_passer->run_time.db));
		if (rc == SQLITE_OK) {
			write_log_message(G_LOG_LEVEL_INFO, "Successfully opened the database",
							data_passer->run_time.log_file);
			return TRUE;
		}

	}
	write_log_message(G_LOG_LEVEL_CRITICAL, "Could not open the database, exiting.",
								data_passer->run_time.log_file);

	return FALSE;
}

