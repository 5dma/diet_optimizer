#include <headers.h>
#include <glib.h>
#include <glib/gstdio.h>

gboolean exists_data_directory(Data_Passer *data_passer) {
	if (g_file_test(data_passer->data_directory, G_FILE_TEST_IS_DIR)) {
		return TRUE;
	}
	// On GUI, ask if want to create the directory. If use says no, return false
	if (g_mkdir(data_passer->data_directory, 0755) == 0) {

		return TRUE;
	} else {
		// On GUI, say could not make data directory, exited
		return FALSE;
	}
}

GSList *get_csv_files(Data_Passer *data_passer) {

	GError *error = NULL;
	GDir *dir= NULL;
	GSList csv_file_list;
	dir = g_dir_open (data_passer->csv_file_directory, 0, &error);
	if (dir == NULL) {
		write_log_message(G_LOG_LEVEL_ERROR, "Could not open the CSV directory.",
								data_passer->run_time.log_file);
		g_error_free(error);
		return NULL;
	}
	const gchar *filename = g_dir_read_name (dir);
	while (filename != NULL) {
		g_print("%s\n", filename);
		filename = g_dir_read_name (dir);
	}
	return &csv_file_list;

}
