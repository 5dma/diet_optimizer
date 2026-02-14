/*
 * get_table_characteristcs.c
 *
 *  Created on: Feb 12, 2026
 *      Author: abba
 */
#include <glib-2.0/glib.h>
#include <json-glib/json-glib.h>
#include <headers.h>

void read_table_characteristic(JsonArray* array, guint index, JsonNode* element_node, gpointer user_data) {
	JsonObject *object = json_node_get_object(element_node);
	Data_Passer *data_passer = (Data_Passer *)user_data;
	Table_Characteristic *table_characteristics = g_malloc(sizeof(Table_Characteristic));
	table_characteristics->table_name = NULL;
	table_characteristics->primary_key = NULL;
	table_characteristics->table_name = g_strdup (json_object_get_string_member (object, "table_name"));
	table_characteristics->primary_key = g_strdup (json_object_get_string_member (object, "primary_key"));
	data_passer->table_characteristics = g_slist_append(data_passer->table_characteristics, table_characteristics);
	g_print("%s %s\n",table_characteristics->table_name, table_characteristics->primary_key);


}

void get_table_characteristics(JsonObject *data_object, Data_Passer *data_passer){
	JsonArray *table_characteristics_array= json_object_get_array_member (data_object, "table_characteristics");
	json_array_foreach_element (table_characteristics_array, read_table_characteristic, data_passer);

}
