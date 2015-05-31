/* etjDB - evillib json DB representation
	Copyright (C) 2015 by Martin Langlotz alias stackshadow

	This file is part of evillib.

	evillib is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	evillib is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with evillib.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_etjDB
#define _H_etjDB


#include "jansson.h"


typedef struct 		etjDB {
	
	json_t*		nodeRoot;
	
// Node for all tables
	json_t*		nodeTable;
	json_t*		nodeTables;
	void*			nodeTablesIterator;

// Node for all columns
	json_t*		nodeColumn;
	json_t*		nodeColumns;
	void*			nodeColumnsIterator;

// values
	json_t*		nodeValuesNew;				// json-object for new values
	json_t*		nodeValues;				

} etjDB;


typedef enum		etjDBAction {
	etjDB_ACTION_NONE = 0,
	etjDB_ACTION_TABLE_ADD,
	etjDB_ACTION_TABLE_GET,
	etjDB_ACTION_TABLE_REMOVE,
	etjDB_ACTION_COLUMN_ADD,
	etjDB_ACTION_DATA_ADD,
	etjDB_ACTION_DATA_GET,
	etjDB_ACTION_DATA_REMOVE,
	etjDB_ACTION_DONE
} etjDBAction;


typedef enum		etjDBColumnType {
	etjDB_COL_TYPE_DEFAULT = 0,
	etjDB_COL_TYPE_TEXT,
	etjDB_COL_TYPE_COUNT
} etjDBColumnType;

typedef enum		etjDBCompareMode {
	etjDB_COMP_EQUAL = 0,
	etjDB_COMP_NOTEQUAL = 1,
	etjDB_COMP_COUNT
} etjDBCompareMode;



#define				etjDBAlloc( etjDBActual ) __etjDBAlloc( &etjDBActual )
etID_STATE				__etjDBAlloc( etjDB **p_etjDBActual );

#define				etjDBFree( etjDBActual ) __etjDBFree( &etjDBActual )
etID_STATE				__etjDBFree( etjDB **p_etjDBActual );


etID_STATE				__etjDBStringGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, const char **p_value );

#define				etjDBIntegerGet( jsonObject, keyNormal, keyNew, p_value ) __etjDBIntegerGet( jsonObject, keyNormal, keyNew, &p_value )
etID_STATE				__etjDBIntegerGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, etjDBColumnType *p_value );


etID_STATE				etjDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew );


void					etjDBDumpf( etjDB *etjDBActual );













#endif