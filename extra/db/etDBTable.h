/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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


#ifndef _H_etDBObjectTable
#define _H_etDBObjectTable

#include "memory/etMemory.h"
#include "memory/etList.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "jansson.h"

#include "db/etDBFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct etDBTable_t {

// jsons
    json_t*     jsonObjectTable;
    json_t*     jsonObjectColumns;
    json_t*     jsonObjectValues;

// indexing
    void*       iteratorColumn;
    json_t*     actualColumn;


} etDBTable;




#define             etDBTableAlloc( dbTable ) __etDBTableAlloc( &dbTable )
etID_STATE          __etDBTableAlloc( etDBTable** p_dbTable );

#define             etDBTableFree( dbTable ) __etDBTableFree( &dbTable )
etID_STATE          __etDBTableFree( etDBTable** p_dbTable );

/*
etID_STATE      etDBObjectTableAdd( etDBObject *dbObject, const char *tableName );



#define         etDBObjectTableNext( dbObject, tableName ) __etDBObjectTableNext( dbObject, &tableName )
etID_STATE      __etDBObjectTableNext( etDBObject *dbObject, const char **p_tableName );


etID_STATE      etDBObjectTablePick( etDBObject *dbObject, const char *tableName );

*/





etID_STATE          etDBTableSetName( etDBTable* dbTable, const char* tableName );

#define             etDBTableGetName( dbTable, tableName ) __etDBTableGetName( dbTable, &tableName )
etID_STATE          __etDBTableGetName( etDBTable* dbTable, const char** p_tableName );


etID_STATE          etDBTableSetDisplayName( etDBTable* dbTable, const char* displayName );

#define             etDBTableGetDisplayName( dbTable, displayName ) __etDBTableGetDisplayName( dbTable, &displayName )
etID_STATE          __etDBTableGetDisplayName( etDBTable* dbTable, const char** p_displayName );




etID_STATE          etDBTableSetDisplayColumnName( etDBTable* dbTable, const char* columnName );

#define             etDBTableGetDisplayColumnName( dbTable, columnName ) __etDBTableGetDisplayColumnName( dbTable, &columnName )
etID_STATE          __etDBTableGetDisplayColumnName( etDBTable* dbTable, const char** p_columnName );










#ifdef __cplusplus
}
#endif

#endif
