#include <headers.h>
#include <stddef.h>
#include <json-glib/json-glib.h>
/*
 * startup.c
 *
 *  Created on: Feb 4, 2026
 *      Author: abba
 */



void read_configuration_data(Data_Passer *data_passer) {
	JsonParser *parser;
	GError *error = NULL;

	/* Memory is freed at end of this function */

	gboolean input_file_exists = g_file_test(CONFIG_FILE, G_FILE_TEST_EXISTS);
	if (!input_file_exists) {
		g_print("The input file %s does not exist. Exiting\n", CONFIG_FILE);
		exit(-1);
	}

	parser = json_parser_new();
	error = NULL;
	json_parser_load_from_file(parser, CONFIG_FILE, &error);

	if (error) {
		g_print("Unable to parse `%s': %s\n", CONFIG_FILE, error->message);
		g_error_free(error);
		g_object_unref(parser);
		return;
	}

	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_object = json_node_get_object(root);

	/* Read the data information. */

	JsonObject *data = json_object_get_object_member(root_object,
			"data");

	data_passer->data_directory = g_strdup(
			json_object_get_string_member(data, "data_directory"));
	data_passer->database_filename = g_strdup(
			json_object_get_string_member(data,
					"database_filename"));
	data_passer->log_filename = g_strdup(
			json_object_get_string_member(data, "log_filename"));
	data_passer->csv_file_directory = g_strdup(
			json_object_get_string_member(data,
					"csv_file_directory"));
	data_passer->run_time.log_file = NULL;


	get_table_characteristics(data, data_passer);

	g_object_unref(parser);

	/*data_passer->csv_column_name_regex = g_regex_new("\"([^\"]+)\"",
			G_REGEX_DEFAULT, G_REGEX_MATCH_DEFAULT, &error);

	if (data_passer->csv_column_name_regex == NULL) {
		g_print(
				"Error creating regex: %s, will not be able to create the database",
				error->message);
		g_error_free(error);
	}*/

}

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

	g_strlcpy (data_passer->sqlite_prepare_dummies, "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,", MAX_NUMBER_TABLE_COLUMNS / 2);

	read_configuration_data(data_passer);
	return data_passer;
}
