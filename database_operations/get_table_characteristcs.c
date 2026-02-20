/*
 * get_table_characteristcs.c
 *
 *  Created on: Feb 12, 2026
 *      Author: abba
 */
#include <glib-2.0/glib.h>
#include <json-glib/json-glib.h>
#include <headers.h>

void read_foreign_keys (JsonArray *array, guint index,
JsonNode *element_node, gpointer user_data) {
	JsonObject *object = json_node_get_object(element_node);
	Table_Characteristic *table_characteristics = (Table_Characteristic *)user_data;
	Foreign_Key *foreign_key = g_malloc(
				sizeof(Foreign_Key));
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
		g_print("I found a primary key");
		table_characteristics->primary_key = g_strdup(
				json_object_get_string_member(object, "primary_key"));
		g_print("%s %s\n", table_characteristics->table_name,
				table_characteristics->primary_key);
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

void get_table_characteristics(JsonObject *data_object,
		Data_Passer *data_passer) {
	JsonArray *table_characteristics_array = json_object_get_array_member(
			data_object, "table_characteristics");
	json_array_foreach_element(table_characteristics_array,
			read_table_characteristic, data_passer);

}
