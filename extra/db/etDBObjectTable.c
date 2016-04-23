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

// vars
    json_t      *jsonTables;
    json_t      *jsonTable;


// reset the index stuff
    etDBObjectIterationReset( dbObject );

// get the object which holds all tables
    jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
    if( jsonTables == NULL ){
    // create a new table
        jsonTables = json_object();
        if( json_object_set_new( dbObject->jsonRootObject, "tables", jsonTables ) != 0 ){
            return etID_STATE_ERR;
        }

    }

// create the table
    jsonTable = json_object();
    if( jsonTable == NULL ){
        return etID_STATE_ERR;
    }
// set the type
    if( etDBObjectTypeSet( jsonTable, etDBObject_TYPE_TABLE ) != etID_YES ) return etID_STATE_ERR;
    if( json_object_set_new( jsonTable, "name", json_string(tableName) ) != 0 ) return etID_STATE_ERR;
    if( json_object_set_new( jsonTable, "inDB", json_integer(0) ) != 0 ) return etID_STATE_ERR;


// add the "new" table
    if( json_object_set_new( jsonTables, tableName, jsonTable ) != 0 ){
        return etID_STATE_ERR;
    }

// save the new created table
    dbObject->jsonTableActual = jsonTable;

// return
    return etID_YES;
}



/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Call for every table the fctItreate callback-function

The callback function is in the following form:
@code
etID_BOOL (*fctIterate)(void *userdata, const char *tableName, const char *tableDisplayName);
@endcode

@param[in] dbObject The pointer to an etDBObject
@param[in] langCode the language code as two letter char-array
@param[in] userdata An void-pointer to userdata which is passed as first argument in the callback-function
@param[in] fctIterate The callback function 
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableIterate( etDBObject *dbObject, const char *langCode, void *userdata, etID_BOOL (*fctIterate)(void *userdata, const char *tableName, const char *tableDisplayName) ){
// check
    etDebugCheckNull( dbObject );

// vars
    json_t      *jsonTables;
    void        *jsonIterator;
    json_t      *jsonIteratorObject;
    json_t      *jsonValue;

    const char  *tempCharArray;
    const char  *tableName;
    const char  *tableDisplayName;
    

// get the object which holds all tables
    jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
    if( jsonTables == NULL ){
        return etID_STATE_NODATA;
    }

// build the language string
    etString *fullLanguageString = NULL;
    etStringAlloc( fullLanguageString );


    jsonIterator = json_object_iter(jsonTables);
    while( jsonIterator != NULL ){

    //
        jsonIteratorObject = json_object_iter_value(jsonIterator);

    //
        etStringCharSet( fullLanguageString, "displayName_\0", 13 );
        etStringCharAdd( fullLanguageString, langCode );


    // get table name
        jsonValue = json_object_get( jsonIteratorObject, "name" );
        if( jsonValue == NULL ){
            goto next;
        }
        tableName = json_string_value(jsonValue);

    // try to get displayName
        etStringCharGet( fullLanguageString, tempCharArray );
        jsonValue = json_object_get( jsonIteratorObject, tempCharArray );
        if( jsonValue == NULL ){

            etStringCharSet( fullLanguageString, "displayName_\0", 13 );
            etStringCharGet( fullLanguageString, tempCharArray );
            jsonValue = json_object_get( jsonIteratorObject, tempCharArray );
            if( jsonValue == NULL ){
                tableDisplayName = tableName;
            } else {
                tableDisplayName = json_string_value(jsonValue);
            }

        } else {
            tableDisplayName = json_string_value(jsonValue);
        }

        fctIterate( userdata, tableName, tableDisplayName );

next:
    // next
        jsonIterator = json_object_iter_next(jsonTables,jsonIterator);
    }


    
    
    
    
    
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Pick a table from the table list

This function \n
 - set the actual table
 - do a etDBObjectTableReset()

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

// vars
    json_t      *jsonTables;

// get the object which holds all tables
    jsonTables = json_object_get( dbObject->jsonRootObject, "tables" );
    if( jsonTables == NULL ){
        return etID_STATE_NODATA;
    }

// pick table
    dbObject->jsonTableActual = json_object_get( jsonTables, tableName );
    if( dbObject->jsonTableActual != NULL ) return etID_YES;


    return etID_STATE_NODATA;
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
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, can not set a name" );
        return etID_STATE_WARN_SEQERR;
    }

// set the name
    json_object_set_new( dbObject->jsonTableActual, "name", json_string(tableName) );
    
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
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, so you don't get a name" );
        *p_tableName = "";
        return etID_STATE_WARN_SEQERR;
    }

// vars
    json_t      *jsonTableName = NULL;

// try to get the table name
    jsonTableName = json_object_get( dbObject->jsonTableActual, "name" );
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
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, can not set a name" );
        return etID_STATE_WARN_SEQERR;
    }

// build the language string
    char fullLangString[15];
    memset( fullLangString, 0, 15 );
    strncpy( fullLangString, "displayName_", 12 );
    strncpy( &fullLangString[12], langCode, 2 );

// set the name
    json_object_set_new( dbObject->jsonTableActual, fullLangString, json_string(displayName) );
    
    return etID_YES;
}


etID_STATE      __etDBObjectTableDisplayNameGet( etDBObject *dbObject, const char *langCode, const char **p_displayName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( langCode );
    etDebugCheckNull( p_displayName );

// check if we pick a table
    if( dbObject->jsonTableActual == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, can not set a name" );
        return etID_STATE_WARN_SEQERR;
    }

// build the language string
    char fullLangString[15];
    memset( fullLangString, 0, 15 );
    strncpy( fullLangString, "displayName_", 12 );
    strncpy( &fullLangString[12], langCode, 2 );

// set the name
    json_t *jsonDisplayName = json_object_get( dbObject->jsonTableActual, fullLangString );
    if( jsonDisplayName == NULL ){
        
        fullLangString[12] = '\0';
        jsonDisplayName = json_object_get( dbObject->jsonTableActual, fullLangString );
        if( jsonDisplayName == NULL ){
            return __etDBObjectTableNameGet( dbObject, p_displayName );
        }
        
        *p_displayName = json_string_value(jsonDisplayName);
        return etID_YES;
        
    }
    
    *p_displayName = json_string_value(jsonDisplayName);
    return etID_YES;
}

