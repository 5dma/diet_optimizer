#include <headers.h>
#include <stddef.h>

void cleanup(Data_Passer *data_passer) {
    if (data_passer->data_directory) {
        g_free(data_passer->data_directory);
    }
    if (data_passer->database_filename) {
        g_free(data_passer->database_filename);
    }
    if (data_passer->log_filename) {
        g_free(data_passer->log_filename);
    }
    if (data_passer->csv_file_directory) {
        g_free(data_passer->csv_file_directory);
    }

	if (data_passer->run_time.log_file) {
		fclose(data_passer->run_time.log_file);
	}
	if (data_passer->run_time.csv_files) {
		g_slist_free_full (data_passer->run_time.csv_files, g_free);
	}
	if (data_passer->run_time.db) {
		sqlite3_close(data_passer->run_time.db);
	}

    g_free(data_passer);

}
