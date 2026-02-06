#include <sqlite3.h>
#include <stddef.h>
#include <glib.h>
#include "../headers/headers.h"

gboolean create_database(Data_Passer *data_passer, gchar *database_file) {
	const gchar csv_files[16][40] = { "food_attribute.csv", "food_attribute_type.csv",
			"food_calorie_conversion_factor.csv", "food_category.csv",
			"food.csv", "food_nutrient_conversion_factor.csv",
			"food_nutrient.csv", "food_nutrient_derivation.csv",
			"food_nutrient_source.csv", "food_portion.csv",
			"food_protein_conversion_factor.csv", "food_update_log_entry.csv",
			"measure_unit.csv", "nutrient.csv", "retention_factor.csv",
			"sr_legacy_food.csv" };

	int rc = sqlite3_open(database_file, &(data_passer->run_time.db));
	if (rc != SQLITE_OK) {
		write_log_message(G_LOG_LEVEL_CRITICAL, "Could not create the database, exiting.",
						data_passer->run_time.log_file);
		return FALSE;
	}
	get_csv_files(data_passer);

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

