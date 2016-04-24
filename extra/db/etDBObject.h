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

#ifndef _H_etDBObject
#define _H_etDBObject

#include "evillib_defines.h"
#include "core/etIDState.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct etDBObject {
   
// root element
    json_t          *jsonRootObject;        // the json root-object


// actual positions
    json_t          *jsonTables;
    json_t          *jsonTable;

    json_t          *jsonColumns;
    json_t          *jsonColumn;


// temporary values
// values for iteration
    int             jsonIteratorIndex;
    json_t          *jsonObjectToIterate;
    void            *jsonIterator;

// some temporary values we keep track of
    const char      *dumpString;

// driver
    void            *driverData;
} etDBObject;



#define             etDBObjectAlloc( dbObject ) __etDBObjectAlloc( &dbObject )
etID_STATE          __etDBObjectAlloc( etDBObject **p_dbObject );

#define             etDBObjectFree( dbObject ) __etDBObjectFree( &dbObject )
etID_STATE          __etDBObjectFree( etDBObject **p_dbObject );


etID_STATE          etDBObjectDump( etDBObject *dbObject );


etID_STATE          etDBObjectIterationReset( etDBObject *dbObject );


etID_STATE          etDBObjectTypeSet( json_t *jsonObject, const char *type );


etID_STATE          etDBObjectTypeCheck( json_t *jsonObject, const char *type );




#define             etDBObjectTablesCheck( dbObject ) if( dbObject->jsonTables == NULL ) { \
        etDebugMessage( etID_STATE_WARN, "No Tables present in dbObject" ); \
        return etID_STATE_WARN_SEQERR; \
    }

#define             etDBObjectTableCheck( dbObject ) if( dbObject->jsonTable == NULL ) { \
        etDebugMessage( etID_STATE_WARN, "No Table Selected" ); \
        return etID_STATE_WARN_SEQERR; \
    }

#define             etDBObjectColumnsCheck( dbObject ) if( dbObject->jsonColumns == NULL ) { \
        etDebugMessage( etID_STATE_WARN, "No columns present in table" ); \
        return etID_STATE_WARN_SEQERR; \
    }
#define             etDBObjectColumnCheck( dbObject ) if( dbObject->jsonColumn == NULL ) { \
        etDebugMessage( etID_STATE_WARN, "No Tables present in dbObject" ); \
        return etID_STATE_WARN_SEQERR; \
    }



#ifdef __cplusplus
}
#endif

#endif