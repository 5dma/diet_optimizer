#include <math.h>
#include <headers.h>

/**
 * @file sqlite_tests.c
 * @brief Statements for determining the SQLITE data type of a character string.
 */

/**
 * Determines the SQLite data type of a passed value read from the CSV file. The passed value is a `gchar *`, and the objective is to determine the smallest data type that value represents. For example, a value `12` corresponds to the SQLITE `INTEGER`.
 *
 * The field tests go from smallest to largest. For example, the first test checks if the value is a `TEXT`. If not that, the next check is for `SMALLINT_UNSIGNED`, and so on. If all tests fail, the value is assumed to be `CHAR(N)`, where `N` is the value's length.
 *
 * The field tests are cumulative such that a field can never return to a smaller data type. For example, once a field is determined to be `BIGINT_SIGNED`, it can never go back to `TINYINT_UNSIGNED` even if all subsequent values are indeed `TINYINT_UNSIGNED`.
 *
 * @see [Datatypes In SQLite](https://sqlite.org/datatype3.html)
 *
 * @param csv_value Value in the CSV file whose SQLite type we want to determine.
 * @param field_analysis The column's current Field_analysis_sqlite.
 * @param data_passer Pointer to the data-passer structure.
 *  */
void do_sqlite_tests(const gchar *csv_value, gpointer user_data) {
	Column_Definition *table_column = (Column_Definition *)user_data;
	gboolean passes_test = FALSE;
	switch (table_column->column_type) {
		case TRASH: /* TRASH is a field's initial data type, basically uninitialized. */
			passes_test = is_null(csv_value);
			if (passes_test) {
				table_column->column_type = NULL_S;
				break;
			}
			passes_test = is_integer(csv_value);
			if (passes_test) {
				table_column->column_type = INTEGER;
				break;
			}
			passes_test = is_real(csv_value);
			if (passes_test) {
				table_column->column_type = REAL;
				break;
			}
			table_column->column_type = TEXT;
			break;
		case NULL_S:
			passes_test = is_null(csv_value);
			if (passes_test) break;
			passes_test = is_integer(csv_value);
			if (passes_test) {
				table_column->column_type = INTEGER;
				break;
			}
			passes_test = is_real(csv_value);
			if (passes_test) {
				table_column->column_type = REAL;
				break;
			}
			table_column->column_type = TEXT;
			break;
		case INTEGER:
			/* If the column is already an integer
				and we received a null value, it's still an integer. */
			passes_test = is_integer(csv_value) || is_null(csv_value);
			if (passes_test) break;
			passes_test = is_real(csv_value);
			if (passes_test) {
				table_column->column_type = REAL;
				break;
			}
			table_column->column_type = TEXT;
			break;
		case REAL:
			/* If the column is already a real
				and we received a null value, it's still real. */
			passes_test = is_real(csv_value) || is_null(csv_value);
			if (passes_test) break;
			table_column->column_type = TEXT;
			break;
		default:
			table_column->column_type = TEXT;
	}


}
