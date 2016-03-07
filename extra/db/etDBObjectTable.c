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

// vars
    json_t      *jsonTable;

// create the table
    jsonTable = json_object();
    if( jsonTable == NULL ){
        return etID_STATE_ERR;
    }

// set the type
    if( etDBObjectTypeSet( jsonTable, etDBObject_TYPE_TABLE ) != etID_YES ){
        return etID_STATE_ERR;
    }
// add the "new" table
    if( json_object_set_new( jsonTable, "name", json_string(tableName) ) != 0 ){
        return etID_STATE_ERR;
    }
// add the "new" table
    if( json_object_set_new( dbObject->jsonRootObject, tableName, jsonTable ) != 0 ){
        return etID_STATE_ERR;
    }



// reset the index stuff
    etDBObjectSelectionReset( dbObject );

// save the object
    dbObject->jsonObject = jsonTable;

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
etID_STATE      etDBObjectTableNext( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// vars
    int     jsonArraySize;

    if( dbObject->jsonIterator == NULL ){
        dbObject->jsonIterator = json_object_iter( dbObject->jsonRootObject );
    } else {
    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonRootObject, dbObject->jsonIterator );
    }

// check size
    while( dbObject->jsonIterator != NULL){

    // get
        dbObject->jsonObject = json_object_iter_value( dbObject->jsonIterator );

    // check if the object is an table
        if( etDBObjectTypeCheck(dbObject->jsonObject, etDBObject_TYPE_TABLE) == etID_YES ){
            return etID_YES;
        }

    // iterate
        dbObject->jsonIterator = json_object_iter_next( dbObject->jsonRootObject, dbObject->jsonIterator );
    }

// return no data
    return etID_STATE_NODATA;
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
    etID_STATE      state = etID_YES;
    const char      *tableNameActual = NULL;

// reset
    state = etDBObjectSelectionReset(dbObject);
    if( state != etID_YES ) return state;

// go to every element
    while( etDBObjectTableNext( dbObject ) == etID_YES ){

        if( etDBObjectTableNameGet( dbObject, tableNameActual ) == etID_YES ){
            
            if( strncmp(tableNameActual,tableName,strlen(tableNameActual)) == 0 ){
                
                return etID_YES;
                
            }
            
        }

    }

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
    if( dbObject->jsonObject == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, can not set a name" );
        return etID_STATE_WARN_SEQERR;
    }
// check if the object is an table
    if( etDBObjectTypeCheck(dbObject->jsonObject, etDBObject_TYPE_TABLE) == etID_YES ){
        return etID_YES;
    }


// set the name
    json_object_set_new( dbObject->jsonObject, "name", json_string(tableName) );
    
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
    if( dbObject->jsonObject == NULL ){
        etDebugMessage( etID_STATE_WARN, "You did not select a table, so you don't get a name" );
        *p_tableName = "";
        return etID_STATE_WARN_SEQERR;
    }
// check if the picked object is an table
    if( etDBObjectTypeCheck(dbObject->jsonObject,etDBObject_TYPE_TABLE) == etID_NO ){
        return etID_STATE_WARN_SEQERR;
    }


// vars
    json_t      *jsonTableName = NULL;

// try to get the table name
    jsonTableName = json_object_get( dbObject->jsonObject, "name" );
    if( jsonTableName == NULL ){
        *p_tableName = "";
        return etID_STATE_NODATA;
    }

// get the string from the json-string
    *p_tableName = json_string_value( jsonTableName );


    return etID_YES;
}



