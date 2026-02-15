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

void get_csv_files( gpointer data,  gpointer user_data) {
	Data_Passer *data_passer = (Data_Passer *) user_data;
	Table_Characteristic *table_characteristic = (Table_Characteristic *) data;

	gchar *filename = g_malloc(MAX_FILE_NAME_LENGTH);
	gchar *pointer = g_stpcpy (filename, data_passer->csv_file_directory);
	pointer = g_stpcpy (filename, table_characteristic->table_name);
	pointer = g_stpcpy (pointer,".csv");
	data_passer->run_time.csv_files = g_slist_append (data_passer->run_time.csv_files, filename);
}
