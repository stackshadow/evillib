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

#ifndef _C_etList
#define _C_etList

#include "memory/etMemory.c"
#include "memory/etMemoryBlock.c"

#include "memory/etList.h"

#ifdef __cplusplus
extern "C" {
#endif




etID_STATE      __etListAlloc( etList** p_list ){
// Object okay ?
    etDebugCheckNull( p_list );

// Vars
    etList*     newList = NULL;


// Allocate etString
    etMemoryRequest( newList, sizeof(etList) );
    if( newList == NULL ){
        *p_list = NULL;
        return etDebugState(etID_STATE_CRIT_NOMEMORY);
    }

// String properties
    newList->prev = NULL;
    newList->next = NULL;
    newList->data = NULL;


// Return
    *p_list = newList;
    return etID_YES;
}

etID_STATE      __etListAppend( etList** p_list, void *data ){
// Object okay ?
    etDebugCheckNull( p_list );

// Vars
    etList*     list = *p_list;
    etList*     newListElement = NULL;

// go to the end
    while( list->next != NULL ) list = list->next;

// Allocate
    etMemoryRequest( newListElement, sizeof(etList) );
    if( newListElement == NULL ){
        *p_list = NULL;
        return etDebugState(etID_STATE_CRIT_NOMEMORY);
    }

    newListElement->prev = list;
    list->next = newListElement;
    list->data = data;

    *p_list = newListElement;
    return etID_YES;
}

etID_STATE      __etListIterate( etList* list, void** iterator ){
// Object okay ?
    etDebugCheckNull( list );
    etDebugCheckNull( iterator );

// vars
    etList*     etListElement = list;

// go to start
    while( etListElement->prev != NULL ) etListElement = etListElement->prev;

// return as iterator
    *iterator = (void*)etListElement;

    return etID_YES;
}

etID_STATE      __etListIterateNext( void** iterator, void** data ){
// Object okay ?
    etDebugCheckNull( iterator );
    etDebugCheckNull( data );

// the end of the list ?
    if( *iterator == NULL ) return etID_STATE_NODATA;

// vars
    etList*     etListElement = (etList*)*iterator;

// get data
    *data = etListElement->data;

// return the next element
    *iterator = (void*)etListElement->next;

    return etID_YES;
}


etID_STATE      etListRemoveElement( etList** p_elementToRemove ){

// vars
    etList*     etListElement = *p_elementToRemove;

// if the element is the first one
    if( etListElement->prev == NULL ){
        etListElement->data = NULL;
        etListElement->next->prev = NULL;

    // return the next element
        *p_elementToRemove = *etListElement->next;

    // release the memory
        etMemoryRelease( etListElement );

        return etID_YES;
    }

    etListElement->data = NULL;
    etListElement->prev->next = etListElement->next;
    etListElement->next->prev = etListElement->prev;

// release the memory
    etMemoryRelease( etListElement );



    return etID_YES;
}


etID_STATE      etListDump( etList* list ){
// Check
    etDebugCheckNull( list );

// Vars
    etList*     etListIterator = list;


#ifndef ET_DEBUG_OFF

    while( etListIterator != NULL ){

            fprintf( stdout, "[ %p ]", etListIterator );
            fprintf( stdout, "[ %p ]", etListIterator->data );
            fprintf( stdout, "[ ->%p ]", etListIterator->next );


            fprintf( stdout, "\n" );

        etListIterator = etListIterator->next;
    }

#endif

    return etID_YES;
}



#ifdef __cplusplus
}
#endif



#endif
