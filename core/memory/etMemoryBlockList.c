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
#include "core/etObject.h"
#include "memory/etMemoryBlockList.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @internal
@ingroup grMemory
@{
@defgroup grMemoryList etMemoryList - List of etMemoryBlocks
@anchor etMemoryList
The etMemoryList holds all allocated etMemoryBlocks.
The idea behind that ist, that no memory gets lost and to reduce the call of alloc/free
Q}
*/






/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Init the etMemoryList

The etMemoryList keeps track of all used and released Blocks. 
*/
etID_STATE                  __etMemoryBlockListAlloc( etMemoryBlockList **p_list ){
// Already init ?
    etDebugCheckNull( p_list );


// Vars
    etMemoryBlock           *etMemoryBlockActual = NULL;
    etMemoryBlockList       *etMemoryList = NULL;

// Our first Block
    etMemoryBlockAlloc( etMemoryBlockActual, sizeof(etMemoryBlockList) ); // We dont need to check if alloc allocate memory, if we run oom the evillib exits directly
    etMemoryBlockDataGet( etMemoryBlockActual, etMemoryList );

    if( etMemoryList == NULL ) {
        return etDebugState( etID_STATE_ERR_INTERR );
    }

// Of course the first element is the new allocated memory for the list
    etMemoryList->start = etMemoryBlockActual;
    etMemoryList->end = etMemoryBlockActual;
    etMemoryList->actual = etMemoryBlockActual;

// return
    *p_list = etMemoryList;
    return etID_YES;
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Free the etMemoryList
This function free all Blocks from the list and free the list-object itselfe
*/
etID_STATE                  __etMemoryBlockListFree( etMemoryBlockList **p_list ){
// Already init ?
    etDebugCheckNull( p_list );
    etDebugCheckNull( *p_list );


// free the blocks in the list
    etMemoryBlockListFreeBlocks( *p_list );

// free the list itselfe
    etMemoryBlockFree( (*p_list)->start );


// return
    *p_list = NULL;
    return etID_YES;    
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Free all blocks inside the memory-list
@return \n
*- @ref etID_YES
*/
etID_STATE                  etMemoryBlockListFreeBlocks( etMemoryBlockList *list ){
// Check
    etDebugCheckNull( list );

// Vars
    etMemoryBlock    *etMemoryBlockActual = NULL;
    etMemoryBlock    *etMemoryBlockNext = NULL;
    

    etMemoryBlockActual = list->start->next;
    while( etMemoryBlockActual != NULL ){
        etMemoryBlockNext = etMemoryBlockActual->next;

        
        if( etMemoryBlockActual->state & etID_MEM_STATE_ALLOCED ){
            etMemoryBlockFree(etMemoryBlockActual);
        }


        etMemoryBlockActual = etMemoryBlockNext;
    }


    return etID_YES;
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Append an Block to the memory-list
@param etMemoryBlockActual - The etMemoryBlock to append to the list
*/
void                        etMemoryBlockListAppend( etMemoryBlockList *list, etMemoryBlock *block ){
// Vars
    list->end->next = block;
    list->end = block;
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etMemoryBlockListBlockGet( etMemoryBlockList *list, void *memoryData, etMemoryBlock *block );
@~english

@brief Append an Block to the memory-list



@param[in] list
@param[in] memoryData
@param[out] block

*/
etID_STATE                  __etMemoryBlockListBlockGet( etMemoryBlockList *list, void *memoryData, etMemoryBlock **p_block ){
// Check
    etDebugCheckNull( list );
    etDebugCheckNull( memoryData );
    etDebugCheckNull( p_block );

// vars
    etMemoryBlock           *memoryBlock;
    
    memoryBlock = list->start->next;
    while( memoryBlock != NULL ){
        
        if( memoryBlock->data == memoryData ){
            *p_block = memoryBlock;
            
            #ifndef ET_DEBUG_OFF
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p: Found pointer %p in the list: %p", list, memoryData, memoryBlock );
            etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
            #endif
            return etID_YES;
        }
        
        memoryBlock = memoryBlock->next;
    }

    #ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p: Not found pointer %p in the list", list, memoryData );
    etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

// we found nothing return
    *p_block = NULL;
    return etID_STATE_NODATA;
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@def etMemoryListGetFreeBlock( etMemoryBlockFree, size )
@~english
@brief Append an Block to the memory-list
@param etMemoryBlockFree - The etMemoryBlock which can hold your size or NULL if no released Block was found
@param size - The requested size
@return \n
*- @ref etID_YES
*- @ref etID_STATE_NODATA
*/
etID_STATE                  __etMemoryBlockListRequest( etMemoryBlockList *list, etMemoryBlock **p_block, size_t size ){
// Check
    etDebugCheckNull( list );
    etDebugCheckNull( p_block );

// Vars
    etMemoryBlock    *blockIterator = NULL;
    etMemoryBlock    *blockCanditate = NULL;
    

    blockIterator = list->start->next;
    while( blockIterator != NULL ){

        if( blockIterator->state & etID_MEM_STATE_FREE ){

        // Check if we found already one that fits
            if( blockCanditate != NULL ){
            // Check if the actual block is smaller that the founded one
                if( blockIterator->size < blockCanditate->size &&
                    blockIterator->size >= size ){
                        blockCanditate = blockIterator;
                    }
            }
            else {
                if( blockIterator->size >= size ){
                    blockCanditate = blockIterator;
                }
            }
        }


        blockIterator = blockIterator->next;
    }

// return
    *p_block = blockCanditate;
    if( blockCanditate == NULL ){
        return etID_STATE_NODATA;
    }
    
    return etID_YES;
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Dump the informations about all Blocks inside the list to stdout
@return \n
*- @ref etID_YES
*/
etID_STATE                  etMemoryBlockListDump( etMemoryBlockList *list ){
// Check
    etDebugCheckNull( list );

// Vars
    etMemoryBlock    *blockIterator = list->start;


#ifndef ET_DEBUG_OFF
        
    blockIterator = list->start;
    while( blockIterator != NULL ){

            fprintf( stdout, "[ %p->%p ]", blockIterator, blockIterator->next );

            if( blockIterator != NULL ){
                // Alloced / Virtual
                if( blockIterator->state & etID_MEM_STATE_ALLOCED ) {
                    fprintf( stdout, "[ALLOCED]" );
                }
                else {
                    fprintf( stdout, "[VIRTUAL]" );
                }

            // Parameter
                if( blockIterator->state & etID_MEM_STATE_FREE ) fprintf( stdout, "[FREE]" );
                if( blockIterator->state & etID_MEM_STATE_USED ) fprintf( stdout, "[USED]" );
                if( blockIterator->state & etID_MEM_STATE_LOCKED ) fprintf( stdout, "[LOCKED]" );
            }
            
            fprintf( stdout, "[%lu]", blockIterator->size );

            
            fprintf( stdout, "\n" );
        
        blockIterator = blockIterator->next;
    }

#endif

    return etID_YES;
}




#ifdef __cplusplus
}
#endif
