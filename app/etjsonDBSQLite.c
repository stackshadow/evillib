/* Apicheck Function for etMemory
	Copyright (C) 2015 by Martin Langlotz alias stackshadow

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


#include "evillib_defines.h"
#include "evillib_depends.h"

#include <sqlite3.h>

#include "core/etDebug.h"
#include "etApicheck.h"

#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "etjDB.h"
#include "etjDBTable.h"
#include "etjDBColumn.h"

#ifdef ET_SINGLEMODULE
	#include "core/etObject.h"
	#include "etjDBDriver.c"
	#include "etjDBSQLite.c"
	#include "etjDBColumn.c"
	#include "etjDBValue.c"
#endif

// Vars
	etjDB 			*etjDBActual = NULL;
	etjDBDriver	dbDriver_static;
	etjDBDriver	*dbDriver = &dbDriver_static;

void				etjDBSQLiteTestCreateTable(){

// Allocate
	etjDBAlloc( etjDBActual );


// ##################### The part table #####################

// Object-Table
	etjDBTableAppend( etjDBActual, "object" );
	etjDBTableVisibleNameSet( etjDBActual, "Objekte" );
	etjDBColumnAppend( etjDBActual, "objectID" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "objectTypeID" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "shortname" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "name" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "description" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_TABLE_ADD );

// ObjectType-Table
	etjDBTableAppend( etjDBActual, "objectType" );
	etjDBTableVisibleNameSet( etjDBActual, "Objekt-Typen" );
	etjDBColumnAppend( etjDBActual, "objectTypeID" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "name" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "description" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBColumnAppend( etjDBActual, "filename" ); 
	etjDBColumnTypeSet( etjDBActual, etjDB_COL_TYPE_TEXT );
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_TABLE_ADD );


// Dump
	etjDBDumpf( etjDBActual );

// Build and run the query
	etjDBRun( dbDriver, etjDBActual );
	


}

void				etjDBSQLiteTestGetTable(){
	
	etjDBTableSeek( etjDBActual, "object" );
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_TABLE_GET );

	
// Build and run the query
	etjDBRun( dbDriver, etjDBActual );
	
	etjDBDumpf( etjDBActual );
}

void				etjDBSQLiteTestDataCreate(){
	
	
	etjDBTableSeek( etjDBActual, "object" );
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_DATA_ADD );

// 
	etjDBValueSet( etjDBActual, "objectID", "5fb1b78a-fc60-11e4-82b7-34e6d7289d44" );
	etjDBValueSet( etjDBActual, "shortname", "S1" );
	etjDBValueSet( etjDBActual, "name", "Schrank 1" );
	etjDBValueSet( etjDBActual, "description", "Schrank für Gefahrengut" );


	etjDBDumpf( etjDBActual );
	
	etjDBRun( dbDriver, etjDBActual );
}


void				etjDBSQLiteTestDataGet(){
	
// Set table action to recieve data
	etjDBTableSeek( etjDBActual, "object" );
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_DATA_GET );

	etjDBRun( dbDriver, etjDBActual );
	
	etjDBDumpf( etjDBActual );
	
	
}


etID_STATE			etjDBSQLiteTest(){

	etDebugLevelSet( etID_LEVEL_DETAIL_DB );
	


// We init the driver
	etjDBSQLiteInit( dbDriver );
	etjDBSQLiteConnect( dbDriver, "/tmp/test.sqlite" );


// Create some tables
	etjDBSQLiteTestCreateTable();
	etjDBSQLiteTestGetTable();
	etjDBSQLiteTestDataCreate();
	etjDBSQLiteTestDataGet();
	return;

// We free the object
	etjDBFree( etjDBActual );




// Build and run the query
	dbDriver->queryRun( dbDriver, etjDBActual );
	dbDriver->dump( dbDriver, etjDBActual );
	

	
	etjDBDumpf( etjDBActual );

// cool we now get the table and add some data


// We free the object
	etjDBFree( etjDBActual );
	return etID_YES;
}




