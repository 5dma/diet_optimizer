/**
 * @file utilities.h
 * @brief Prototypes for functions.
 *
 */

#include "../headers/headers.h"

#ifndef UTILITIES_UTILITIES_H_
#define UTILITIES_UTILITIES_H_  ///< Include guard to prevent multiple inclusions

void cleanup(Data_Passer *data_passer);
Data_Passer * startup();
gboolean start_logger(Data_Passer *data_passer);
gboolean exists_data_directory(Data_Passer *data_passer);
void write_log_message(GLogLevelFlags log_level, const gchar *message, gpointer user_data);
void get_csv_files( gpointer data,  gpointer user_data);
void normalize_string(char *str);
#endif /* UTILITIES_UTILITIES_H_ */
