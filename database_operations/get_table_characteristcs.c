/**
@file get_table_characteristcs.c

@brief Functions to read table characteristics from JSON data.

This file contains functions to parse JSON data to retrieve table characteristics,
including foreign keys and primary keys, storing the results in an appropriate format. */
#include <glib-2.0/glib.h>
#include <json-glib/json-glib.h>
#include <headers.h>


/**
 * @brief Reads foreign key information from a JSON array.
 *
 * This callback function extracts foreign key details from a JSON object and appends
 * them to the specified Table_Characteristic.
 *
 * @param array The JSON array containing foreign keys.
 * @param index The current index of the foreign key in the JSON array.
 * @param element_node The current JSON node being processed.
 * @param user_data A pointer to the Table_Characteristic to which the foreign key will be added.
 * @sa read_table_characteristic
 */
void read_foreign_keys(JsonArray *array, guint index, JsonNode *element_node,
		gpointer user_data) {
	JsonObject *object = json_node_get_object(element_node);
	Table_Characteristic *table_characteristics =
			(Table_Characteristic*) user_data;
	Foreign_Key *foreign_key = g_malloc(sizeof(Foreign_Key));
	foreign_key->local_column = NULL;
	foreign_key->foreign_table = NULL;
	foreign_key->foreign_column = NULL;
	foreign_key->local_column = g_strdup(
			json_object_get_string_member(object, "local_column"));
	foreign_key->foreign_table = g_strdup(
			json_object_get_string_member(object, "foreign_table"));
	foreign_key->foreign_column = g_strdup(
			json_object_get_string_member(object, "foreign_column"));
	table_characteristics->foreign_keys = g_slist_append(
			table_characteristics->foreign_keys, foreign_key);
}

/**
 * @brief Reads table characteristics from a JSON array.
 *
 * This callback function extracts table name and optional primary key information from a
 * JSON object and appends the new Table_Characteristic to a GSList of
 * table characteristics in the Data_Passer. If foreign keys are present, it processes
 * them as well.
 *
 * @param array The JSON array containing table characteristics.
 * @param index The current index of the table characteristic in the JSON array.
 * @param element_node The current JSON node being processed.
 * @param user_data A pointer to the Data_Passer structure for storing the characteristics.
 * @sa get_table_characteristics
 */
void read_table_characteristic(JsonArray *array, guint index,
		JsonNode *element_node, gpointer user_data) {
	JsonObject *object = json_node_get_object(element_node);
	Data_Passer *data_passer = (Data_Passer*) user_data;
	Table_Characteristic *table_characteristics = g_malloc(
			sizeof(Table_Characteristic));
	table_characteristics->table_name = NULL;
	table_characteristics->primary_key = NULL;
	table_characteristics->foreign_keys = NULL;
	table_characteristics->table_name = g_strdup(
			json_object_get_string_member(object, "table_name"));

	if (json_object_has_member(object, "primary_key")) {

		table_characteristics->primary_key = g_strdup(
				json_object_get_string_member(object, "primary_key"));
	}
	data_passer->table_characteristics = g_slist_append(
			data_passer->table_characteristics, table_characteristics);

	if (json_object_has_member(object, "foreign_keys")) {
		JsonArray *foreign_keys_array = json_object_get_array_member(object,
				"foreign_keys");
		json_array_foreach_element(foreign_keys_array, read_foreign_keys,
				table_characteristics);
	}
}


/**
 * @brief Retrieves all table characteristics from a JSON object.
 *
 * This function extracts table characteristics from a JSON object and appends
 * them to the Data_Passer. It processes each element in the JSON array
 * of table characteristics.
 *
 * @param data_object The JSON object containing table characteristics.
 * @param data_passer A pointer to the Data_Passer structure for storing the characteristics.
 * @sa read_table_characteristic
 * @sa read_foreign_keys
 */
void get_table_characteristics(JsonObject *data_object,
		Data_Passer *data_passer) {
	JsonArray *table_characteristics_array = json_object_get_array_member(
			data_object, "table_characteristics");
	json_array_foreach_element(table_characteristics_array,
			read_table_characteristic, data_passer);

}
