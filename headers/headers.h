#pragma once
#include <sqlite3.h>
#include <structures.h>
#include <constants.h>

#ifndef HEADERS_HEADERS_H_
#define HEADERS_HEADERS_H_

sqlite3 *create_database();
Data_Passer * startup();
void cleanup(Data_Passer *data_passer);


#endif /* HEADERS_HEADERS_H_ */
