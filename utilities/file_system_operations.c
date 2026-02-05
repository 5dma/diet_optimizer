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
