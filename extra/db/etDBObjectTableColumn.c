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
#include "db/etDBObjectTableColumn.h"


etID_STATE      etDBObjectTableColumnAdd( etDBObject *dbObject, const char *columnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( columnName );


// check if we pick a table
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// get the columns object
    json_t *jsonColumns = json_object_get( dbObject->jsonTableActual, "columns" );
    if( jsonColumns == NULL ){
        jsonColumns = json_object();
        json_object_set( dbObject->jsonTableActual, "columns", jsonColumns );
    }


// create a new column
    json_t          *jsonColumn = json_object();
    json_object_set_new( jsonColumn, "name", json_string(columnName) );
    etDBObjectTypeSet( jsonColumn, etDBObject_TYPE_COLUMN );

// append the column to the table
    json_object_set_new( jsonColumns, columnName, jsonColumn );



}









