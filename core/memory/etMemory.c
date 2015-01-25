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
	#include "memory/etMemory.h"
#endif

/** @cond DEV */
/** @defgroup grMemoryCore etMemory - Core functions
@ingroup grMemory
@anchor etMemory
@brief The memory subsystem of the evillib
The etMemory system handle memory allocation, freeing and sorting of memory. \n
Optimisation and reordering is also included, but not yet automatically.

# How it works
Mainly the memory-system holds information about an memory block: \n
  * the state \n
  * the size \n
All used and unused blocks, are connected to one linked-list, which is defined global and is itselfe an etMemoryBlock which gets init on etMemoryInit(). \n
If you release an memory-block with etMemoryBlockRelease() it will not be freed, it is marked as aviable memory and is aviable for etMemoryRequest().
Depending if @ref ET_SECURE_MEMORY_OFF is defined, the released Block get overwritten with zeros.

@todo Provide two lists, to speed up allocation / release
@todo enable automatic optimisation

# Example
This is the normal use-case when you need Memory which is supervised by the evillib:
@snippet etMemory.c etMemory request

## Description
As you see in the example, we first request an new Memory-Block with etMemoryRequest() \n
The second command will return you an void-pointer which you can use in your program. \n
@warning Don't write to the void pointer directly, use etMemorySet()
*/

/** @ingroup grMemoryCore
@def ET_SECURE_MEMORY_OFF
Disable secure memory \n
This disables some features which result in a secure memory. \n
For example it disables the cleaning of allocated memory when you release it to the etMemory system
*/

// Basics
/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Init the etMemory-System

