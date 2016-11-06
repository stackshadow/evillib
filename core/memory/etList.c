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


etID_STATE      __etListFree( etList** p_list ){
// Object okay ?
    etDebugCheckNull( p_list );
    etDebugCheckNull( *p_list );

// vars
    etList*     etListElement = *p_list;
    etList*     etListElementToFree = NULL;

// go to start
    if( etListElement->prev != NULL ) etListElement = etListElement->prev;

// release all elements
    etListElement = etListElement->next;
    while( etListElement != NULL ){

        etListElementToFree = etListElement;
        etListElement = etListElement->next;

        etMemoryRelease( etListElementToFree );
    }

// release the list
    etListElement = *p_list;
    etMemoryRelease( etListElement );

// return
    *p_list = NULL;
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
    newListElement->data = data;
    list->next = newListElement;

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
    *iterator = (void*)etListElement->next;

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


etID_STATE      etListIterateNextAviable( void* iterator ){
// Object okay ?
    if( iterator == NULL ) return etID_NO;

// vars
    etList*     etListElement = (etList*)iterator;

// get data
    if( etListElement->next != NULL ){
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE      etListElementRemove( etList* elementToRemove ){

// vars
    etList*     etListElement = elementToRemove;

// unlink
    if( etListElement->prev == NULL ){
        // we get the first element, so we can not remove
        return etID_NO;
    }

    etListElement->prev->next = etListElement->next;

    if( etListElement->next != NULL ){
        etListElement->next->prev = etListElement->prev;
    }

// remove data
    etListElement->data = NULL;

// release the memory
    etMemoryRelease( etListElement );

    return etID_YES;
}


etID_STATE      etListDataRemove( etList* list, void* data, etID_BOOL removeAll ){
// Object okay ?
    etDebugCheckNull( list );
    etDebugCheckNull( data );

// vars
    etList*     etListElement = list;

// go to start
    if( etListElement->prev != NULL ) etListElement = etListElement->prev;

// try to find the data
    etListElement = etListElement->next;
    while( etListElement != NULL ){

        if( etListElement->data == data ){

        // remove the element
            etListElementRemove( etListElement );

            if( removeAll == etID_FALSE ){
                return etID_YES;
            }
        }

        etListElement = etListElement->next;
    }

}


etID_STATE      etListDump( etList* list ){
// Check
    etDebugCheckNull( list );

// Vars
    etList*     etListIterator = list;


#ifndef ET_DEBUG_OFF

    while( etListIterator != NULL ){

            fprintf( stdout, "[ %p<- ]", etListIterator->prev );
            fprintf( stdout, "[ %p ]", etListIterator );
            fprintf( stdout, "[ ->%p ]", etListIterator->next );
            fprintf( stdout, "[ %p ]", etListIterator->data );

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
