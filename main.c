#include <headers.h>
#include <glib.h>

/**
 * @file main.c
 * @brief Main entry point for the application.
 *
 * This program initializes the data passer and performs various setup tasks,
 * reading configuration from disk, and freeing memory.
 */

/**
 * @brief Entry point of the application.
 *
 * This function handles the startup sequence by initializing the data passer,
 * checking for the existence of a data directory, starting the logger, and
 * opening the database. In case of any failure, it performs cleanup and exits
 * with an error code.
 *
 * @return int Returns 0 on success, or 1 on failure.
 */
int main(void) {
	/* Initialize data_passer and read configuration from disk. */
	Data_Passer *data_passer = startup();
	if (!data_passer) {
		return EXIT_FAILURE;
	}
	if (!exists_data_directory(data_passer)) {
		cleanup(data_passer);
		return EXIT_FAILURE;
	}
	/* Start up the logging. */
	if (start_logger(data_passer) == FALSE) {
		cleanup(data_passer);
		return EXIT_FAILURE;
	}

	write_log_message(G_LOG_LEVEL_INFO, "Application started",
			data_passer->run_time.log_file);
	/* Open the database, or create a new one if it doesn't exist. */
	if (open_database(data_passer) == FALSE) {
		cleanup(data_passer);
		return EXIT_FAILURE;
	}

	/* Free memory. */
	cleanup(data_passer);

	return EXIT_SUCCESS;
}

