
#include <headers.h>
#include <glib.h>
int main(void)
{
	Data_Passer *data_passer = startup();
	if (!exists_data_directory(data_passer)) {
		cleanup(data_passer);
	}
	if (start_logger(data_passer) == FALSE) {
		cleanup(data_passer);
	}

	sqlite3 *create_database();
	cleanup(data_passer);
	return 0;
}

