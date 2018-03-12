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

// alloc list of elements
    etListAlloc( dbFilter->elements );
    dbFilter->elementIterator = NULL;

// return
    *p_dbFilter = dbFilter;
    return etID_OK;
}


etID_STATE          __etDBFilterFree( etDBFilter** p_dbFilter ){
    etDebugCheckNull( p_dbFilter );

// vars
    etDBFilter*             dbFilter = *p_dbFilter;
    void*                   dbFilterElementIterator = NULL;
    etDBFilterElement*      dbFilterElement = NULL;

// free elements
    if( etListIterate( dbFilter->elements, dbFilterElementIterator ) != etID_OK ){
        return etID_STATE_ERR_INTERR;
    }
    while( etListIterateNext( dbFilterElementIterator, dbFilterElement ) == etID_OK ){
        etStringFree( dbFilterElement->column );
        etStringFree( dbFilterElement->value );
        etMemoryRelease( dbFilterElement );
    }
    etListFree( dbFilter->elements );

// free the list
    etMemoryRelease( dbFilter );

// return
    *p_dbFilter = NULL;
    return etID_OK;
}


etID_STATE          etDBFilterAppend( etDBFilter* dbFilter, int group, etDBFILTER_OP operation, const char* columnName, etDBFILTER_TYPE type, const char* value ){
    etDebugCheckNull( dbFilter );

// a new element
    etDBFilterElement*      dbFilterElement = NULL;

// allocate memory
    etMemoryAlloc( dbFilterElement, sizeof(etDBFilterElement) );
    etStringAllocLen( dbFilterElement->column, 16 );
    etStringAllocLen( dbFilterElement->value, 32 );


// set values
    dbFilterElement->group = group;
    dbFilterElement->operation = operation;
    etStringCharSet( dbFilterElement->column, columnName, -1 );
    dbFilterElement->type = type;
    etStringCharSet( dbFilterElement->value, value, -1 );

// append
    etListAppend( dbFilter->elements, dbFilterElement );


    return etID_OK;
}


etID_STATE          etDBFilterIterateReset( etDBFilter* dbFilter ){
    etDebugCheckNull( dbFilter );

    dbFilter->elementIterator = NULL;
    etListIterate( dbFilter->elements, dbFilter->elementIterator );

    return etID_OK;
}


etID_STATE          etDBFilterIterate( etDBFilter* dbFilter ){
// check
    etDebugCheckNull( dbFilter );

// start or continue
    if( dbFilter->elementIterator == NULL ){
		return etID_NO;
    }

// get the data
	if( etListIterateNext( dbFilter->elementIterator, dbFilter->element ) == etID_YES ){
		return etID_OK;
	}

    return etID_NO;
}


etID_STATE          __etDBFilterElementGet( etDBFilter* dbFilter, int* group, etDBFILTER_OP* operation, const char** p_columnName, etDBFILTER_TYPE* type, const char** p_value ){
// check
    etDebugCheckNull( dbFilter );
    etDebugCheckNull( dbFilter->element );

// values
    etDBFilterElement*      dbFilterElement = (etDBFilterElement*)dbFilter->element;

    *group = dbFilterElement->group;
    *operation = dbFilterElement->operation;
    __etStringCharGet( dbFilterElement->column, p_columnName );
    *type = dbFilterElement->type;
    __etStringCharGet( dbFilterElement->value, p_value );


    return etID_OK;
}


#ifdef __cplusplus
}
#endif


#endif
