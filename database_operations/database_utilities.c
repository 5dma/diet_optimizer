/**
 * @file database_utilities.c
 * @brief Utility functions used by other functions to use the SQLite database.
 *
 * This file contains functions for extracting table names from CSV filenames,
 * finding table definitions, and managing foreign keys for SQLite tables.
 */
#include <glib.h>
#include <headers.h>

/**
 * @brief Extracts the table name from a CSV filename.
 *
 * This function takes a CSV filename and removes the ".csv" extension,
 * storing the result in the provided table_name array.
 *
 * @param table_name An array where the extracted table name will be stored.
 * @param csv_name The CSV filename from which the table name is derived.
 */
void get_table_name_from_csv_name(gchar table_name[], const gchar *csv_name) {
	memset(table_name, '\0', MAX_COLUMN_NAME_LENGTH);
	gchar *extension_start = g_strrstr(csv_name, ".csv");
	guint num_characters_in_table_name = extension_start - csv_name;
	strncpy(table_name, csv_name, num_characters_in_table_name);
	table_name[num_characters_in_table_name + 1] = '\0';

}

/**
 * @brief Finds a table definition within a GSList of table definitions.
 *
 * This callback function provides the matching condition for finding
 * a table definition in a GSList of table definitions. If the candidate
 * table definition has the same name as the passed table name, then
 * a match is found.
 *
 * @param data A pointer to the Table_Characteristic structure being checked.
 * @param user_data A pointer to the table name to compare against.
 * @return TRUE if the table name matches, FALSE otherwise.
 * @sa make_create_command
 */
gboolean find_table_definition(gconstpointer data, gconstpointer user_data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic*) data;
	gchar *table_name = (gchar*) user_data;
	return strcmp(table_characteristic->table_name, table_name);
}


/**
 * @brief Finds a table definition containing foreign keys within a GSList of table definitions.
 *
 * This callback function provides the matching condition for finding
 * a table definition in a GSList of table definitions. If the candidate
 * table definition has the same name as the passed table name, _and_ if the
 * table definition contains clauses for foreign keys, then
 * a match is found.
 *
 * @param data A pointer to the Table_Characteristic structure being checked.
 * @param user_data A pointer to the table name to compare against.
 * @return TRUE if the table definition matches and has foreign keys, FALSE otherwise.
 * @sa make_create_command
 */
gboolean find_table_definition_with_foreign_key(gconstpointer data, gconstpointer user_data) {
	Table_Characteristic *table_characteristic = (Table_Characteristic*) data;
	gchar *table_name = (gchar*) user_data;
	if ((strcmp(table_characteristic->table_name, table_name) == 0)
			&& (table_characteristic->foreign_keys != NULL)) {
		return 0;
	}
	return 1;
}

/**
 * @brief Constructs foreign key definitions for an SQL `CREATE TABLE` command.
 *
 * This callback function assembles `FOREIGN KEY` clauses from
 * the GSList of foreign keys associated with a particular column definition.
 * See [foreign-key-clause](https://www.sqlite.org/syntax/foreign-key-clause.html)
 * for the syntax of an SQLite foreign key clause.
 *
 * @param data A pointer to a Foreign_Key structure containing the foreign key details.
 * @param user_data A pointer to the command string being constructed.
 * @sa make_create_command
 */
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
