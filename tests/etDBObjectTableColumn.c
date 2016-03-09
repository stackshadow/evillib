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


#include "evillib_defines.h"
#include "evillib_depends.h"


#include "core/etDebug.h"
#include "core/etDebug.c"

#include "memory/etMemoryBlock.h"
#include "memory/etMemoryBlock.c"
#include "memory/etMemoryBlockList.h"
#include "memory/etMemoryBlockList.c"
#include "memory/etMemory.h"
#include "memory/etMemory.c"

#include "core/etVersion.h"
#include "core/etVersion.c"
#include "core/etInit.h"
#include "core/etInit.c"

#include "app/etApicheck.h"
#include "app/etApicheck.c"

#include "db/etDBObject.c"
#include "db/etDBObjectTable.c"
#include "db/etDBObjectTableColumn.c"


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

// alloc the dbobject
    etDBObject *dbObject;
    etDBObjectAlloc( dbObject );

// add some tables
    etDBObjectTableAdd( dbObject, "table1" );
    etDBObjectTableAdd( dbObject, "table2" );
    etDBObjectTableAdd( dbObject, "table3" );
    etDBObjectTableAdd( dbObject, "table4" );
    etDBObjectDump( dbObject );


// pick table2
    etDBObjectTablePick( dbObject, "table2" );


// add some columns
    etDBObjectTableColumnAdd( dbObject, "column1" );
    etDBObjectTableColumnAdd( dbObject, "column2" );
    etDBObjectTableColumnAdd( dbObject, "column3" );



    etDBObjectDump( dbObject );
}