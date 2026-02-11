/*
 * headers.h
 *
 *  Created on: Feb 5, 2026
 *      Author: abba
 */
#include "../headers/headers.h"

#ifndef DATABASE_OPERATIONS_DATABASE_H_
#define DATABASE_OPERATIONS_DATABASE_H_

gboolean open_database(Data_Passer *data_passer);
void do_sqlite_tests(const gchar *csv_value, Column_Definition *table_column);

gboolean is_integer(const gchar *token);
gboolean is_real(const gchar *token);
gboolean is_null(const gchar *token);

#endif /* DATABASE_OPERATIONS_DATABASE_H_ */
