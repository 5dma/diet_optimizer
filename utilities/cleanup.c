#include <headers.h>


void cleanup(Data_Passer *data_passer) {
	if (!data_passer->database_directory) {
		g_free(data_passer->database_directory);
	}
	if (!data_passer->database_filename) {
		g_free(data_passer->database_filename);
	}
	if (!data_passer->csv_file_directory) {
		g_free(data_passer->csv_file_directory);
	}

	g_free(data_passer);

}
