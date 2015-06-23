/* etDB - evillib json DB representation
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


typedef struct 		etDB {
	
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
	void*			nodeValuesNewIterator;
	json_t*		nodeValues;				// json-object for original ( actual ) values
	void*			nodeValuesIterator;

// filter
	json_t*		nodeValuesFilters;
	json_t*		nodeValuesFilter;
	void*			nodeValuesFilterIterator;

} etDB;


typedef enum		etDBAction {
	etDB_ACTION_NONE = 0,
	etDB_ACTION_TABLE_ADD,
	etDB_ACTION_TABLE_GET,
	etDB_ACTION_TABLE_GET_ALL,
	etDB_ACTION_TABLE_REMOVE,
	etDB_ACTION_COLUMN_ADD,
	etDB_ACTION_DATA_ADD,
	etDB_ACTION_DATA_GET,
	etDB_ACTION_DATA_REMOVE,
	etDB_ACTION_DONE
} etjDBAction;





#define				etDBAlloc( etDBActual ) __etDBAlloc( &etDBActual )
etID_STATE				__etDBAlloc( etDB **p_etjDBActual );

#define				etDBFree( etDBActual ) __etDBFree( &etDBActual )
etID_STATE				__etDBFree( etDB **p_etjDBActual );


etID_STATE				__etDBStringGet( json_t *jsonObject, 
							const char *keyNormal, 
							const char *keyNew, 
							const char **p_value,
							const char **p_valueNew
						);

#define				etDBIntegerGet( jsonObject, keyNormal, keyNew, p_value ) __etDBIntegerGet( jsonObject, keyNormal, keyNew, &p_value )
etID_STATE				__etDBIntegerGet( json_t *jsonObject, const char *keyNormal, const char *keyNew, int *p_value );


etID_STATE				etDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew );


void					etDBDumpf( etDB *etDBActual );













#endif