This is a NEEDED function. Please call this before you do something with the etMemory. \n
This function will be also invoced by etInit() \n
*/
etID_STATE				etMemoryInit(){



// Debug
#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "Some infos:\nsizeof(etMemoryBlock)=%li \n", sizeof(etMemoryBlock) );
	etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
#endif

// Warning if secure-memory is disabled
#ifdef ET_SECURE_MEMORY_OFF
	etDebugMessage( etID_LEVEL_WARNING, "Secure Memory is disabled!!! This can harm your security!" );
#endif

// Check if char = 1 BYTE
	if( sizeof(char) != 1 ){
		etDebugMessage( etID_LEVEL_CRITICAL, "sizeof(char) != 1\n" );
	}

// Check if a jump of 15 is the same as pointer
// Char Jump
	int TestValue = 1024;
	char *CharPointer = (char*)(&TestValue);
	CharPointer = CharPointer + 15;

// Size Jump
	size_t sPointer = (size_t)(&TestValue);
	sPointer = sPointer + 15;
	char *SizePointer = (char*)sPointer;

// Check
	if( (size_t)CharPointer != (size_t)SizePointer ){
		etDebugMessage( etID_LEVEL_CRITICAL, "(size_t)CharPointer != (size_t)SizePointer\n" );
	}


// Allocate lists
	etMemoryListInit();




//	etDebugMessage( etID_LEVEL_DETAIL_MEM, NULL );
//	fprintf( stdout, "Standart Frame size:%li First Frame:%p \n", etMemoryGlobal->StadartFrameSize, etMemoryGlobal->FrameFirst );

// Register the atexit function
	atexit(etMemoryExit);


	etDebugMessage( etID_LEVEL_INFO, "Memory System initialised.." );


	return etID_YES;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief free the whole Memory of the etMemory-System. \n
DONT USE THE MEMORY SYSTEM ANYMORE. If you would like to use the etMemory-System again, use etMemoryInit()
*/
void					etMemoryExit(){

	
	etMemoryDump( NULL, NULL );
	etMemoryListFree();


	etDebugMessage( etID_LEVEL_INFO, "Memory System deinitialised." );
}



// Aloc / free
/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@fn etID_STATE etMemoryAlloc( void *data, size_t size )
@brief Allocate a new etMemoryBlock

This allocate a new memory block. Only use this function if you really need to allocate a new Block \n
This function will not look for unused, released blocks.
If you would like to use released, unused Blocks, use the function etMemoryRequest()

@param[out] data The new allocated data, just use it ;)
@param size Size of the requested Block in Bytes
@return If the etMemory object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_LEVEL_CRITICAL ( If the memory system was not init before )
*- @ref etID_STATE_NOMEMORY
*/
etID_STATE				__etMemoryAlloc( etMemoryBlock **p_etMemoryBlockActual, size_t size ){
// Check
	etCheckNull(p_etMemoryBlockActual);

// Allocate a new Block
	__etMemoryBlockAlloc( p_etMemoryBlockActual, size );
	etMemoryListAppend( *p_etMemoryBlockActual );

	return etID_YES;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etMemoryRequest( etMemoryBlock, size )

@~english
@brief Request a new Block

If you release a memory Block, it will not be freed. Its marked as spare-memory-block. \n
This function will first search for any spare-memory-block which can hold your requested size. \n
If no spare is present, a new Block will be allocated. \n
To optimize the searching for a spare-memory-block the function etMemoryResort() can be used.

@param etMemoryBlock
@param size Size of the requested Block in Bytes
@return \n
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE				__etMemoryRequest( etMemoryBlock **p_etMemoryBlockActual, size_t size ){
// Check
	etCheckNull(p_etMemoryBlockActual);

// Vars
	etMemoryBlock *etMemoryBlockActual = NULL;


// Get a free Block (if possible) from the list
	etMemoryListGetFreeBlock( etMemoryBlockActual, size );


// We found a Block
	if( etMemoryBlockActual != NULL ){

	#ifndef ET_DEBUG_OFF
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has %li bytes, enough for %li bytes", etMemoryBlockActual, etMemoryBlockActual->Size, size );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	#endif

	// Set Properties
		etMemoryBlockActual->Properties &= ~etID_MEM_STATE_FREE; // unset free
		etMemoryBlockActual->Properties |= etID_MEM_STATE_USED; // set used
		
		*p_etMemoryBlockActual = etMemoryBlockActual;
		return etID_YES;
	}

	__etMemoryAlloc( p_etMemoryBlockActual, size);
	return etID_YES;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etMemoryRelease( etMemoryBlock )

@~english
@brief Release a etMemoryBlock for future use

If you release an etMemoryBlock it will be ready for future use and can be returned by etMemoryRequest(). \n
This reduce the amount of allocs. \n
This function trigger also an free or an resorting of the memory for optimisation. ( if its enabled )

@param etMemoryBlock The pointer to an etMemoryBlock object
@return If the etMemoryBlock object was released \n
*- @ref etID_YES - etMemoryBlock object was released
*- or an @ref etID_STATE
*/
void					__etMemoryRelease( etMemoryBlock **p_etMemoryBlockActual ){
	etCheckNullVoid(p_etMemoryBlockActual);

	etMemoryBlockRelease( *p_etMemoryBlockActual );
	*p_etMemoryBlockActual = NULL;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@fn etMemorySet( void *dataActual, void *dataSource, size_t offset, size_t size )
@brief Set the data inside an etMemoryBlock from an void pointer with an offset

You must specify the size which you would like to copy to the etMemoryBlock. \n

@param[in,out] dataActual Data-Pointer which was allocated from etMemoryRequest() or etMemoryAlloc()
@param[in] dataSource The data which will be copyed to the etMemoryBlock
@param[in] offset The offset ( in Bytes ) inside the data of the etMemoryBlock where to start with the copy
@param[in] size The size ( in Bytes ) of the data you will copy
@return If the data was copyed from the void-pointer to the etMemoryBlock \n
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE				__etMemorySet( etMemoryBlock **p_etMemoryBlockActual, void *dataSource, size_t size ){
//Checks
	etCheckNull( p_etMemoryBlockActual );
	etCheckNull( *p_etMemoryBlockActual );
	if( size == 0 ) return etID_YES;

// Vars
	etMemoryBlock 	*etMemoryBlockActual = *p_etMemoryBlockActual;
	void			*etMemoryBlockData = NULL;

// Not enough size
	if( etMemoryBlockHasSpace(etMemoryBlockActual,size) != etID_YES ){
		
	// Release the old one
		etMemoryBlockRelease( etMemoryBlockActual );
		
	// Debug
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has %li and can not hold %li. Request a new one", etMemoryBlockActual, etMemoryBlockActual->Size, size );
			etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
		#endif
		
	// Request new memory
		etMemoryRequest( etMemoryBlockActual, size );
	}

// Copy the rest
	etMemoryBlockDataGet(etMemoryBlockActual,etMemoryBlockData);
	memcpy( etMemoryBlockData, dataSource, size );


// Return
	*p_etMemoryBlockActual = etMemoryBlockActual;
	return etID_YES;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@fn etMemorySet( void *dataActual, void *dataSource, size_t offset, size_t size )
@brief Set the data inside an etMemoryBlock from an void pointer with an offset

You must specify the size which you would like to copy to the etMemoryBlock. \n

@param[in,out] dataActual Data-Pointer which was allocated from etMemoryRequest() or etMemoryAlloc()
@param[in] dataSource The data which will be copyed to the etMemoryBlock
@param[in] offset The offset ( in Bytes ) inside the data of the etMemoryBlock where to start with the copy
@param[in] size The size ( in Bytes ) of the data you will copy
@return If the data was copyed from the void-pointer to the etMemoryBlock \n
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE				__etMemorySetOffset( etMemoryBlock **p_etMemoryBlockActual, void *dataSource, size_t offset, size_t size ){
//Checks
	etCheckNull( p_etMemoryBlockActual );
	etCheckNull( *p_etMemoryBlockActual );

	if( size == 0 ) return etID_YES;

// Vars
	etMemoryBlock 	*etMemoryBlockActual = *p_etMemoryBlockActual;
	void			*etMemoryBlockData = NULL;

// Not enough size
	if( etMemoryBlockHasSpace(etMemoryBlockActual,offset + size) != etID_YES ){

	// Debug
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has %li and can not hold %li. Request a new one", etMemoryBlockActual, etMemoryBlockActual->Size, size );
			etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
		#endif

	// We allocate a new Block
		etMemoryBlock *etMemoryBlockNew = NULL;
		etMemoryRequest( etMemoryBlockNew, offset + size );
	
	// Copy
		etMemoryBlockCopy( etMemoryBlockNew, etMemoryBlockActual, etMemoryBlockActual->Size );
	
	// Release the old one
		etMemoryBlockRelease( etMemoryBlockActual );
		etMemoryBlockActual = etMemoryBlockNew;
	}

// Copy the data
	#ifndef ET_DEBUG_OFF
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "copy %li to %p with an offset of %li", size, etMemoryBlockActual, offset );
		etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
	#endif

// Get data with offset
	etMemoryBlockDataGetOffset( etMemoryBlockActual, offset, etMemoryBlockData );
	if( etMemoryBlockData == NULL )  return etID_NO;

// Copy the rest
	memcpy( etMemoryBlockData, dataSource, size );

/* Future
	char *cPointer = (char*)sPointer;
	char *cPointerData = (char*)data;

	int index = 0;
	for( index = 0; index < size; index++ ){
		cPointer[index] = cPointerData[index];
	}
*/

// Return
	*p_etMemoryBlockActual = etMemoryBlockActual;
	return etID_YES;
}


// Optimisation
/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Free unused spare-memory-blocks

@return If function work normaly \n
*- @ref etID_YES - Everything okay
*- @ref etID_LEVEL_CRITICAL - Memory system not init
*/
etID_STATE				etMemoryOptFreeUnused(){


// Append
#ifndef ET_DEBUG_OFF
	etDebugMessage( etID_LEVEL_DETAIL_MEM, "Start free unused memory\n" );
#endif
/*
	etMemoryDump( NULL, NULL );

	etMemoryBlock *etMemoryBlockInUse = etMemory->BlockFirst;
	etMemoryBlock *etMemoryBlockIterator = etMemory->BlockFirst;
	etMemoryBlock *etMemoryBlockNextOld = NULL;

	while( etMemoryBlockIterator != NULL ){

		if( etMemoryBlockIterator->Properties & etID_MEM_STATE_USED ){
			etMemoryBlockInUse = etMemoryBlockIterator;
		}

		if( etMemoryBlockIterator->Properties & etID_MEM_STATE_FREE ){

		// For debugging
			etMemoryBlockNextOld = etMemoryBlockInUse->Next;
			etMemoryBlockInUse->Next = etMemoryBlockIterator->Next;

			snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p was ->%p now ->%p", etMemoryBlockInUse, etMemoryBlockNextOld, etMemoryBlockInUse->Next );
			etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );


		// Free
			etMemoryBlockIterator->Size = 0;
			etMemoryBlockIterator->Next = NULL;
			free( etMemoryBlockIterator );

		// Go to next
			etMemoryBlockIterator = etMemoryBlockInUse;
		}




		etMemoryBlockIterator = etMemoryBlockIterator->Next;
	}

	etMemoryDump( NULL, NULL );
*/
	return etID_YES;
}

/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Resort etMemoryBlocks

This is to optimize the structure of the etMemory-system \n
The spare-part-blocks will be sorted to the beginning, to use etMemoryRequest() faster.

@return If function work normaly \n
*- @ref etID_YES - Everything okay
*- @ref etID_LEVEL_CRITICAL - Memory system not init
*/
etID_STATE				etMemoryOptResort(){


	return etID_YES;
}



// Special things
/** @ingroup grMemoryCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Print out the whole etMemory-structure

@return If function work normaly \n
*- @ref etID_YES - Everything okay
*- @ref etID_LEVEL_CRITICAL - Memory system not init
*/
etID_STATE				etMemoryDump( void *Userdata, void (*IteratorFunction)(	int state, size_t size ) ){


	etMemoryListDump();

// Out
	return etID_YES;
}






/** @endcond */













