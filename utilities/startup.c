#include <headers.h>
#include <stddef.h>
#include <json-glib/json-glib.h>
/**

 @file startup.c

 @brief Functions for reading configuration data and initializing the data passer.
 This file contains functions to read configuration data from a JSON file
 and initialize the data passer structure used throughout the application.
 */

/**
 * @brief Reads configuration data from a JSON file.
 *
 * This function verifies the existence of a configuration file and reads its
 * contents to initialize the members of the `Data_Passer` structure. The JSON
 * file is expected to contain the necessary data paths required for application
 * runtime. If the file does not exist or cannot be parsed, the function
 * prints an error and returns `NULL`.
 *
 * @param data_passer A pointer to a `Data_Passer` structure to be initialized.
 * @return `TRUE` if successfully read the configuration file,
 *         `FALSE` otherwise. */
gboolean read_configuration_data(Data_Passer *data_passer) {
	JsonParser *parser;
	GError *error = NULL;

	gboolean input_file_exists = g_file_test(CONFIG_FILE, G_FILE_TEST_EXISTS);
	if (!input_file_exists) {
		g_print("The configuration file does not exist, exiting.\n");
		return FALSE;
	}

	parser = json_parser_new();
	error = NULL;
	json_parser_load_from_file(parser, CONFIG_FILE, &error);

	if (error) {
		g_print("Unable to parse `%s': %s", CONFIG_FILE, error->message);
		g_error_free(error);
		g_object_unref(parser);
		return FALSE;
	}

	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_object = json_node_get_object(root);

	/* Read the data information. */
	JsonObject *data = json_object_get_object_member(root_object, "data");

	data_passer->data_directory = g_strdup(
			json_object_get_string_member(data, "data_directory"));
	data_passer->database_filename = g_strdup(
			json_object_get_string_member(data, "database_filename"));
	data_passer->log_filename = g_strdup(
			json_object_get_string_member(data, "log_filename"));
	data_passer->csv_file_directory = g_strdup(
			json_object_get_string_member(data, "csv_file_directory"));
	data_passer->run_time.log_file = NULL;

	get_table_characteristics(data, data_passer);

	g_object_unref(parser);
	return TRUE;
}

/**
 * @brief Initializes a new Data_Passer structure.
 *
 * This function allocates memory for a new `Data_Passer` structure,
 * initializes its members to default values, and reads configuration data
 * from the specified configuration file. It prepares strings for SQL
 * statements based on the maximum number of table columns allowed.
 *
 * @return Data_Passer* A pointer to the newly initialized `Data_Passer` structure.
 */
Data_Passer* startup() {
	Data_Passer *data_passer = g_malloc(sizeof(Data_Passer));
	data_passer->data_directory = NULL;
	data_passer->database_filename = NULL;
	data_passer->log_filename = NULL;
	data_passer->csv_file_directory = NULL;
	data_passer->run_time.log_file = NULL;
	data_passer->run_time.db = NULL;
	data_passer->run_time.csv_files = NULL;
	data_passer->error = NULL;
	data_passer->csv_column_name_regex = NULL;
	data_passer->table_characteristics = NULL;

	g_strlcpy(data_passer->sqlite_prepare_dummies,
			"?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,",
			MAX_NUMBER_TABLE_COLUMNS / 2);

	/* If could not read the configuration file, return NULL. */
	if (read_configuration_data(data_passer)) {
		return data_passer;
	} else {
		return NULL;
	}
}
