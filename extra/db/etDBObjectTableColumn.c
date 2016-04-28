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

#include "evillib_depends.h"
#include "evillib-extra_depends.h"


#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"
#include "db/etDBObjectTable.h"
#include "db/etDBObjectTableColumn.h"



etID_STATE      etDBObjectTableColumnAdd( etDBObject *dbObject, const char *columnName, etDBColumnType columnType, char columnOption ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( columnName );


// check if we pick a table
    etDBObjectTableCheck( dbObject );

// get the columns object
    if( dbObject->jsonColumns == NULL ){
        dbObject->jsonColumns = json_object_get( dbObject->jsonTable, "columns" );
        if( dbObject->jsonColumns == NULL ){
            dbObject->jsonColumns = json_object();
            if( json_object_set_new( dbObject->jsonTable, "columns", dbObject->jsonColumns ) != 0 ){
                return etID_STATE_ERR;
            }
        }
    }

// vars
    json_t          *jsonColumns = dbObject->jsonColumns;
    json_t          *jsonColumn = NULL;

// create a new column
    jsonColumn = json_object();
    if( etDBObjectTypeSet( jsonColumn, etDBObject_TYPE_COLUMN ) != etID_YES ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "name", json_string(columnName) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "columnType", json_integer(columnType) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonColumn, "columnOption", json_integer(columnOption) ) != 0 ) return etID_STATE_ERR;    
    if( json_object_set_new( jsonColumn, "inDB", json_integer(0) ) != 0 ) return etID_STATE_ERR;

// append the column to the table
    if( json_object_set_new( jsonColumns, columnName, jsonColumn ) != 0 ) return etID_STATE_ERR;
    dbObject->jsonColumn = jsonColumn;

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
etID_STATE      __etDBObjectTableColumnNext( etDBObject *dbObject, const char **p_columnName ){
// check
    etDebugCheckNull( dbObject );

// check if we pick a table
    etDBObjectTableCheck( dbObject );


// get the columns object
    if( dbObject->jsonColumns == NULL ){
        dbObject->jsonColumns = json_object_get( dbObject->jsonTable, "columns" );
        if( dbObject->jsonColumns == NULL ){
            *p_columnName = NULL;
            return etID_STATE_ERR;
        }
    }

// iterate
    if( dbObject->jsonIterator == NULL ){
        dbObject->jsonObjectToIterate = dbObject->jsonColumns;
        dbObject->jsonIterator = json_object_iter( dbObject->jsonObjectToIterate );
    } else {
    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonObjectToIterate, dbObject->jsonIterator );
    }


// end if list
    if( dbObject->jsonIterator == NULL){
        *p_columnName = NULL;
        return etID_STATE_NODATA;
    }

// get object
    dbObject->jsonColumn = json_object_iter_value( dbObject->jsonIterator );
    __etDBObjectTableColumnNameGet( dbObject, p_columnName );

    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Call for every column the fctItreate callback-function

This function DONT change the pick-state of an table ;)

The callback function is in the following form:
@code
etID_BOOL fctIterate( void *userdata, const char *columnName, etDBColumnType columnType, char columnOption );
@endcode

@param[in] dbObject The pointer to an etDBObject
@param[in] userdata An void-pointer to userdata which is passed as first argument in the callback-function
@param[in] fctIterate The callback function 
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableColumnIterate( etDBObject *dbObject, const char *tableName, void *userdata, etID_BOOL (*fctIterate)(void *userdata, const char *columnName, etDBColumnType columnType, char columnOption) ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( fctIterate );

// vars
    json_t      *jsonTables;
    json_t      *jsonTable;
    json_t      *jsonColumns;
    json_t      *jsonColumn;
    void        *jsonIterator;
    json_t      *jsonColumnName;
    json_t      *jsonColumnType;
    json_t      *jsonColumnOption;

    

// get the object which holds all tables
    jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
    if( jsonTables == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "There are no tables" );
        return etID_STATE_WARN_SEQERR;
    }

// pick table
    jsonTable = json_object_get( jsonTables, tableName );
    if( jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "Table not found" );
        return etID_STATE_WARN_SEQERR;
    }

// get the columns
    jsonColumns = json_object_get( jsonTable, "columns" );
    if( jsonColumns == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "There is no column array inside the table" );
        return etID_STATE_WARN_SEQERR;
    }

// iterate
    jsonIterator = json_object_iter(jsonColumns);
    while( jsonIterator != NULL ){

    //
        jsonColumn = json_object_iter_value(jsonIterator);
        
        jsonColumnName = json_object_get( jsonColumn, "name" );
        jsonColumnType = json_object_get( jsonColumn, "columnType" );
        jsonColumnOption = json_object_get( jsonColumn, "columnOption" );
        
        fctIterate( userdata, json_string_value(jsonColumnName), json_integer_value(jsonColumnType), json_integer_value(jsonColumnOption) );

    // next
        jsonIterator = json_object_iter_next(jsonColumns,jsonIterator);
    }

    
    
    
    
    
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Pick a column

