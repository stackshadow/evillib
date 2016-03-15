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

#ifndef _H_etDBObject
#define _H_etDBObject

#ifdef __cplusplus
extern "C" {
#endif



typedef struct etDBObject {
   
// root element
    json_t          *jsonRootObject;        // the json root-object


// actual positions
    json_t          *jsonTableActual;
    json_t          *jsonColumnActual;


// temporary values
// values for iteration
    int             jsonIteratorIndex;
    json_t          *jsonObjectToIterate;
    void            *jsonIterator;

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



#ifdef __cplusplus
}
#endif

#endif