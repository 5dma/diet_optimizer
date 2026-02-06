
#include <headers.h>
#include <glib.h>
int main(void)
{
	Data_Passer *data_passer = startup();
	if (!exists_data_directory(data_passer)) {
		cleanup(data_passer);
		return 1;
	}
	if (start_logger(data_passer) == FALSE) {
		cleanup(data_passer);
		return 1;
	}

	if (open_database(data_passer) == FALSE) {
		cleanup(data_passer);
		return 1;
	}
	cleanup(data_passer);
	return 0;
}

