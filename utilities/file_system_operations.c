#include <headers.h>
#include <glib.h>
#include <glib/gstdio.h>
/**

    @file file_system_operations.c
    @brief Functions for managing data directories and CSV files.
    This file includes functions to check for the existence of a data
    directory and to generate CSV filenames based on table characteristics. */

/**
 * @brief Checks if the data directory exists and creates it if necessary.
 *
 * This function verifies if the specified data directory exists. If it
 * does not exist, it attempts to create the directory. If the creation
 * is successful or if the directory already exists, it returns `TRUE`.
 * If it fails to create the directory, it returns `FALSE`.
 *
 * @param data_passer A pointer to the Data_Passer structure which contains
 *                    the path to the data directory.
 * @return `TRUE` if the directory exists or is successfully created,
 *         `FALSE` if the directory cannot be created.
 */
gboolean exists_data_directory(Data_Passer *data_passer) {
	if (g_file_test(data_passer->data_directory, G_FILE_TEST_IS_DIR)) {
		return TRUE;
	}
	// On GUI, ask if want to create the directory. If use says no, return false
	if (g_mkdir(data_passer->data_directory, 0755) == 0) {

		return TRUE;
	} else {
		// On GUI, say could not make data directory, exited
		return FALSE;
	}
}

/**
 * @brief Constructs CSV filenames for table characteristics.
 *
 * This function generates a CSV filename based on the given table
 * characteristic, and appends it to the GSList of CSV files in the
 * Data_Passer structure.
 *
 * @param data A pointer to a Table_Characteristic structure containing
 *             the table name.
 * @param user_data A pointer to the Data_Passer structure which contains
 *                  the directory for CSV file storage.
 */
void get_csv_files( gpointer data,  gpointer user_data) {
	Data_Passer *data_passer = (Data_Passer *) user_data;
	Table_Characteristic *table_characteristic = (Table_Characteristic *) data;

	gchar *filename = g_malloc(MAX_FILE_NAME_LENGTH);
	gchar *pointer = g_stpcpy (filename, data_passer->csv_file_directory);
	pointer = g_stpcpy (filename, table_characteristic->table_name);
	pointer = g_stpcpy (pointer,".csv");
	data_passer->run_time.csv_files = g_slist_append (data_passer->run_time.csv_files, filename);
}
