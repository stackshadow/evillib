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



etID_STATE      etDBObjectTableColumnAdd( etDBObject *dbObject, const char *columnName, etDBColumnType columnType ){
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
    if( etDBObjectTypeSet( jsonColumn, etDBObject_TYPE_COLUMN ) != etID_YES ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "name", json_string(columnName) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "columnType", json_integer(columnType) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "inDB", json_integer(0) ) != 0 ) return etID_STATE_ERR;

// append the column to the table
    if( json_object_set_new( jsonColumns, columnName, jsonColumn ) != 0 ) return etID_STATE_ERR;


    return etID_YES;
}




/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Go to the next column

@param[in] dbObject The pointer to an etDBObject
@return \n
*- @ref etID_STATE_NODATA
*- @ref etID_STATE_ERR
*- @ref etID_STATE_WARN_SEQERR when you dont select an table before
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableColumnNext( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// check if we pick a table
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }



// vars
    if( dbObject->jsonIterator == NULL ){
    // get the columns object
        json_t *jsonColumns = json_object_get( dbObject->jsonTableActual, "columns" );
        if( jsonColumns == NULL ) return etID_STATE_ERR;

        dbObject->jsonObjectToIterate = jsonColumns;
        dbObject->jsonIterator = json_object_iter( dbObject->jsonObjectToIterate );
    } else {
    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonObjectToIterate, dbObject->jsonIterator );
    }

// get object
    if( dbObject->jsonIterator != NULL){
        dbObject->jsonColumnActual = json_object_iter_value( dbObject->jsonIterator );
        return etID_YES;
    }

// return no data
    return etID_STATE_NODATA;
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Pick a column

@param[in] dbObject The pointer to an etDBObject
@return \n
*- @ref etID_STATE_NODATA
*- @ref etID_STATE_ERR
*- @ref etID_STATE_WARN_SEQERR when you dont select an table before
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableColumnPick( etDBObject *dbObject, const char *columnName ){
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
    if( jsonColumns == NULL ) return etID_STATE_ERR;

// pick column
    dbObject->jsonColumnActual = json_object_get( jsonColumns, columnName );
    if( dbObject->jsonColumnActual != NULL ) return etID_YES;


    return etID_STATE_NODATA;
}





/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableColumnNameGet( etDBObject *dbObject, const char *columnName );
@~english
@short Get the name of the current column

@param[in] dbObject The pointer to an etDBObject
@param[in] columnName The name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableColumnNameGet( etDBObject *dbObject, const char **p_columnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_columnName );


// check if we pick a table
    if( dbObject->jsonColumnActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a column" );
        *p_columnName = "";
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonColumnName = NULL;

// try to get the table name
    jsonColumnName = json_object_get( dbObject->jsonColumnActual, "name" );
    if( jsonColumnName == NULL ){
        *p_columnName = "";
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_columnName = json_string_value( jsonColumnName );


    return etID_YES;
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Set the primary column of an table

@param[in] dbObject The pointer to an etDBObject
@param[in] primaryColumnName The name of the primary column
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableColumnPrimarySet( etDBObject *dbObject, const char *primaryColumnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( primaryColumnName );

// check if we pick a table
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// set the name
    json_object_set_new( dbObject->jsonTableActual, "primaryColumn", json_string(primaryColumnName) );
    
    return etID_YES;
}
