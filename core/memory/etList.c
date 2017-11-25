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
    newList->start = NULL;
    newList->end = NULL;

// Return
    *p_list = newList;
    return etID_YES;
}


etID_STATE      __etListFree( etList** p_list ){
// Object okay ?
    etDebugCheckNull( p_list );
    etDebugCheckNull( *p_list );

// vars
    etList*             list = *p_list;

// clean the list
    etListClean( list );

// release the list
    etMemoryRelease( list );

// return
    *p_list = NULL;
    return etID_YES;
}


etID_STATE      etListClean( etList* list ){
// Object okay ?
    etDebugCheckNull( list );

// vars
    etListElement*      listElement = list->start;
    etListElement*      listElementNext = NULL;
    int                 etListCounter = 0;

// release all elements
    while( listElement != NULL ){
        listElementNext = listElement->next;

        etMemoryRelease( listElement );

        listElement = listElementNext;
        etListCounter++;
    }

    list->start = NULL;
    list->end = NULL;

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p cleaned %i elements", list, etListCounter );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

    return etID_YES;
}


etID_STATE      etListAppend( etList* list, void *data ){
// Object okay ?
    etDebugCheckNull( list );

// Vars
    etListElement*      listElement = NULL;

// Allocate
    etMemoryRequest( listElement, sizeof(etListElement) );
    if( listElement == NULL ){
        return etDebugState(etID_STATE_CRIT_NOMEMORY);
    }
    listElement->data = data;

// refresh the end
    if( list->end != NULL ){
        while( list->end->next != NULL ) list->end = list->end->next;
    }

    if( list->end == NULL ){
        list->start = listElement;
        list->end = listElement;
    } else {
        list->end->next = listElement;
        listElement->prev = list->end;
        list->end = listElement;
    }

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "append %p to list %p", listElement, list );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

    return etID_YES;
}




etID_STATE      etListElementRemove( etList* list, etListElement* element ){
// Object okay ?
    etDebugCheckNull( list );
    etDebugCheckNull( element );


// if the element is the start
    if( element == list->start ){
        list->start = element->next;
    }

// if the element is the end
    if( element == list->end ){
        list->end = element->prev;
    }

// unlink from the prev element
    if( element->prev != NULL ){
        element->prev->next = element->next;
    }

// unlink the next
    if( element->next != NULL ){
        element->next->prev = element->prev;
    }

// release the element
    etMemoryRelease( element );

    return etID_YES;
}





etID_STATE      __etListIterate( etList* list, void** iterator ){
// Object okay ?
    etDebugCheckNull( list );
    etDebugCheckNull( iterator );

// list start is empty
	if( list->start == NULL ){
		*iterator = NULL;
		return etID_NO;
	}

// vars
    etListElement*      listElement = list->start;

// return as iterator
    *iterator = (void*)listElement;

    return etID_YES;
}


etID_STATE      __etListIterateGet( void* iterator, void** data ){
// Object okay ?
    etDebugCheckNull( iterator );

// vars
    etListElement*      listElement = (etListElement*)iterator;

// the end of the list ?
    if( listElement == NULL ) return etID_STATE_NODATA;

// get data
    *data = listElement->data;

    return etID_YES;
}


etID_STATE      __etListIterateNext( void** iterator, void** data ){
// Object okay ?
    etDebugCheckNull( iterator );
    etDebugCheckNull( data );


// vars
    etListElement*      listElement = (etListElement*)*iterator;

// the end of the list ?
    if( listElement == NULL ) return etID_STATE_NODATA;

// get data
    *data = listElement->data;

// next
    listElement = listElement->next;

// return the next element
    *iterator = (void*)listElement;

    return etID_YES;
}


etID_STATE      etListIterateNextAviable( void* iterator ){
// Object okay ?
    if( iterator == NULL ) return etID_NO;

// vars
    etListElement*      listElement = NULL;
    listElement = (etListElement*)iterator;

// get data
    if( listElement->next != NULL ){
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE      __etListIterateRemove( etList* list, void** iterator ){

// vars
    etListElement*      listElement = (etListElement*)*iterator;

// return the next element
    *iterator = (void*)listElement->next;

// remove the actual element
    return etListElementRemove( list, listElement );
}




etID_STATE      etListDataRemove( etList* list, void* data, etID_BOOL removeAll ){
// Object okay ?
    etDebugCheckNull( list );
    etDebugCheckNull( data );



// vars
    etListElement*      listElement = list->start;
    etListElement*      listElementToFree = NULL;



    while( listElement != NULL ){



        if( listElement->data == data ){

        // if the element is the start
            if( listElement == list->start ){
                list->start = listElement->next;
            }

        // if the element is the end
            if( listElement == list->end ){
                list->end = listElement->prev;
            }

        // unlink from the prev element
            if( listElement->prev != NULL ){
                listElement->prev->next = listElement->next;
            }

        // unlink the next
            if( listElement->next != NULL ){
                listElement->next->prev = listElement->prev;
            }

            listElementToFree = listElement;
            listElement = listElement->next;
            etMemoryRelease( listElementToFree );

            return etID_YES;
        }



        listElement = listElement->next;
    }

	return etID_YES;
}


etID_STATE      etListDump( etList* list ){
// Check
    etDebugCheckNull( list );

// Vars
    etListElement*      listIterator = list->start;


#ifndef ET_DEBUG_OFF

    while( listIterator != NULL ){

            fprintf( stdout, "[ %p<- ]", listIterator->prev );
            fprintf( stdout, "[ %p ]", listIterator );
            fprintf( stdout, "[ ->%p ]", listIterator->next );
            fprintf( stdout, "[ %p ]", listIterator->data );

            fprintf( stdout, "\n" );

        listIterator = listIterator->next;
    }

#endif

    return etID_YES;
}



#ifdef __cplusplus
}
#endif



#endif
