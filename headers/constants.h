#pragma once
/**
 * @file constants.h
 * @brief List of constants and associated values.
 *
 */

#ifndef HEADERS_CONSTANTS_H_  ///< Include guard to prevent multiple inclusions
#define HEADERS_CONSTANTS_H_

/**
 * @brief Path to the configuration file.
 *
 * This is the default location of the diet optimizer configuration file.
 */
#define CONFIG_FILE "/home/abba/.diet_optimizer/diet_optimizer.json"

/**
 * @brief Maximum line length for CSV files.
 *
 * This constant defines the maximum length of a line in a CSV file,
 * limiting to prevent overflow and ensure proper parsing.
 */
#define MAX_CSV_FILE_LINE_LENGTH 4096

/**
 * @brief Maximum length for file names.
 *
 * This constant defines the maximum length allowed for file names.
 */
#define MAX_FILE_NAME_LENGTH 256

/**
 * @brief Maximum length for column names.
 *
 * This constant defines the maximum length allowed for column names
 * in the application's data structure.
 */
#define MAX_COLUMN_NAME_LENGTH 255

/**
 * @brief Maximum length for SQLite statements.
 *
 * This constant defines the maximum length allowed for SQL statements
 * used within the application.
 */
#define MAX_SQLITE_LENGTH 4096

/**
 * @brief Maximum number of table columns.
 *
 * This constant represents the maximum number of columns allowed in
 * database tables managed by the application.
 */
#define MAX_NUMBER_TABLE_COLUMNS 100

/**
 * @brief Maximum length for foreign key phrases.
 *
 * This constant sets the maximum length for phrases used as foreign
 * keys in the application's database schema.
 */
#define MAX_FOREIGN_KEY_PHRASE 200

#endif /* HEADERS_CONSTANTS_H_ */
