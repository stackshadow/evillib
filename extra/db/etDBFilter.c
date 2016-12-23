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



#ifndef etDBFilter_C
#define etDBFilter_C

#include "core/etDebug.c"
#include "core/etObject.c"
#include "memory/etMemory.c"
#include "string/etString.c"
#include "string/etStringChar.c"

#include "db/etDBFilter.h"

#ifdef __cplusplus
extern "C" {
#endif


etID_STATE          __etDBFilterAlloc( etDBFilter** p_dbFilter ){
    etDebugCheckNull( p_dbFilter );

// vars
    etDBFilter*             dbFilter = NULL;

// allocate memory
    etMemoryAlloc( dbFilter, sizeof(etDBFilter) );

// allocate list
    etListAlloc( dbFilter->start );
    dbFilter->end = dbFilter->start;


// return
    *p_dbFilter = dbFilter;
    return etID_YES;
}


etID_STATE          __etDBFilterFree( etDBFilter** p_dbFilter ){
    etDebugCheckNull( p_dbFilter );

// vars
    etDBFilter*             dbFilter = *p_dbFilter;
    etDBFilterElement*      dbFilterElement = NULL;
    void*                   dbFilterIterator = NULL;




// clean the list itselfe
    etListFree( dbFilter->start );

// free the memory for our class
    etMemoryRelease( dbFilter );

// return
    *p_dbFilter = NULL;
    return etID_YES;
}


etID_STATE          etDBFilterAppend( etDBFilter* dbFilter, int group, etDBFILTER_OP operation, const char* columnName, etDBFILTER_TYPE type, const char* value ){
    etDebugCheckNull( dbFilter );

// vars
    etDBFilterElement*      dbFilterElement = NULL;

// create a new element
    etMemoryAlloc( dbFilterElement, sizeof(etDBFilterElement) );

    dbFilterElement->group = group;

    dbFilterElement->operation = operation;

    if( dbFilterElement->column == NULL ) etStringAlloc( dbFilterElement->column );
    etStringCharSet( dbFilterElement->column, columnName, -1 );

    dbFilterElement->type = type;

    if( dbFilterElement->value == NULL ) etStringAlloc( dbFilterElement->value );
    etStringCharSet( dbFilterElement->value, value, -1 );

// append to the list
    etListAppend( dbFilter->end, dbFilterElement );

    return etID_YES;
}


etID_STATE          __etDBFilterIterate( etDBFilter* dbFilter, void** iterator, etDBFilterElement** p_dbFilterElement ){

// iterate from the beginning
    if( *iterator == NULL && *p_dbFilterElement == NULL ){
        __etListIterate( dbFilter->start, iterator );
    }

// get data and set iterator to next element
    return __etListIterateNext( iterator, (void**)p_dbFilterElement );
}


etID_STATE          __etDBFilterElementGet( etDBFilterElement* dbFilterElement, int* group, etDBFILTER_OP* operation, const char** p_columnName, etDBFILTER_TYPE* type, const char** p_value ){
    etDebugCheckNull( dbFilterElement );


    if( group != NULL ){
        *group = dbFilterElement->group;
    }

    if( operation != NULL ){
        *operation = dbFilterElement->operation;
    }

    if( p_columnName != NULL ){
        __etStringCharGet( dbFilterElement->column, p_columnName );
    }

    if( type != NULL ){
        *type = dbFilterElement->type;
    }

    if( p_value != NULL ){
        __etStringCharGet( dbFilterElement->value, p_value );
    }

}


#ifdef __cplusplus
}
#endif


#endif
