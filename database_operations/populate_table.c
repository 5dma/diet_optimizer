/*
 * populate_table.c
 *
 *  Created on: Feb 19, 2026
 *      Author: abba
 */
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <headers.h>
void populate_table(FILE *csv_file, const guint csv_start, gchar table_name[],
		guint number_columns, GSList *table_columns, Data_Passer *data_passer) {

	fseek(csv_file, csv_start, SEEK_SET);
	gchar line[MAX_CSV_FILE_LINE_LENGTH];
	GMatchInfo *match_info = NULL;
	gchar *match = NULL;
	gchar dummy[] = "?,?,?,?,?,?,?,?,?,?,?,?,";
	gchar insert_statement[MAX_SQLITE_LENGTH];
	gchar *command_pointer = g_stpcpy(insert_statement, "INSERT INTO ");
	command_pointer = g_stpcpy(command_pointer, table_name);
	command_pointer = g_stpcpy(command_pointer, " VALUES (");
	guint number_dummies = 2 * (number_columns - 1) + 1;
	strncpy(command_pointer, dummy, number_dummies);
	command_pointer += number_dummies;
	command_pointer = g_stpcpy(command_pointer, ")");
	sqlite3_stmt *stmt;
	int barf = sqlite3_prepare_v2(data_passer->run_time.db, insert_statement,
			-1, &stmt, NULL);
	if (barf != SQLITE_OK) {
		g_print("Could not prepare the insert statement\n");
	}

	sqlite3_exec(data_passer->run_time.db, "BEGIN TRANSACTION;", NULL, NULL, NULL);


	while (fgets(line, sizeof(line), csv_file) != NULL) {
		//g_print("%s\n", line);
		g_regex_match(data_passer->csv_column_name_regex, line, 0,
				&match_info);
		if (data_passer->error) {
			g_print("Error matching regex: %s\n", data_passer->error->message);
			g_error_free(data_passer->error);
			return;
		}

		guint bind_index = 1;
		gint bind_return;
		do {
			match = g_match_info_fetch(match_info, 1);
			//g_print("Found match: %s\n", match);
			GSList *table_column = g_slist_nth(table_columns, bind_index - 1);
			Column_Definition *column_definition =
					(Column_Definition*) table_column->data;
			switch (column_definition->column_type) {
			case INTEGER:
				bind_return = sqlite3_bind_int64(stmt, bind_index, atoi(match));
				if (bind_return != SQLITE_OK) {
					g_print("Execution failed: %s\n",
							sqlite3_errmsg(data_passer->run_time.db));
				}
				break;

			case REAL:
				bind_return = sqlite3_bind_double(stmt, bind_index, atof(match));
				if (bind_return != SQLITE_OK) {
					g_print("Execution failed: %s\n",
							sqlite3_errmsg(data_passer->run_time.db));
				}
				break;

			case TEXT:
				bind_return = sqlite3_bind_text(stmt, bind_index, match, -1,
				SQLITE_TRANSIENT);
				if (bind_return != SQLITE_OK) {
					g_print("Execution failed: %s\n",
							sqlite3_errmsg(data_passer->run_time.db));
				}
				break;

			default:
				g_print("We have a problem\n");

			}
			bind_index++;
			g_free(match);
		} while (g_match_info_next(match_info, &(data_passer->error)));
		int rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			g_print("Execution failed: %s\n",
					sqlite3_errmsg(data_passer->run_time.db));
		}
		rc = sqlite3_reset(stmt);
		if (rc != SQLITE_OK) {
			g_print("Reset failed: %s\n",
					sqlite3_errmsg(data_passer->run_time.db));
		}

		g_match_info_free(match_info);

	}
	sqlite3_finalize(stmt);
	sqlite3_exec(data_passer->run_time.db, "COMMIT;", NULL, NULL, NULL);


}
