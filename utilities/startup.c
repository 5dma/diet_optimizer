#include <headers.h>
#include <stddef.h>
#include <json-glib/json-glib.h>
/*
 * startup.c
 *
 *  Created on: Feb 4, 2026
 *      Author: abba
 */


void read_configuration_data(Data_Passer* data_passer) {
	JsonParser* parser;
	GError* error = NULL;

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

	/* Read the database file information. */

	JsonObject *database_object = json_object_get_object_member(root_object, "database");

	g_free(data_passer->database_filename);
	const gchar *barf = json_object_get_string_member(database_object, "database_directory");
	data_passer->database_filename = g_strdup(barf);
	//data_passer->database_filename = g_strdup(json_object_get_string_member(database_object, "database_filename"));

	g_object_unref(parser);
}

Data_Passer * startup() {
	Data_Passer *data_passer = g_malloc(sizeof(Data_Passer));
	data_passer->database_directory = NULL;
	data_passer->database_filename = NULL;
	data_passer->csv_file_directory= NULL;

	read_configuration_data(data_passer);
	return data_passer;
}
