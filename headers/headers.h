#pragma once
#include <sqlite3.h>
#include <structures.h>
#include <constants.h>
#include "../database_operations/database.h"
#include "../utilities/utilities.h"
#include <glib.h>


#ifndef HEADERS_HEADERS_H_
#define HEADERS_HEADERS_H_

void make_table(gpointer filename, gpointer user_data);

#endif /* HEADERS_HEADERS_H_ */
