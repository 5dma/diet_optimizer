#include <glib.h>
#include <stdio.h>
#include "utilities.h"
#define LOG_LEVEL_LENGTH 50

void barf(GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
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

gboolean start_logger(Data_Passer *data_passer) {

	gchar log_file[256];
	gchar *pointer = g_stpcpy(log_file, data_passer->data_directory);
	pointer = g_stpcpy(pointer, data_passer->log_filename);
	if ((data_passer->run_time.log_file = fopen(log_file, "a")) == NULL) {
		g_print("Could not open file, no logging will be done\n");
		return FALSE;
	}

	const gchar message[] = "Hello there";

	barf(G_LOG_LEVEL_CRITICAL, message, data_passer->run_time.log_file);
	barf(G_LOG_LEVEL_WARNING, message, data_passer->run_time.log_file);
	barf(G_LOG_LEVEL_MESSAGE, message, data_passer->run_time.log_file);
	barf(G_LOG_LEVEL_INFO, message, data_passer->run_time.log_file);
	barf(G_LOG_LEVEL_DEBUG, message, data_passer->run_time.log_file);

	return TRUE;
}
