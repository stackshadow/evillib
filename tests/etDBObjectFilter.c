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
#include "db/etDBObjectFilter.c"


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

    etDBObject *dbObject;
    etDBObjectAlloc( dbObject );
    etDBObjectFilterClear( dbObject );
    etDBObjectFilterAdd( dbObject, 0, etDBFILTER_OP_AND, "column", etDBFILTER_TYPE_CONTAIN, "test" );

    etDBObjectDump( dbObject );


// get the filter
    etDBObjectIterationReset( dbObject );
    int                 filterGroup;
    etDBFILTER_OP       filterOperation;
    const char          *filterColumn;
    etDBFILTER_TYPE     filterType;
    const char          *filterString;
    etDBObjectFilterNext( dbObject, &filterGroup, &filterOperation, &filterColumn, &filterType, &filterString );


}