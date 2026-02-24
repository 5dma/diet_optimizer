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
	gchar *extension_start = g_strrstr(csv_name, ".csv");
	guint num_characters_in_table_name = extension_start - csv_name;
	strncpy(table_name, csv_name, num_characters_in_table_name);
	table_name[num_characters_in_table_name + 1] = '\0';

}

gboolean find_table_definition(gconstpointer data, gconstpointer user_data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic*) data;
	gchar *table_name = (gchar*) user_data;
	return strcmp(table_characteristic->table_name, table_name);
}


gboolean find_table_definition_with_foreign_key(gconstpointer data, gconstpointer user_data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic*) data;
	gchar *table_name = (gchar*) user_data;
	if ((strcmp(table_characteristic->table_name, table_name) == 0)
			&& (table_characteristic->foreign_keys != NULL)) {
		return 0;
	}
	return 1;
}

void make_foreign_keys(gpointer data, gpointer user_data) {
	gchar foreign_key_phrase[MAX_FOREIGN_KEY_PHRASE];
	Foreign_Key *foreign_key = (Foreign_Key*) data;
	gchar **command_pointer = (gchar**) user_data;
	g_snprintf(foreign_key_phrase, MAX_FOREIGN_KEY_PHRASE,
			" FOREIGN KEY (%s) REFERENCES %s(%s),", foreign_key->local_column,
			foreign_key->foreign_table, foreign_key->foreign_column);
	*command_pointer = g_stpcpy(*command_pointer, foreign_key_phrase);
}

/* Toss this function? */
gboolean is_table_exists (const char *table_name, const Data_Passer *data_passer) {

    sqlite3_bind_text(data_passer->stmt_is_table_exists, 1, table_name, -1, SQLITE_STATIC);

    if (sqlite3_step(data_passer->stmt_is_table_exists) == SQLITE_ROW) {
        return (sqlite3_column_int(data_passer->stmt_is_table_exists, 0) == 1); // 1 if found, 0 if not
    }

    return FALSE;
}
