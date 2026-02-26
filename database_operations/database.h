/**
 * @file database.h
 * @brief Prototypes for functions.
 *
 */
#include <json-glib/json-glib.h>
#include "../headers/headers.h"

#ifndef DATABASE_OPERATIONS_DATABASE_H_
#define DATABASE_OPERATIONS_DATABASE_H_  ///< Include guard to prevent multiple inclusions

gboolean open_database(Data_Passer *data_passer);
void do_sqlite_tests(const gchar *csv_value, gpointer user_data);

gboolean is_integer(const gchar *token);
gboolean is_real(const gchar *token);
gboolean is_null(const gchar *token);
void get_table_characteristics(JsonObject *data_object, Data_Passer *data_passer);
void get_table_name_from_csv_name(gchar table_name[], const gchar *csv_name);
gboolean find_table_definition(gconstpointer data, gconstpointer user_data);
void make_foreign_keys(gpointer data, gpointer user_data);
gboolean find_table_definition_with_foreign_key(gconstpointer data, gconstpointer user_data);

void populate_table(FILE *csv_file, const guint csv_start, gchar table_name[], guint number_columns, GSList *table_columns, Data_Passer *data_passer);

/* Toss this function? */
gboolean is_table_exists (const char *table_name, const Data_Passer *data_passer);
#endif /* DATABASE_OPERATIONS_DATABASE_H_ */
