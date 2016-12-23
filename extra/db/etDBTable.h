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

#include "db/etDBColumn.h"
#include "db/etDBFilter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct etDBTable_t {
    etString*       name;
    etString*       displayName;

// columns
    etList*         etDBColumnList;
    etDBColumn*     etDBColumnDisplayValue;


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




etID_STATE          etDBTableAppendColumn( etDBTable* dbTable, etDBColumn* column );

#define             etDBTableIterateColumn( dbTable, iterator, column ) __etDBTableIterateColumn( dbTable, &iterator, &column )
etID_STATE          __etDBTableIterateColumn( etDBTable* dbTable, void **iterator, etDBColumn** p_column );

#define             etDBTableGetColumn( dbTable, columnName, dbColumn ) __etDBTableGetColumn( dbTable, columnName, &dbColumn )
etID_STATE          __etDBTableGetColumn( etDBTable* dbTable, const char* columnName, etDBColumn** p_dbColumn );

#define             etDBTableGetColumnWithOption( dbTable, dbColumnOptionToFind, dbColumn ) __etDBTableGetColumnWithOption( dbTable, dbColumnOptionToFind, &dbColumn )
etID_STATE          __etDBTableGetColumnWithOption( etDBTable* dbTable, int dbColumnOptionToFind, etDBColumn** p_dbColumn );


etID_STATE          etDBTableDumpColumn( etDBTable* dbTable );




etID_STATE          etDBTableSetDisplayColumn( etDBTable* dbTable, etDBColumn* dbColumn );

#define             etDBTableGetDisplayColumn( dbTable, dbColumn ) __etDBTableGetDisplayColumn( dbTable, &dbColumn )
etID_STATE          __etDBTableGetDisplayColumn( etDBTable* dbTable, etDBColumn** p_dbColumn );




etID_STATE          etDBTableCleanColumnValues( etDBTable* dbTable );










#ifdef __cplusplus
}
#endif

#endif
