/*
 * database_utilities.c
 *
 *  Created on: Feb 14, 2026
 *      Author: abba
 */
#include <glib.h>
#include <headers.h>

void get_table_name_from_csv_name(gchar table_name[], const gchar *csv_name) {
	memset(table_name, '\0', MAX_COLUMN_NAME_LENGTH);
	gchar *extension_start = g_strrstr (csv_name, ".csv");
	guint num_characters_in_table_name = extension_start - csv_name;
	strncpy( table_name, csv_name, num_characters_in_table_name);
	table_name[num_characters_in_table_name + 1] = '\0';

}

gboolean find_table_definition(gconstpointer data, gconstpointer user_data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic*) data;
	gchar *table_name = (gchar*) user_data;
	return strcmp(table_characteristic->table_name, table_name);
}
