#include <glib.h>
#include <stdio.h>
#include "utilities.h"
/**

    @file logging.c
    @brief Functions for logging messages to a log file.
    This file contains functions to write log messages at various log levels
    and to initialize the logging system by opening a log file for output. */


/**
 * @brief Maximal length of a log level literal.
 *
 * This constant defines the maximal length of a log level
 * (`ERROR`, `CRITICAL`, `WARNING`, etc.)
 */
#define LOG_LEVEL_LENGTH 50 ///< Maximal length of a log level (`ERROR`, `CRITICAL`, `WARNING`, etc.)

/**
 * @brief Writes a log message to the specified output stream.
 *
 * This function formats and sends a log message to the output stream,
 * prefixed with the corresponding log level and a timestamp. The log
 * levels include ERROR, CRITICAL, WARNING, MESSAGE, INFO, and DEBUG.

	Examples for calling this function:
	- `write_log_message(G_LOG_LEVEL_CRITICAL, message, data_passer->run_time.log_file);`
	- `write_log_message(G_LOG_LEVEL_WARNING, message, data_passer->run_time.log_file);`
	- `write_log_message(G_LOG_LEVEL_MESSAGE, message, data_passer->run_time.log_file);`
	- `write_log_message(G_LOG_LEVEL_INFO, message, data_passer->run_time.log_file);`
	- `write_log_message(G_LOG_LEVEL_DEBUG, message, data_passer->run_time.log_file);`


 *
 * @param log_level The severity level of the log message, represented as
 *                  GLogLevelFlags.
 * @param message The message string to log.
 * @param user_data A pointer to user-defined data, which is expected to be a
 *                  file stream (e.g., a FILE pointer).
 *
 * @note The function assumes that the user_data pointer is valid and refers
 *       to an open file stream. The format for log messages is `%-8s %%s %%s\n`
 *       where the first `%%s` is the log level, the second `%%s` is the timestamp,
 *       and the third `%%s` is the log message.
 */
void write_log_message(GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
	FILE *stream = (FILE*) user_data;

	gchar my_log_level[LOG_LEVEL_LENGTH + 1];

	switch (log_level) {
	case G_LOG_LEVEL_ERROR:
		g_strlcpy(my_log_level, "ERROR", LOG_LEVEL_LENGTH);
		break;
	case G_LOG_LEVEL_CRITICAL:
		g_strlcpy(my_log_level, "CRITICAL", LOG_LEVEL_LENGTH);
		break;
	case G_LOG_LEVEL_WARNING:
		g_strlcpy(my_log_level, "WARNING", LOG_LEVEL_LENGTH);
		break;
	case G_LOG_LEVEL_MESSAGE:
		g_strlcpy(my_log_level, "MESSAGE", LOG_LEVEL_LENGTH);
		break;
	case G_LOG_LEVEL_INFO:
		g_strlcpy(my_log_level, "INFO", LOG_LEVEL_LENGTH);
		break;
	case G_LOG_LEVEL_DEBUG:
		g_strlcpy(my_log_level, "DEBUG", LOG_LEVEL_LENGTH);
		break;
	default:
		g_strlcpy(my_log_level, "MESSAGE", LOG_LEVEL_LENGTH);
		break;
	}

	GDateTime *date_time = g_date_time_new_now_local();
	gchar *date_time_string = g_date_time_format(date_time, "%H:%M:%S");
	fprintf(stream, "%-8s %s %s\n", my_log_level, date_time_string, message);
	fflush(stream);
	g_free(date_time_string);
	g_date_time_unref(date_time);
}


/**
 * @brief Initializes the logger by opening the log file for appending.
 *
 * This function constructs the file path for the log file using the
 * data directory and filename from the `Data_Passer` structure. It attempts
 * to open the log file in append mode. If successful, logging can proceed;
 * otherwise, it prints an error message and returns FALSE.
 *
 * @param data_passer A pointer to a Data_Passer structure that contains the
 *                    data directory and log filename.
 *
 * @return gboolean Returns `TRUE` if the log file opened successfully,
 *                  or `FALSE` if it failed to open.
 */
gboolean start_logger(Data_Passer *data_passer) {

	gchar log_file[256];
	gchar *pointer = g_stpcpy(log_file, data_passer->data_directory);
	pointer = g_stpcpy(pointer, data_passer->log_filename);
	if ((data_passer->run_time.log_file = fopen(log_file, "a")) == NULL) {
		g_print("Could not open file, no logging will be done\n");
		return FALSE;
	}

	return TRUE;
}
