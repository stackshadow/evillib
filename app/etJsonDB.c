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
	#include "etjDB.c"
	#include "etjDBTable.c"
#endif

etID_STATE			etjDBTest(){

	etDebugLevelSet( etID_LEVEL_DETAIL_DB );
	
// Vars
	etjDB 			*etjDBActual = NULL;

	etjDBAlloc( etjDBActual );

// Table settings
	etjDBTableAppend( etjDBActual, "parts" );

// Append some columns
	etjDBColumnAppend( etjDBActual, "UUID" );
	
	etjDBColumnAppend( etjDBActual, "datatimechanged" );
	etjDBColumnAppend( etjDBActual, "shortname" );
	etjDBColumnAppend( etjDBActual, "description" );
	etjDBColumnAppend( etjDBActual, "image" );
	etjDBColumnAppend( etjDBActual, "count" );




// We would like to add this table
	etjDBTableActionSet( etjDBActual, etjDB_ACTION_TABLE_ADD );


// We seek for a column and set it to validated
	etjDBColumnSeek( etjDBActual, "shortname" );

// Dump
	etjDBDumpf( etjDBActual );
	etjDBFree( etjDBActual );


	return etID_YES;


}


