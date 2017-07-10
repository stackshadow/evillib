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
#include "db/etDBColumn.h"


/** @ingroup etDB
@defgroup gretDBObjectTable etDBObjectTable - Table handling for etDBObject
@short The etDBObject contains Tables, this functions handle this tables




*/

#ifdef __cplusplus
extern "C" {
#endif

#include "core/etInit.c"

#include "string/etString.c"
#include "memory/etList.c"

// pseudo defines
//etID_STATE          etDBColumnRemoveAll( etDBTable* dbTable );



etID_STATE          __etDBTableAlloc( etDBTable** p_dbTable ){
    etDebugCheckNull( p_dbTable );

// vars
    etDBTable*          dbTable = NULL;

// allocate memory
    etMemoryAlloc( dbTable, sizeof(etDBTable) );

// prealloc strings
	etStringAllocLen( dbTable->tableName, 32 );
	etStringAllocLen( dbTable->tableDisplayName, 64 );
	etStringAllocLen( dbTable->columnNameOfDisplayValues, 32 );

// allocate lists
	etListAlloc( dbTable->columns );
	dbTable->column = NULL;
	dbTable->columnIterator = NULL;


// return
    *p_dbTable = dbTable;
    return etID_YES;
}


etID_STATE          __etDBTableFree( etDBTable** p_dbTable ){
    etDebugCheckNull( p_dbTable );

// vars
    etDBTable*          dbTable = *p_dbTable;

//
	etStringFree( dbTable->tableName );
	etStringFree( dbTable->tableDisplayName );
	etStringFree( dbTable->columnNameOfDisplayValues );

// free the columns
	etDBColumnRemoveAll( dbTable );
	etListFree( dbTable->columns );


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
	etStringCharSet( dbTable->tableName, tableName, -1 );

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
	if( __etStringCharGet( dbTable->tableName, p_tableName ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
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
	if( etStringCharSet( dbTable->tableDisplayName, displayName, -1 ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          __etDBTableGetDisplayName( etDBTable* dbTable, const char** p_displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_displayName );

// get name
	if( __etStringCharGet( dbTable->tableDisplayName, p_displayName ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}




etID_STATE          etDBTableSetDisplayColumnName( etDBTable* dbTable, const char* columnName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( columnName );

// set the name
	if( etStringCharSet( dbTable->columnNameOfDisplayValues, columnName, -1 ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          __etDBTableGetDisplayColumnName( etDBTable* dbTable, const char** p_columnName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_columnName );

// get name
	if( __etStringCharGet( dbTable->columnNameOfDisplayValues, p_columnName ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}




etID_STATE          etDBTableDump( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );


    return etID_YES;
}





#ifdef __cplusplus
}
#endif


#endif
