/*  Copyright (C) 2014-2016 by Martin Langlotz alias stackshadow

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

#include "evillib_depends.h"
#include "evillib-extra_depends.h"


#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"
#include "db/etDBObjectTable.h"



etID_STATE      etDBObjectTableAdd( etDBObject *dbobject, const char *tableName ){
// check
    etDebugCheckNull( dbobject );
    etDebugCheckNull( tableName );

// vars
    json_t      *jsonTable = json_object();

// add the table name
    json_object_set( jsonTable, "name", json_string(tableName) );

// add the "new" table to the array
    json_array_append( dbobject->jsonTables, jsonTable );

// reset the rest
    etDBObjectTableReset( dbobject );

    return etID_YES;
}


etID_STATE      etDBObjectTableReset( etDBObject *dbobject ){
// check
    etDebugCheckNull( dbobject );

// reset the actual table stuff    
    dbobject->jsonTable = NULL;

// reset the column stuff
    dbobject->jsonColumns = NULL;
    dbobject->jsonColumn = NULL;
    dbobject->jsonColumnIndex = 0;

// return
    return etID_YES;
}


etID_STATE      etDBObjectTableNextReset( etDBObject *dbobject ){
// check
    etDebugCheckNull( dbobject );

    dbobject->jsonTableIndex = 0;

    return etID_YES;
}


etID_STATE      etDBObjectTableNext( etDBObject *dbobject ){
// check
    etDebugCheckNull( dbobject );

// vars
    int     jsonArraySize;
    
// get size
    jsonArraySize = json_array_size( dbobject->jsonTables );
    
// check size
    if( dbobject->jsonTableIndex >= jsonArraySize ){
        return etID_STATE_NODATA;
    }

// reset
    etDBObjectTableReset( dbobject );

// get
    dbobject->jsonTable = json_array_get( dbobject->jsonTables, dbobject->jsonTableIndex );
    
// increment
    dbobject->jsonTableIndex = dbobject->jsonTableIndex + 1;


// return
    if( dbobject->jsonTable != NULL ){
        return etID_YES;
    }
    return etID_STATE_NODATA;
}


etID_STATE      etDBObjectTablePick( etDBObject *dbobject, const char *tableName ){
    
}


etID_STATE      __etDBObjectTableNameGet( etDBObject *dbobject, const char **p_tableName ){
// check
    etDebugCheckNull( dbobject );
    etDebugCheckNull( p_tableName );


// check if we pick a table
    if( dbobject->jsonTable == NULL ){
        *p_tableName = NULL;
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonTableName = NULL;

// try to get the table name
    jsonTableName = json_object_get( dbobject->jsonTable, "name" );
    if( jsonTableName == NULL ){
        *p_tableName = NULL;
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_tableName = json_string_value( jsonTableName );


    return etID_YES;
}


