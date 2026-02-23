/*
 * create_tables.c
 *
 *  Created on: Feb 5, 2026
 *      Author: abba
 */
#include <glib.h>
#include <headers.h>
#include <string.h>

gchar* make_csv_filename(const gchar *directory, const gchar *csv_file) {
	return g_strconcat(directory, csv_file, NULL);
}

void free_column_info(gpointer data) {
	g_free((Column_Definition*) data);
}

void make_clause(gpointer data, gpointer user_data) {
	Column_Definition *column_definition = (Column_Definition*) data;
	//g_print("Field name %s\n", column_definition->column_name );
	gchar **command_pointer = (gchar**) user_data;
	//gchar *command_pointer = *step1;
	*command_pointer = g_stpcpy(*command_pointer,
			column_definition->column_name);
	*command_pointer = g_stpcpy(*command_pointer, " ");
	switch (column_definition->column_type) {
	case NULL_S:
		*command_pointer = g_stpcpy(*command_pointer, "NULL");
		break;
	case INTEGER:
		*command_pointer = g_stpcpy(*command_pointer, "INTEGER");
		break;
	case REAL:
		*command_pointer = g_stpcpy(*command_pointer, "REAL");
		break;
	case TEXT:
		*command_pointer = g_stpcpy(*command_pointer, "TEXT");
		break;
	default:

	}
	if (column_definition->is_primary_key) {
		*command_pointer = g_stpcpy(*command_pointer, " PRIMARY KEY");
	}
	*command_pointer = g_stpcpy(*command_pointer, ", ");
}

/*
 * CREATE TABLE barf (id INTEGER, desc TEXT, gag REAL, omg FLOAT);
 */
gchar* make_create_command(const gchar *table_name, GSList *table_columns,
		Data_Passer *data_passer) {
	gchar *create_command = g_malloc(sizeof(gchar) * MAX_SQLITE_LENGTH);
	gchar *command_pointer = g_stpcpy(create_command, "CREATE TABLE ");
	command_pointer = g_stpcpy(command_pointer, table_name);
	command_pointer = g_stpcpy(command_pointer, " (");

	g_slist_foreach(table_columns, make_clause, &command_pointer);

	/* Find the record containing the current table, check if it has foreign keys. */
	GSList *table_list = g_slist_find_custom(data_passer->table_characteristics,
			table_name, find_table_definition_with_foreign_key);

	if (table_list) {
		Table_Characteristic *table_characteristic =
				(Table_Characteristic*) table_list->data;

		command_pointer = g_strrstr(create_command, ",") + 1;
		g_slist_foreach(table_characteristic->foreign_keys, make_foreign_keys,
				command_pointer);
		command_pointer = g_strrstr(create_command, ",");
		command_pointer = g_stpcpy(command_pointer, ")");
	} else {
		/* For tables without a foreign key, replace trailing comma-space with a closing paren */
		command_pointer = g_strrstr(create_command, ", ");
		command_pointer = g_stpcpy(command_pointer, ")");
	}

	return create_command;
}
void execute_create_table_command(const gchar *create_command,
		Data_Passer *data_passer) {
	g_print("%s\n", create_command);
	gchar *errmsg = NULL;
	int rc = sqlite3_exec(data_passer->run_time.db, create_command, 0, 0,
			&errmsg);
	if (rc != SQLITE_OK) {
		g_print("SQL error: %s\n", errmsg);
		sqlite3_free(errmsg); // Free the error message if needed
	}
}

void make_table(gpointer filename, gpointer user_data) {
	Data_Passer *data_passer = (Data_Passer*) user_data;
	gchar *csv_filename = (gchar*) filename;

	gchar *csv_pathname = make_csv_filename(data_passer->csv_file_directory,
			csv_filename);
	g_print("Processing %s\n", (gchar*) csv_pathname);

	FILE *file = fopen(csv_pathname, "r");
	if (!file) {
		g_print("Error opening csv file %s\n", (gchar*) csv_pathname);
		g_free(csv_pathname);
		return;
	}
	gchar line[MAX_CSV_FILE_LINE_LENGTH];
	fgets(line, sizeof(line), file);
	/* Save the start of the CSV data, use it when populating the table. */
	guint csv_start = strlen(line);
	GMatchInfo *match_info = NULL;
	data_passer->error = NULL;

	g_regex_match(data_passer->csv_column_name_regex, line, 0, &match_info);
	if (data_passer->error) {
		g_print("Error matching regex: %s\n", data_passer->error->message);
		g_error_free(data_passer->error);
		data_passer->error = NULL;
		return;
	}
	guint number_columns = 0;
	GSList *table_columns = NULL;
	gchar *match = NULL;
	gchar table_name[MAX_COLUMN_NAME_LENGTH];
	/* Parse the column names appearing in the first row */
	do {
		match = g_match_info_fetch(match_info, 1); // Fetch the first capturing group
		if (match) {
			g_print("Found a column with title: %s\n", match);
			normalize_string(match);
			Column_Definition *column_definition = g_malloc(
					sizeof(Column_Definition));
			g_strlcpy(column_definition->column_name, match,
			MAX_COLUMN_NAME_LENGTH);
			column_definition->column_type = TRASH;
			column_definition->is_primary_key = FALSE;

			get_table_name_from_csv_name(table_name, csv_filename);
			GSList *table_list = g_slist_find_custom(
					data_passer->table_characteristics, table_name,
					find_table_definition);
			if (table_list) {
				Table_Characteristic *table_characteristic =
						(Table_Characteristic*) table_list->data;
				if ((table_characteristic->primary_key)
						&& strcmp(table_characteristic->primary_key,
								column_definition->column_name) == 0) {
					column_definition->is_primary_key = TRUE;
				}
			}

			table_columns = g_slist_append(table_columns, column_definition);
			number_columns++;

			g_free(match);
		}

	} while (g_match_info_next(match_info, &(data_passer->error)));

	g_match_info_free(match_info);
	/* Read from the second to last line, deciphering the SQLlite data type. */
	guint current_column;
	while (fgets(line, sizeof(line), file) != NULL) {
		//g_print("%s\n", line);

		g_regex_match(data_passer->csv_column_name_regex, line, 0, &match_info);
		if (data_passer->error) {
			g_print("Error matching regex: %s\n", data_passer->error->message);
			g_error_free(data_passer->error);
			data_passer->error = NULL;
			return;
		}

		current_column = 0;
		do {
			match = g_match_info_fetch(match_info, 1);
			//g_print("Found match: %s\n", match);
			do_sqlite_tests(match,
					g_slist_nth_data(table_columns, current_column));
			g_free(match);
			current_column++;
		} while (g_match_info_next(match_info, &(data_passer->error)));
		g_match_info_free(match_info);
	}

	gchar *create_command = make_create_command(table_name, table_columns,
			data_passer);
	g_print("%s\n", create_command);
	execute_create_table_command(create_command, data_passer);
	g_free(create_command);
	populate_table(file, csv_start, table_name, number_columns, table_columns, data_passer);
	g_slist_free_full(table_columns, free_column_info);
	fclose(file);
	g_free(csv_pathname);
}

