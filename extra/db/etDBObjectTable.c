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

/** @defgroup gretDBObjectTable etDBObjectTable - Table handling for etDBObject
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

// create the table
    dbObject->jsonTable = json_object();
    if( dbObject->jsonTable == NULL ){
        return etID_STATE_ERR;
    }

// add the table name
    if( json_object_set_new( dbObject->jsonTable, "name", json_string(tableName) ) != 0 ){
        return etID_STATE_ERR;
    }
//    json_object_set_new( jsonTable, "inDB", json_integer(0) );

// add the "new" table to the array
    if( json_array_append( dbObject->jsonTables, dbObject->jsonTable ) != 0 ){
        return etID_STATE_ERR;
    }

// reset the rest
    etDBObjectTableReset( dbObject );

    return etID_YES;
}

/** @ingroup gretDBObjectTable
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Reset The table

Reset the table

@param[in] dbObject The pointer to an etDBObject
@return If the table was added correctly \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableReset( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// reset the column stuff
    dbObject->jsonColumns = NULL;
    dbObject->jsonColumn = NULL;
    dbObject->jsonColumnIndex = 0;

// return
    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Reset the iteration of all tables

This reset the iteration of an table to beginning for later use of etDBObjectTableNext()

@param[in] dbObject The pointer to an etDBObject
@return  \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableNextReset( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

    dbObject->jsonTableIndex = 0;

    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@short Go to the next table

@param[in] dbObject The pointer to an etDBObject
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableNext( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// vars
    int     jsonArraySize;
    
// get size
    jsonArraySize = json_array_size( dbObject->jsonTables );
    
// check size
    if( dbObject->jsonTableIndex >= jsonArraySize ){
        return etID_STATE_NODATA;
    }

// reset
    etDBObjectTableReset( dbObject );

// get
    dbObject->jsonTable = json_array_get( dbObject->jsonTables, dbObject->jsonTableIndex );
    
// increment
    dbObject->jsonTableIndex = dbObject->jsonTableIndex + 1;


// return
    if( dbObject->jsonTable != NULL ){
        return etID_YES;
    }
    return etID_STATE_NODATA;
}


etID_STATE      etDBObjectTablePick( etDBObject *dbObject, const char *tableName ){
    
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
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, so you don't get a name" );
        *p_tableName = "";
        return etID_STATE_WARN_SEQERR;
    }

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
@short Set the display name of the current table

You can set the current table with etDBObjectTableNext() or etDBObjectTablePick() \n
If no table is selected the tableName is set to "". \n
You need two chars as language code. Like "de" or "en". If you provide more chars, only the first two letters are used

@param[in] dbObject The pointer to an etDBObject
@param[in] langCode The language code
@param[in] displayName The name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      etDBObjectTableDisplayNameSet( etDBObject *dbObject, const char *langCode, const char *displayName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( displayName );


// check if we pick a table
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, so you don't can set a name" );
        return etID_STATE_WARN_SEQERR;
    }

// vars
    char        fullLangCode[15] = "displayName";


// if the langCode is not null
    if( langCode != NULL ){
    // displayName + _de
        strncpy( fullLangCode, "displayName_", 12 );
        strncpy( &fullLangCode[12], langCode, 2 );
        fullLangCode[14] = 0;        
    }

// set string inside the object
    json_object_set_new( dbObject->jsonTable, fullLangCode, json_string(displayName) );
    
    return etID_YES;
}

/** @ingroup gretDBObjectTable
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etDBObjectTableDisplayNameGet( etDBObject *dbObject, const char *langCode, const char *displayName );
@~english
@short Get the display name of the current table

You can set the current table with etDBObjectTableNext() or etDBObjectTablePick() \n
If no table is selected the tableName is set to "". \n
You need two chars as language code. Like "de" or "en". If you provide more chars, only the first two letters are used

@param[in] dbObject The pointer to an etDBObject
@param[in] langCode The language code
@param[in] tableName The name of the current table
@return \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_STATE_WARN_SEQERR
*- @ref etID_STATE_NODATA
*- @ref etID_YES
*/
etID_STATE      __etDBObjectTableDisplayNameGet( etDBObject *dbObject, const char *langCode, const char **p_displayName ){
// check
    etDebugCheckNull( dbObject );
    etDebugCheckNull( p_displayName );


// check if we pick a table
    if( dbObject->jsonTable == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, so you don't can get a name" );
        *p_displayName = "";
        return etID_STATE_WARN_SEQERR;
    }

// vars
    char        fullLangCode[11+4] = "displayName";
    json_t      *jsonString = NULL;

// if no langCode was provided, use the default
    if( langCode != NULL ){
    // set displayname
        strncpy( fullLangCode, "displayName_", 12 );
        strncpy( &fullLangCode[12], langCode, 2 );
        fullLangCode[14] = 0;
    }

// check if we get the string
    jsonString = json_object_get( dbObject->jsonTable, fullLangCode );
    if( jsonString == NULL ){
        *p_displayName = "";
        return etID_STATE_NODATA;
    }

// return the display name
    *p_displayName = json_string_value( jsonString );
    return etID_YES;
}




