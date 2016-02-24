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


#include "evillib-extra_depends.h"

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "db/etDBObject.h"

#ifdef __cplusplus
extern "C" {
#endif



etID_STATE          __etDBObjectAlloc( etDBObject **p_dbObject ){
    etDebugCheckNull( p_dbObject );
    
// vars
    etDBObject *tempDBObject = NULL;

// allocate memory
    etMemoryAlloc( tempDBObject, sizeof(etDBObject) );

// set
    tempDBObject->jsonTables = json_array();
    
// return
    *p_dbObject = tempDBObject;
    return etID_YES;
}


etID_STATE          __etDBObjectFree( etDBObject **p_dbObject ){
    etDebugCheckNull( p_dbObject );

// vars
    etDBObject *tempDBObject = *p_dbObject;

// release json stuff
    json_decref( tempDBObject->jsonTables );
    etMemoryRelease( tempDBObject );
    
// return
    *p_dbObject = NULL;
    return etID_YES;
}


etID_STATE          etDBObjectDump( etDBObject *dbObject ){
// vars
    etDebugCheckNull( dbObject );

    
    char *jsonDump = json_dumps( dbObject->jsonTables, JSON_INDENT(4) );
    
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s", jsonDump );
    etDebugMessage( etID_LEVEL_DETAIL_DB, etDebugTempMessage );

    return etID_YES;
}




#ifdef __cplusplus
}
#endif


