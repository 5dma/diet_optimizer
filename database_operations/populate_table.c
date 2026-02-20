/*
 * populate_table.c
 *
 *  Created on: Feb 19, 2026
 *      Author: abba
 */
#include <stdio.h>
#include <glib.h>
#include <headers.h>
void populate_table(FILE *csv_file, const guint csv_start, gchar table_name[], Data_Passer *data_passer) {
	fseek(csv_file, csv_start, SEEK_SET);
	gchar line[MAX_CSV_FILE_LINE_LENGTH];
	GMatchInfo *match_info = NULL;
	gchar *match = NULL;

	gchar insert_statement[MAX_SQLITE_LENGTH];
	gchar *command_pointer = g_stpcpy(insert_statement, "INSERT INTO ");
	command_pointer = g_stpcpy(command_pointer, table_name);
	command_pointer = g_stpcpy(command_pointer, " VALUES (");
	gchar *command_pointer_start = command_pointer;
	gchar *errmsg = NULL;
	while (fgets(line, sizeof(line), csv_file) != NULL) {
			//g_print("%s\n", line);
			g_regex_match(data_passer->csv_column_value_regex, line, 0, &match_info);
			if (data_passer->error) {
				g_print("Error matching regex: %s\n", data_passer->error->message);
				g_error_free(data_passer->error);
				return;
			}
			command_pointer = command_pointer_start;
			do {
				match = g_match_info_fetch(match_info, 1);
				//g_print("Found match: %s\n", match);
				command_pointer = g_stpcpy(command_pointer, match);
				command_pointer = g_stpcpy(command_pointer, ",");
				g_free(match);
			} while (g_match_info_next(match_info, &(data_passer->error)));
			g_match_info_free(match_info);
			command_pointer = g_stpcpy(command_pointer - 1, ")");
			//g_print("%s\n", insert_statement);

			int rc = sqlite3_exec(data_passer->run_time.db, insert_statement, 0, 0,
					&errmsg);
			if (rc != SQLITE_OK) {
				g_print("SQL error: %s\n", errmsg);
				sqlite3_free(errmsg); // Free the error message if needed
			} else {
				//g_print("Row inserted successfully\n");
			}
		}

}
