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


#ifndef _H_etDBObjectTableColumn
#define _H_etDBObjectTableColumn

#include "etDBObjectTable.h"


#ifdef __cplusplus
extern "C" {
#endif


#define etDBObject_TYPE_COLUMN "column"

typedef enum {
// nothing
    etDBCOLUMN_TYPE_NOTHING = 0,

// basic types
    etDBCOLUMN_TYPE_INT = 1,
    etDBCOLUMN_TYPE_FLOAT = 2,
    etDBCOLUMN_TYPE_STRING = 3,

// special
    etDBCOLUMN_TYPE_BLOB = 20


} etDBColumnType;

#define etDBCOLUMN_OPTION_NOTHING 0
#define etDBCOLUMN_OPTION_PRIMARY 1
#define etDBCOLUMN_OPTION_NOTNULL 2
#define etDBCOLUMN_OPTION_UNIQUE 4


etID_STATE      etDBObjectTableColumnAdd( etDBObject *dbObject, const char *columnName, etDBColumnType columnType, char columnOption );




#define         etDBObjectTableColumnNext( dbObject, columnName ) __etDBObjectTableColumnNext( dbObject, &columnName )
etID_STATE      __etDBObjectTableColumnNext( etDBObject *dbObject, const char **p_columnName );


etID_STATE      etDBObjectTableColumnIterate( etDBObject *dbObject, const char *tableName, void *userdata, etID_BOOL (*fctIterate)(void *userdata, const char *columnName, etDBColumnType columnType, char columnOption) );


etID_STATE      etDBObjectTableColumnPick( etDBObject *dbObject, const char *columnName );




#define         etDBObjectTableColumnNameGet( dbObject, columnName ) __etDBObjectTableColumnNameGet( dbObject, &columnName )
etID_STATE      __etDBObjectTableColumnNameGet( etDBObject *dbObject, const char **p_columnName );

#define         etDBObjectTableColumnTypeGet( dbObject, columnType ) __etDBObjectTableColumnTypeGet( dbObject, &columnType )
etID_STATE      __etDBObjectTableColumnTypeGet( etDBObject *dbObject, etDBColumnType *p_columnType );

#define         etDBObjectTableColumnOptionGet( dbObject, columnOption ) __etDBObjectTableColumnOptionGet( dbObject, &columnOption )
etID_STATE      __etDBObjectTableColumnOptionGet( etDBObject *dbObject, int *p_columnOption );




etID_STATE      etDBObjectTableColumnPrimarySet( etDBObject *dbObject, const char *primaryColumnName );

#define         etDBObjectTableColumnPrimaryGet( dbObject, primaryColumnName ) __etDBObjectTableColumnPrimaryGet( dbObject, &primaryColumnName )
etID_STATE      __etDBObjectTableColumnPrimaryGet( etDBObject *dbObject, const char **p_primaryColumnName );


etID_STATE      etDBObjectTableColumnMainSet( etDBObject *dbObject, const char *mainColumnName );

#define         etDBObjectTableColumnMainGet( dbObject, mainColumnName ) __etDBObjectTableColumnMainGet( dbObject, &mainColumnName )
etID_STATE      __etDBObjectTableColumnMainGet( etDBObject *dbObject, const char **p_mainColumnName );





#ifdef __cplusplus
}
#endif

#endif
