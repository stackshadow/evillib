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

#ifndef _C_etDBTable
#define _C_etDBTable

// needed sources
#include "core/etDebug.c"
#include "core/etObject.c"
#include "memory/etMemory.c"
#include "string/etString.c"
#include "string/etStringChar.c"

// headers
#include "db/etDBTable.h"



/** @ingroup etDB
@defgroup gretDBObjectTable etDBObjectTable - Table handling for etDBObject
@short The etDBObject contains Tables, this functions handle this tables




*/

#ifdef __cplusplus
extern "C" {
#endif


etID_STATE          __etDBTableAlloc( etDBTable** p_dbTable ){
    etDebugCheckNull( p_dbTable );

// vars
    etDBTable*          dbTable = NULL;

// allocate memory
    etMemoryAlloc( dbTable, sizeof(etDBTable) );

// create json-table
    dbTable->jsonObjectTable = json_object();
    dbTable->jsonObjectColumns = json_object();
    dbTable->jsonObjectValues = json_object();

    json_object_set_new( dbTable->jsonObjectTable, "columns", dbTable->jsonObjectColumns );

// return
    *p_dbTable = dbTable;
    return etID_YES;
}


etID_STATE          __etDBTableFree( etDBTable** p_dbTable ){
    etDebugCheckNull( p_dbTable );

// vars
    etDBTable*          dbTable = *p_dbTable;

// destroy json
    json_decref( dbTable->jsonObjectTable );


// release memory
    __etMemoryRelease( (void**)p_dbTable );

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
etID_STATE          etDBTableSetName( etDBTable* dbTable, const char* tableName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( tableName );

// set the name
    json_object_set_new( dbTable->jsonObjectTable, "name", json_string(tableName) );

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
etID_STATE          __etDBTableGetName( etDBTable* dbTable, const char** p_tableName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_tableName );

// get name
    json_t* jsonTableName = json_object_get( dbTable->jsonObjectTable, "name" );
    if( jsonTableName == NULL ) return etID_STATE_ERR;
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
etID_STATE          etDBTableSetDisplayName( etDBTable* dbTable, const char* displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( displayName );

// set the name
    json_object_set_new( dbTable->jsonObjectTable, "displayName", json_string(displayName) );

    return etID_YES;
}


etID_STATE          __etDBTableGetDisplayName( etDBTable* dbTable, const char** p_displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_displayName );

// get name
    if( p_displayName != NULL ){
        json_t* jsonTableDisplayName = json_object_get( dbTable->jsonObjectTable, "displayName" );
        if( jsonTableDisplayName == NULL ) return etID_STATE_ERR;
        *p_displayName = json_string_value( jsonTableDisplayName );
    }

    return etID_YES;
}




etID_STATE          etDBTableSetDisplayColumnName( etDBTable* dbTable, const char* columnName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( columnName );

// set the name
    json_object_set_new( dbTable->jsonObjectTable, "displayColumnName", json_string(columnName) );

    return etID_YES;
}


etID_STATE          __etDBTableGetDisplayColumnName( etDBTable* dbTable, const char** p_columnName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_columnName );

// get name
    if( p_columnName != NULL ){
        json_t* jsonTableDisplayColumnName = json_object_get( dbTable->jsonObjectTable, "displayColumnName" );
        if( jsonTableDisplayColumnName == NULL ) return etID_STATE_ERR;
        *p_columnName = json_string_value( jsonTableDisplayColumnName );
        return etID_YES;
    }

    return etID_YES;
}




etID_STATE          etDBTableDump( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );

    char* dumpString = json_dumps( dbTable->jsonObjectTable, JSON_PRESERVE_ORDER | JSON_INDENT(4) );
    fprintf( stdout, "%s\n", dumpString );
    free( dumpString );

    return etID_YES;
}





#ifdef __cplusplus
}
#endif


#endif
