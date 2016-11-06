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
#include "string/etString.h"
#include "string/etStringChar.h"
#include "db/etDBObject.h"
#include "db/etDBObjectTable.h"



/** @ingroup etDB
@defgroup gretDBObjectTable etDBObjectTable - Table handling for etDBObject
@short The etDBObject contains Tables, this functions handle this tables




*/



/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Add an table to an etDBObject

If tableName is NULL, nothing will be done. \n

@param[in] dbObject The pointer to an etDBObject
@param[in] tableName The name of the new table
@return If the table was added correctly \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_ERR
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableAdd( etDBObject *dbObject, const char *tableName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( tableName );

// find object which hold all tables
    if( dbObject->jsonTables == NULL ){
        dbObject->jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
        if( dbObject->jsonTables == NULL ){
            dbObject->jsonTables = json_object();
            if( json_object_set_new( dbObject->jsonRootObject, "tables", dbObject->jsonTables ) != 0 ){
                return etID_STATE_ERR;
            }
        }
    }

// vars
    json_t      *jsonTables = dbObject->jsonTables;
    json_t      *jsonTable = NULL;

// reset the index stuff
    etDBObjectIterationReset( dbObject );

// create the table
    jsonTable = json_object();
    if( jsonTable == NULL ){
        return etID_STATE_ERR;
    }
// set the type
    if( json_object_set_new( jsonTable, "name", json_string(tableName) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonTable, "inDB", json_integer(0) ) != 0 ) return etID_STATE_ERR;


// add the "new" table
    if( json_object_set_new( jsonTables, tableName, jsonTable ) != 0 ){
        return etID_STATE_ERR;
    }

// save the new created table
    dbObject->jsonTable = jsonTable;
    dbObject->jsonColumns = NULL;
    dbObject->jsonColumn = NULL;

    snprintf( etDebugTempMessage, etDebugTempMessageLen, "append table '%s'", tableName );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

// return
    return etID_YES;
}



/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
 
@~english
@short Go to the next table
 
This function does also an etDBObjectTableReset()
 
@param[in] dbObject The pointer to an etDBObject
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableNext( etDBObject *dbObject, const char **p_tableName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_tableName );

// find object which hold all tables
    if( dbObject->jsonTables == NULL ){
    // get the columns object
        dbObject->jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
        if( dbObject->jsonTables == NULL ){
            *p_tableName = NULL;
            return etID_STATE_ERR;
        }
    }

// vars
    json_t      *jsonTables = dbObject->jsonTables;
    
// iterate
    if( dbObject->jsonIterator == NULL ){
        dbObject->jsonObjectToIterate = jsonTables;
        dbObject->jsonIterator = json_object_iter( dbObject->jsonObjectToIterate );
    } else {
    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonObjectToIterate, dbObject->jsonIterator );
    }

// end if list
    if( dbObject->jsonIterator == NULL){
        *p_tableName = NULL;
        return etID_STATE_NODATA;
    }

// get table from iterator
    dbObject->jsonTable = json_object_iter_value( dbObject->jsonIterator );
    dbObject->jsonColumns = NULL;
    dbObject->jsonColumn = NULL;

// get table name
    __etDBObjectTableNameGet( dbObject, p_tableName );

    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Pick a table from the table list

This function \n
 - set the actual table
 - unset the selected column
 - do a etDBObjectIterationReset()

@param[in] dbObject The pointer to an etDBObject
@param[in] tableName The table to pick
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTablePick( etDBObject *dbObject, const char *tableName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( tableName );

//
    etDBObjectTablesCheck( dbObject );

// vars
    json_t      *jsonTables = dbObject->jsonTables;

// pick table
    dbObject->jsonTable = json_object_get( jsonTables, tableName );
    if( dbObject->jsonTable == NULL ) {

        snprintf( etDebugTempMessage, etDebugTempMessageLen, "table '%s' not found in etDBObject", tableName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

        return etID_STATE_NODATA;
    }

    snprintf( etDebugTempMessage, etDebugTempMessageLen, "pick table '%s'", tableName );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

// reset the actual stuff
    dbObject->jsonColumns = NULL;
    dbObject->jsonColumn = NULL;
    etDBObjectIterationReset( dbObject );

    return etID_YES;
}



/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Set the name of the current table

You can set the current table with etDBObjectTableNext() or etDBObjectTablePick()

@param[in] dbObject The pointer to an etDBObject
@param[in] tableName The new name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableNameSet( etDBObject *dbObject, const char *tableName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( tableName );

// check if we pick a table
    etDBObjectTableCheck( dbObject );


// set the name
    json_object_set_new( dbObject->jsonTable, "name", json_string(tableName) );
    
    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableNameGet( etDBObject *dbObject, const char *tableName );
@~english
@short Get the name of the current table

You can set the current table with etDBObjectTableNext() or etDBObjectTablePick() \n
If no table is selected the tableName is set to "". \n


@param[in] dbObject The pointer to an etDBObject
@param[in] tableName The name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableNameGet( etDBObject *dbObject, const char **p_tableName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_tableName );


// check if we pick a table
    etDBObjectTableCheck( dbObject );

// vars
    json_t      *jsonTableName = NULL;

// try to get the table name
    jsonTableName = json_object_get( dbObject->jsonTable, "name" );
    if( jsonTableName == NULL ){
        *p_tableName = "";
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_tableName = json_string_value( jsonTableName );


    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Set the name of the current table

You can set the current table with etDBObjectTableNext() or etDBObjectTablePick()

@param[in] dbObject The pointer to an etDBObject
@param[in] tableName The new name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableDisplayNameSet( etDBObject *dbObject, const char *langCode, const char *displayName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( langCode );
    etDebugCheckNull( displayName );

// check if we pick a table
    etDBObjectTableCheck( dbObject );


// build the language string
    char fullLangString[15];
    memset( fullLangString, 0, 15 );
    strncpy( fullLangString, "displayName_", 12 );
    strncpy( &fullLangString[12], langCode, 2 );

// set the name
    json_object_set_new( dbObject->jsonTable, fullLangString, json_string(displayName) );
    
    return etID_YES;
}


etID_STATE      __etDBObjectTableDisplayNameGet( etDBObject *dbObject, const char *langCode, const char **p_displayName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( langCode );
    etDebugCheckNull( p_displayName );

// check if we pick a table
    etDBObjectTableCheck( dbObject );


// build the language string
    char fullLangString[15];
    memset( fullLangString, 0, 15 );
    strncpy( fullLangString, "displayName_", 12 );
    strncpy( &fullLangString[12], langCode, 2 );

// set the name
    json_t *jsonDisplayName = json_object_get( dbObject->jsonTable, fullLangString );
    if( jsonDisplayName == NULL ){
        
        fullLangString[12] = '\0';
        jsonDisplayName = json_object_get( dbObject->jsonTable, fullLangString );
        if( jsonDisplayName == NULL ){
            return __etDBObjectTableNameGet( dbObject, p_displayName );
        }
        
        *p_displayName = json_string_value(jsonDisplayName);
        return etID_YES;
        
    }
    
    *p_displayName = json_string_value(jsonDisplayName);
    return etID_YES;
}