@param[in] dbObject The pointer to an etDBObject
@param[in] columnName The column to pick
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
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// get the columns object
    json_t *jsonColumns = json_object_get( dbObject->jsonTable, "columns" );
    if( jsonColumns == NULL ) return etID_STATE_ERR;

// pick column
    dbObject->jsonColumn = json_object_get( jsonColumns, columnName );
    if( dbObject->jsonColumn != NULL ) return etID_YES;


    return etID_STATE_NODATA;
}




/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableColumnNameGet( etDBObject *dbObject, const char *columnName );
@~english
@short Get the name of the current column

@param[in] dbObject The pointer to an etDBObject
@param[out] columnName The name of the current table
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


// check if we pick a column
    if( dbObject->jsonColumn == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a column" );
        *p_columnName = "";
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonColumnName = NULL;

// try to get the table name
    jsonColumnName = json_object_get( dbObject->jsonColumn, "name" );
    if( jsonColumnName == NULL ){
        *p_columnName = "";
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_columnName = json_string_value( jsonColumnName );


    return etID_YES;
}


etID_STATE      __etDBObjectTableColumnTypeGet( etDBObject *dbObject, etDBColumnType *p_columnType ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_columnType );


// check if we pick a column
    if( dbObject->jsonColumn == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a column" );
        *p_columnType = etDBCOLUMN_TYPE_NOTHING;
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonColumnType = NULL;

// try to get the table name
    jsonColumnType = json_object_get( dbObject->jsonColumn, "columnType" );
    if( jsonColumnType == NULL ){
        *p_columnType = etDBCOLUMN_TYPE_NOTHING;
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_columnType = json_integer_value( jsonColumnType );


    return etID_YES;
}


etID_STATE      __etDBObjectTableColumnOptionGet( etDBObject *dbObject, int *p_columnOption ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_columnOption );


// check if we pick a column
    if( dbObject->jsonColumn == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a column" );
        *p_columnOption = etDBCOLUMN_OPTION_NOTHING;
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonColumnOption = NULL;

// try to get the table name
    jsonColumnOption = json_object_get( dbObject->jsonColumn, "columnOption" );
    if( jsonColumnOption == NULL ){
        *p_columnOption = etDBCOLUMN_OPTION_NOTHING;
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_columnOption = json_integer_value( jsonColumnOption );


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
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// set the name
    json_object_set_new( dbObject->jsonTable, "primaryColumn", json_string(primaryColumnName) );
    
    return etID_YES;
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableColumnPrimaryGet( etDBObject *dbObject, const char *primaryColumnName )
@~english
@short Get the name of the primary column

@param[in] dbObject The pointer to an etDBObject
@param[out] primaryColumnName The name of the primary column
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableColumnPrimaryGet( etDBObject *dbObject, const char **p_primaryColumnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_primaryColumnName );

// check if we pick a table
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// get
    json_t *jsonPrimaryColumn = json_object_get( dbObject->jsonTable, "primaryColumn" );
    if( jsonPrimaryColumn == NULL ){

    // get table name for debug info
        const char *tableName = NULL;
        if( etDBObjectTableNameGet( dbObject, tableName ) != etID_YES ){
            return etID_STATE_ERR_INTERR;
        }

    // debug info
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "No Primary Column for table %s\n", tableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

// set the name
    *p_primaryColumnName = json_string_value(jsonPrimaryColumn);
    
    return etID_YES;
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Set the column which holds the main value

This function is primary for user-interfaces and sets the column which holds the main value which are primary meant
to be displayed by an user interface

@param[in] dbObject The pointer to an etDBObject
@param[in] mainColumnName The name of the main column
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableColumnMainSet( etDBObject *dbObject, const char *mainColumnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( mainColumnName );

// check if we pick a table
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// set the name
    json_object_set_new( dbObject->jsonTable, "mainColumn", json_string(mainColumnName) );
    
    return etID_YES;
}

/** @ingroup etDBObjectTableColumn
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableColumnPrimaryGet( etDBObject *dbObject, const char *primaryColumnName )
@~english
@short Get the name of the main column @see etDBObjectTableColumnMainSet()

@param[in] dbObject The pointer to an etDBObject
@param[out] mainColumnName The name of the main column
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableColumnMainGet( etDBObject *dbObject, const char **p_mainColumnName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_mainColumnName );

// check if we pick a table
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_LEVEL_WARNING, "You did not select a table" );
        return etID_STATE_WARN_SEQERR;
    }

// get
    json_t *jsonMainColumn = json_object_get( dbObject->jsonTable, "mainColumn" );
    if( jsonMainColumn == NULL ){

    // get table name for debug info
        const char *tableName = NULL;
        if( etDBObjectTableNameGet( dbObject, tableName ) != etID_YES ){
            return etID_STATE_ERR_INTERR;
        }

    // debug info
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "No Main Column for table %s\n", tableName );
        etDebugMessage( etID_LEVEL_ERR, etDebugTempMessage );
        return etID_STATE_ERR_INTERR;
    }

// set the name
    *p_mainColumnName = json_string_value(jsonMainColumn);
    
    return etID_YES;
}


