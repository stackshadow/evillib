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
#include "memory/etList.c"
#include "string/etString.c"
#include "string/etStringChar.c"

// db stuff
#include "db/etDBColumn.c"
#include "db/etDBFilter.c"

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

// allocate strings
    etStringAllocLen( dbTable->name, 64 );
    etStringAllocLen( dbTable->displayName, 64 );

// columns
    dbTable->etDBColumnList = NULL;
    dbTable->etDBColumnDisplayValue = NULL;


// return
    *p_dbTable = dbTable;
    return etID_YES;
}


etID_STATE          __etDBTableFree( etDBTable** p_dbTable ){
    etDebugCheckNull( p_dbTable );

// vars
    etDBTable*          dbTable = *p_dbTable;

// release string
    etStringFree( dbTable->name );
    etStringFree( dbTable->displayName );

// release columns
    if( dbTable->etDBColumnList != NULL ){

        void*           dbColumnIterator = NULL;
        etDBColumn*     dbColumn;

        etListIterate( dbTable->etDBColumnList, dbColumnIterator );
        while( etListIterateNext(dbColumnIterator,dbColumn) == etID_YES ){
            etDBColumnFree(dbColumn);
        }

        etListFree( dbTable->etDBColumnList );
    }


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
    etStringCharSet( dbTable->name, tableName, -1 );

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


// get the string from the json-string
    __etStringCharGet( dbTable->name, p_tableName );

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
    etStringCharSet( dbTable->displayName, displayName, -1 );

    return etID_YES;
}


etID_STATE          __etDBTableGetDisplayName( etDBTable* dbTable, const char** p_displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_displayName );


// get the string from the json-string
    __etStringCharGet( dbTable->displayName, p_displayName );

    return etID_YES;
}




etID_STATE          etDBTableAppendColumn( etDBTable* dbTable, etDBColumn* column ){

// alloc if needed
    if( dbTable->etDBColumnList == NULL ){
        etListAlloc( dbTable->etDBColumnList );
    }

// append
    etListAppend( dbTable->etDBColumnList, column );

    return etID_YES;
}


etID_STATE          __etDBTableIterateColumn( etDBTable* dbTable, void** iterator, etDBColumn** p_column ){


// iterate from the beginning
    if( *iterator == NULL && *p_column == NULL ){
        __etListIterate( dbTable->etDBColumnList, iterator );
    }

// get data and set iterator to next element
    return __etListIterateNext( iterator, (void**)p_column );
}


etID_STATE          __etDBTableGetColumn( etDBTable* dbTable, const char* columnName, etDBColumn** p_dbColumn ){

    etDBColumn*         dbColumn = NULL;
    const char*         dbColumnName = NULL;
    void*               dbColumnIterator = NULL;

    while( etDBTableIterateColumn( dbTable, dbColumnIterator, dbColumn ) == etID_YES ){

        __etDBColumnGet( dbColumn, &dbColumnName, NULL, NULL );

        if( strlen(columnName) != strlen(dbColumnName) ) continue;

        if( strncmp( columnName, dbColumnName, strlen(dbColumnName) ) == 0 ){
            *p_dbColumn = dbColumn;

        // Debug
            #ifndef ET_DEBUG_OFF
                snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column '%s' found", columnName );
                etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
            #endif

            return etID_YES;
        }

    }

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "Column '%s' not found", columnName );
        etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );
    #endif
    return etID_NO;
}


etID_STATE          __etDBTableGetColumnWithOption( etDBTable* dbTable, int dbColumnOptionToFind, etDBColumn** p_dbColumn ){


    void*               dbColumnIterator = NULL;
    etDBColumn*         dbColumn = NULL;
    const char*         dbColumnName = NULL;
    unsigned int        dbColumnOption = etDBCOLUMN_OPTION_NOTHING;

    while( etDBTableIterateColumn( dbTable, dbColumnIterator, dbColumn ) == etID_YES ){

        __etDBColumnGet( dbColumn, NULL, NULL, &dbColumnOption );

        if( dbColumnOption & dbColumnOptionToFind ){
            *p_dbColumn = dbColumn;
            return etID_YES;
        }

    }

    *p_dbColumn = NULL;
    return etID_STATE_NODATA;
}


etID_STATE          etDBTableDumpColumn( etDBTable* dbTable ){


    etDBColumn*         dbColumn = NULL;
    const char*         dbColumnName = NULL;
    etDBColumnType      dbColumnType = etDBCOLUMN_TYPE_NOTHING;
    unsigned int        dbColumnOption = 0;
    const char*         dbColumnValue = NULL;
    void*               listIterator = NULL;

    etListDump( dbTable->etDBColumnList );

    if( dbTable->etDBColumnList != NULL ){

        fprintf( stdout, "[COLUMN][TYPE][OPTION]\n" );


        while( etDBTableIterateColumn(dbTable,listIterator,dbColumn) == etID_YES ){

            etDBColumnGet( dbColumn, dbColumnName, dbColumnType, dbColumnOption );
            etDBColumnGetValue( dbColumn, dbColumnValue );

            fprintf( stdout, "[%s]", dbColumnName );
            fprintf( stdout, "[%i]", dbColumnType );
            fprintf( stdout, "[%i]", dbColumnOption );
            fprintf( stdout, "[%s]", dbColumnValue );

            fprintf( stdout, "\n" );


        }

    }


}




etID_STATE          etDBTableSetDisplayColumn( etDBTable* dbTable, etDBColumn* dbColumn ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbColumn );

    dbTable->etDBColumnDisplayValue = dbColumn;

    return etID_YES;
}


etID_STATE          __etDBTableGetDisplayColumn( etDBTable* dbTable, etDBColumn** p_dbColumn ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_dbColumn );

    *p_dbColumn = dbTable->etDBColumnDisplayValue;

    return etID_YES;
}




etID_STATE          etDBTableCleanColumnValues( etDBTable* dbTable ){

    etDBColumn*         dbColumn = NULL;
    const char*         dbColumnName = NULL;
    etDBColumnType      dbColumnType = etDBCOLUMN_TYPE_NOTHING;
    unsigned int        dbColumnOption = 0;
    void*               listIterator = NULL;

    if( dbTable->etDBColumnList != NULL ){

        fprintf( stdout, "[COLUMN][TYPE][OPTION]\n" );

        while( etDBTableIterateColumn(dbTable,listIterator,dbColumn) == etID_YES ){
            //etDBColumnSet( dbColumn, "", dbColumn->type, dbColumn->option );
            etDBColumnSetValue( dbColumn, "" );
        }

    }

}






#ifdef __cplusplus
}
#endif


#endif
