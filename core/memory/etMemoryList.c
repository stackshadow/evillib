/* etMemory - The evillib memory subsystem
	Copyright (C) 2015 by Martin Langlotz alias stackshadow

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

#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemoryList.h"
#endif

/** @cond DEV */
/** @internal
@defgroup grMemoryList etMemoryList - List of etMemoryBlocks
@ingroup grMemory
@anchor etMemoryList
@brief The etMemory List
The etMemoryList holds all allocated etMemoryBlocks. \n
The idea behind that ist, that no memory gets lost and to reduce the call of alloc/free
*/


etMemoryList_*			etMemoryList = NULL;


/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Init the etMemoryList

The etMemoryList keeps track of all used and released Blocks. 
*/
void					etMemoryListInit(){
// Already init ?
	if( etMemoryList != NULL ){
		etDebugMessage( etID_LEVEL_INFO, "etMemoryList already init" );
		return;
	}

// Vars
	etMemoryBlock	*etMemoryBlockActual = NULL;

	
// Our first Block
	etMemoryBlockAlloc( etMemoryBlockActual, sizeof(etMemoryList_) ); // We dont need to check if alloc allocate memory, if we run oom the evillib exits directly
	etMemoryBlockDataGet( etMemoryBlockActual, etMemoryList );

	if( etMemoryList == NULL ) {
		etDebugState( etID_STATE_ERROR_INTERNAL );
		return;
	}

// Setup infos
	etMemoryList->blocks.start = etMemoryBlockActual;
	etMemoryList->blocks.end = etMemoryBlockActual;


// return
	return;	
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Free the etMemoryList
This function free all Blocks from the list and free the list-object itselfe
*/
void					etMemoryListFree(){
// Already free ?
	if( etMemoryList == NULL ){
		etDebugMessage( etID_LEVEL_INFO, "etMemoryList already freed" );
		return;
	}

// Vars
	etMemoryBlock	*etMemoryBlockActual = NULL;

// Free the Blocks inside the list
	etMemoryListFreeBlocks();

// Free the etList
	etMemoryBlockFromData( (void*)etMemoryList, etMemoryBlockActual );
	etMemoryBlockFree( etMemoryBlockActual );

// return
	etMemoryList = NULL;
	return;	
}

/** @ingroup grMemoryList
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Append an Block to the memory-list
@param etMemoryBlockActual - The etMemoryBlock to append to the list
*/
void					etMemoryListAppend( etMemoryBlock *etMemoryBlockActual ){

// Vars
	etMemoryList->blocks.end->next = etMemoryBlockActual;
	etMemoryList->blocks.end = etMemoryBlockActual;
	
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
etID_STATE				__etMemoryListGetFreeBlock( etMemoryBlock **p_etMemoryBlockFree, size_t size ){
// Vars
	etMemoryBlock	*etMemoryBlockIterator = NULL;
	etMemoryBlock	*etMemoryBlockActual = NULL;
	

	etMemoryBlockIterator = etMemoryList->blocks.start;
	while( etMemoryBlockIterator != NULL ){

		if( etMemoryBlockIterator->Properties & etID_MEM_STATE_FREE ){

		// Check if we found already one that fits
			if( etMemoryBlockActual != NULL ){
			// Check if the actual block is smaller that the founded one
				if( etMemoryBlockIterator->Size < etMemoryBlockActual->Size &&
					etMemoryBlockIterator->Size >= size ){
						etMemoryBlockActual = etMemoryBlockIterator;
					}
			}
			else {
				if( etMemoryBlockIterator->Size >= size ){
					etMemoryBlockActual = etMemoryBlockIterator;
				}
			}
		}


		etMemoryBlockIterator = etMemoryBlockIterator->next;
	}

// return
	*p_etMemoryBlockFree = etMemoryBlockActual;
	if( etMemoryBlockActual == NULL ){
		return etID_STATE_NODATA;
	}
	
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
etID_STATE				etMemoryListFreeBlocks(){
// Vars
	etMemoryBlock	*etMemoryBlockIterator = NULL;
	etMemoryBlock	*etMemoryBlockNext = NULL;
	

	etMemoryBlockIterator = etMemoryList->blocks.start->next;
	while( etMemoryBlockIterator != NULL ){
		etMemoryBlockNext = etMemoryBlockIterator->next;

		
		if( etMemoryBlockIterator->Properties & etID_MEM_STATE_ALLOCED ){
			etMemoryBlockFree(etMemoryBlockIterator);
		}


		etMemoryBlockIterator = etMemoryBlockNext;
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
etID_STATE				etMemoryListDump(){

// Vars
	etMemoryBlock	*etMemoryBlockIterator = NULL;
	

#ifndef ET_DEBUG_OFF
		
	etMemoryBlockIterator = etMemoryList->blocks.start;
	while( etMemoryBlockIterator != NULL ){

			fprintf( stdout, "[ %p->%p ]", etMemoryBlockIterator, etMemoryBlockIterator->next );

			if( etMemoryBlockIterator != NULL ){
				// Alloced / Virtual
				if( etMemoryBlockIterator->Properties & etID_MEM_STATE_ALLOCED ) {
					fprintf( stdout, "[ALLOCED]" );
				}
				else {
					fprintf( stdout, "[VIRTUAL]" );
				}

			// Parameter
				if( etMemoryBlockIterator->Properties & etID_MEM_STATE_FREE ) fprintf( stdout, "[FREE]" );
				if( etMemoryBlockIterator->Properties & etID_MEM_STATE_USED ) fprintf( stdout, "[USED]" );
				if( etMemoryBlockIterator->Properties & etID_MEM_STATE_LOCKED ) fprintf( stdout, "[LOCKED]" );
			}
			
			fprintf( stdout, "[%lu]", etMemoryBlockIterator->Size );

			
			fprintf( stdout, "\n" );
		
		etMemoryBlockIterator = etMemoryBlockIterator->next;
	}

#endif

	return etID_YES;
}


/** @endcond */

