#include <headers.h>
#include <stddef.h>
/**
    @file cleanup.c

    @brief Functions for freeing resources and performing cleanup.

    This file contains functions to free dynamically allocated memory for foreign key
    structures, table characteristics, and to perform cleanup for the `Data_Passer` structure.
*/


/**
 * @brief Frees memory allocated for a `Foreign_Key` structure.
 *
 * This callback function frees memory for the local column, foreign column, and
 * foreign table associated with a foreign key, and then frees the foreign key
 * structure itself.
 *
 * @param data A pointer to the `Foreign_Key` structure to be freed.
 * @see `free_table_characteristics`
 */
void free_foreign_keys(gpointer data) {
	Foreign_Key *foreign_key = (Foreign_Key *) data;
	g_free(foreign_key->local_column);
	g_free(foreign_key->foreign_column);
	g_free(foreign_key->foreign_table);
	g_free(foreign_key);
}


/**
 * @brief Frees memory allocated for a `Table_Characteristic` structure.
 *
 * This callback function frees memory for the table name, primary key, and any
 * associated foreign keys within a `Table_Characteristic` structure. It also
 * frees the Table_Characteristic structure itself once all its components
 * have been released.
 *
 * @param data A pointer to the Table_Characteristic structure to be freed.
 * @see `free_foreign_keys`
 */
void free_table_characteristics(gpointer data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic *)data;
	g_free(table_characteristic->table_name );
	g_free(table_characteristic->primary_key);
	if (table_characteristic->foreign_keys) {
		g_slist_free_full (table_characteristic->foreign_keys, free_foreign_keys);
	}
	g_free(table_characteristic);
}

/**
 * @brief Cleans up and frees allocated memory in the `Data_Passer` structure.
 *
 * This function releases all dynamically allocated memory associated with the
 * `Data_Passer` structure, including file paths, log files, CSV file lists,
 * and table characteristics. It also ensures that the database connection
 * is closed properly.
 *
 * @param data_passer A pointer to the Data_Passer structure to be cleaned up.
 * @see `free_table_characteristics`
 */
void cleanup(Data_Passer *data_passer) {
    if (data_passer->data_directory) {
        g_free(data_passer->data_directory);
    }
    if (data_passer->database_filename) {
        g_free(data_passer->database_filename);
    }
    if (data_passer->log_filename) {
        g_free(data_passer->log_filename);
    }
    if (data_passer->csv_file_directory) {
        g_free(data_passer->csv_file_directory);
    }

	write_log_message(G_LOG_LEVEL_INFO, "Application ending, shutting down log file", data_passer->run_time.log_file);

	if (data_passer->run_time.log_file) {
		fclose(data_passer->run_time.log_file);
	}
	/* Free the definitions of CSV files stored as a GSList in get_csv_files. */
	if (data_passer->run_time.csv_files) {
		g_slist_free_full (data_passer->run_time.csv_files, g_free);
	}
	if (data_passer->run_time.db) {
		sqlite3_close(data_passer->run_time.db);
	}

	/* Free the definitions of table characteristics stored as a GSList in read_table_characteristic. */
	if (data_passer->table_characteristics) {
		g_slist_free_full (data_passer->table_characteristics, free_table_characteristics);
	}

    sqlite3_finalize(data_passer->stmt_is_table_exists);

    g_free(data_passer);

}
