#include <sqlite3.h>
#include <stddef.h>
#include <glib.h>
#include "../headers/headers.h"



gboolean create_database(Data_Passer *data_passer, gchar *database_file) {

	int rc = sqlite3_open(database_file, &(data_passer->run_time.db));
	if (rc != SQLITE_OK) {
		write_log_message(G_LOG_LEVEL_CRITICAL,
				"Could not create the database, exiting.",
				data_passer->run_time.log_file);
		return FALSE;
	}
	get_csv_files(data_passer);
	if (data_passer->run_time.csv_files == NULL) {
		write_log_message(G_LOG_LEVEL_CRITICAL,
				"Could not find the list of CSV files, exiting.",
				data_passer->run_time.log_file);
		return FALSE;
	}
	// The regex pattern is "(.*?)"
	data_passer->csv_column_name_regex = g_regex_new("\"(.*?)\"", G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &(data_passer->error));

	 if (data_passer->csv_column_name_regex == NULL) {
		 g_print("Error creating regex: %s, will not be able to create the database", data_passer->error->message);
		 g_error_free(data_passer->error);
	 }

	g_slist_foreach(data_passer->run_time.csv_files, make_table, data_passer);
	g_regex_unref(data_passer->csv_column_name_regex);

	return TRUE;

}

gboolean open_database(Data_Passer *data_passer) {

	gchar database_file[256];
	gint rc;
	gchar *pointer = g_stpcpy(database_file, data_passer->data_directory);
	pointer = g_stpcpy(pointer, data_passer->database_filename);

	if (!g_file_test(database_file, G_FILE_TEST_EXISTS)) {
		write_log_message(G_LOG_LEVEL_INFO, "Database not found, creating it",
				data_passer->run_time.log_file);

		if (create_database(data_passer, database_file)) {
			return TRUE;
		} else {
			return FALSE;
		}

	} else {
		rc = sqlite3_open(database_file, &(data_passer->run_time.db));
		if (rc == SQLITE_OK) {
			return TRUE;
		}

	}

	return FALSE;
}

