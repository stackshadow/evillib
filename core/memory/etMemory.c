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

#include "evillib_depends.h"
#include "evillib_version.h"

#include "memory/etMemory.h"


#include "core/etObject.h"
#include "core/etDebug.h"
#include "memory/etMemoryBlockList.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup grMemory etMemory - Memory management inside the evillib
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

@warning Don't write to the void pointer directly, use etMemorySet()
*/

/** @ingroup compileOptions
@def ET_SECURE_MEMORY_OFF
This define works only on compile time of the evillib itselfe, not inside an userspace program !.
Disable secure memory \n
This disables some features which result in a secure memory. \n
For example it disables the cleaning of allocated memory when you release it to the etMemory system \n
@warning Be very careful of disabling this function. Don't use this in an security related application ( e.g. handling with passwords )
*/


etMemoryBlockList           *etMemoryList;

// Basics
/** @ingroup grMemory
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Init the etMemory-System

Normally this function is not needed in your userspace program because etInit() calls it \n
*/
etID_STATE                  etMemoryInit(){



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
    etMemoryBlockListAlloc( etMemoryList );




//    etDebugMessage( etID_LEVEL_DETAIL_MEM, NULL );
//    fprintf( stdout, "Standart Frame size:%li First Frame:%p \n", etMemoryGlobal->StadartFrameSize, etMemoryGlobal->FrameFirst );

// Register the atexit function
    atexit(etMemoryExit);


    etDebugMessage( etID_LEVEL_INFO, "Memory System initialised.." );


    return etID_YES;
}

/** @ingroup grMemory
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief free the whole Memory of the etMemory-System. \n
DONT USE THE MEMORY SYSTEM ANYMORE. If you would like to use the etMemory-System again, use etMemoryInit()
*/
void                        etMemoryExit(){


    etMemoryBlockListDump( etMemoryList );
    etMemoryBlockListFree( etMemoryList );


    etDebugMessage( etID_LEVEL_INFO, "Memory System deinitialised." );
}



// Aloc / free
/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn etID_STATE etMemoryAlloc( void *data, size_t size )
@~english
@brief Allocate a new etMemoryBlock

This allocate a new memory block. Only use this function if you really need to allocate a new Block \n
This function will not look for unused, released blocks.
If you would like to use released, unused Blocks, use the function etMemoryRequest()

@param[out] data Returns the pointer of the new allocated memory
@param[in] size Size of the requested Block in Bytes
@return If the etMemory object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                  __etMemoryAlloc( void **p_data, size_t size ){
// Check
    etDebugCheckNull( p_data );

// Allocate a new Block
    etMemoryBlock       *memoryBlockNew = NULL;
    etMemoryBlockAlloc( memoryBlockNew, size );

// append the block to the list
    etMemoryBlockListAppend( etMemoryList, memoryBlockNew );

    *p_data = memoryBlockNew->data;
    return etID_YES;
}

/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn etID_STATE etMemoryRequest( void *data, size_t size )
@~english
@brief Request a new Block

If you release a memory Block, it will not be freed. Its marked as spare-memory-block. \n
This function will first search for any spare-memory-block which can hold your requested size. \n
If no spare is present, a new Block will be allocated. \n

@todo Don't save the position of the next block inside the block ( to avoid manipulation ) \n Do this in the etMemoryList !

@param[out] data Returns the pointer of the new allocated memory
@param size Size of the requested Block in Bytes
@return \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                  __etMemoryRequest( void **p_data, size_t size ){
// Check
    etDebugCheckNull(p_data);

// Vars
    etMemoryBlock       *etMemoryBlockActual = NULL;
    void                *data = NULL;
    
// Get a free Block (if possible) from the list
    etMemoryBlockListRequest( etMemoryList, etMemoryBlockActual, size );


// We found a Block
    if( etMemoryBlockActual != NULL ){

        #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has %li bytes, enough for %li bytes", etMemoryBlockActual, etMemoryBlockActual->size, size );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif
        
        // this will set the block to used !
        etMemoryBlockRelease( etMemoryBlockActual, etID_FALSE );
        etMemoryBlockDataGet( etMemoryBlockActual, data );
    } else {
        etMemoryAlloc( data, size );
    }

// no new Block found, allocate a new one
    *p_data = data;
    if( data == NULL ){
        return etID_NO;
    }
    return etID_YES;
}

/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@fn void etMemoryRelease( void *data )
@~english
@brief Release a etMemoryBlock for future use

If you release an etMemoryBlock it will be ready for future use and can be returned by etMemoryRequest(). \n
This reduce the amount of allocs.
 * 
@param[out] data Pointer to memory which was allocated with etMemoryRequest() or etMemoryAlloc()
@return If the etMemoryBlock object was released \n
*- @ref etID_YES - etMemoryBlock object was released
*- or an @ref etID_STATE
*/
void                        __etMemoryRelease( void **p_data ){
    etDebugCheckNullVoid(p_data);
    etDebugCheckNullVoid(*p_data);

// vars
    etMemoryBlock           *memoryBlock = NULL;
    void                    *data = *p_data;

// get the etMemoryBlock from p_data
    if( etMemoryBlockListBlockGet( etMemoryList, data, memoryBlock ) != etID_YES ){
        etDebugState( etID_STATE_ERR_INTERR );
        return;
    }

    etMemoryBlockRelease( memoryBlock, etID_TRUE );
    *p_data = NULL;
}


etID_STATE                  etMemoryClean( void *data ){
    etDebugCheckNull(data);

// vars
    etMemoryBlock           *memoryBlock = NULL;

// get the etMemoryBlock from p_data
    if( etMemoryBlockListBlockGet( etMemoryList, data, memoryBlock ) != etID_YES ){
        return etDebugState( etID_STATE_ERR_INTERR );
    }
    
    if( etMemoryBlockClean( memoryBlock ) != etID_YES ){
        return etDebugState( etID_STATE_ERR_INTERR );
    }
    
    return etID_YES;
}

/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etMemorySet( void *dest, void *source, size_t size )

@~english
@brief Set the data inside an etMemoryBlock from an void pointer

This function copy the memory block from source to dest with an amount of size. \n
If the dest memory can not hold the amount of size ( in bytes ) a new memory block will be alloced. \n
So the dest-pointer can be different after call of this function

@param[in,out] dest Pointer to an memory which was allocated with etMemoryAlloc() or etMemoryRequest()
@param[in] source The data which will be copyed to the etMemoryBlock
@param[in] size The size ( in Bytes ) of the data you will copy
@return If the data was copyed from the void-pointer to the etMemoryBlock \n
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE                  __etMemorySet( void **p_dest, void *source, size_t size ){
//Checks
    etDebugCheckNull( p_dest );
    etDebugCheckNull( *p_dest );
    etDebugCheckNull( source );
    if( size == 0 ) return etID_YES;

// Vars
    etMemoryBlock       *etMemoryBlockActual = NULL;
    void                *etMemoryBlockData = *p_dest;

// get etMemoryBlock from data-pointer
    if( etMemoryBlockListBlockGet( etMemoryList, etMemoryBlockData, etMemoryBlockActual ) != etID_YES ){
        return etDebugState( etID_STATE_ERR_INTERR );
    }

// Not enough size
    if( etMemoryBlockHasSpace(etMemoryBlockActual,size) != etID_YES ){
        
    // Release the old one
        etMemoryBlockRelease( etMemoryBlockActual, etID_TRUE );

    // Request new memory
        etMemoryRequest( etMemoryBlockData, size );
    }

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p set %li bytes from %p", etMemoryBlockData, size, source );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

// Copy the rest
    memcpy( etMemoryBlockData, source, size );


// Return
    *p_dest = etMemoryBlockData;
    return etID_YES;
}

/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etMemorySetOffset( void *data, void *dataSource, size_t offset, size_t size )

@~english
@brief Set the data inside an etMemoryBlock from an void pointer with an offset

This function checks if the etMemoryBlock can hold your requested size + offset and allocate a new block if needed

@param[in,out] data Pointer to an memory which was allocated with etMemoryAlloc() or etMemoryRequest()
@param[in] dataSource The data which will be copyed to data
@param[in] offset The offset ( in Bytes ) inside the data of the etMemoryBlock where to start with the copy
@param[in] size The size ( in Bytes ) of the data you will copy
@return If the data was copyed from the void-pointer to the etMemoryBlock \n
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE                  __etMemorySetOffset( void **p_data, void *dataSource, size_t offset, size_t size ){
//Checks
    etDebugCheckNull( p_data );
    if( size == 0 ) return etID_YES;

// Vars
    etMemoryBlock       *etMemoryBlockActual = NULL;
    void                *etMemoryBlockData = *p_data;
    
    
    
// get the memory block from the data
    etMemoryBlockListBlockGet( etMemoryList, etMemoryBlockData, etMemoryBlockActual );

// Not enough size
    if( etMemoryBlockHasSpace(etMemoryBlockActual,offset + size) != etID_YES ){

    // Debug
        #ifndef ET_DEBUG_OFF
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has %li and can not hold %li. Request a new one", etMemoryBlockActual, etMemoryBlockActual->size, size );
            etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif

    // We allocate a new Block
        void *etMemoryBlockDataNew = NULL;
        etMemoryRequest( etMemoryBlockDataNew, offset + size );

    // Copy
        memcpy( etMemoryBlockDataNew, etMemoryBlockData, offset );
    
    // Release the old one
        etMemoryBlockRelease( etMemoryBlockActual, etID_TRUE );
        
    //
        etMemoryBlockData = etMemoryBlockDataNew;
    }

// Copy the data
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "copy %li to %p with an offset of %li", size, etMemoryBlockActual, offset );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

// calculate the pointer
    size_t sPointer = (size_t)etMemoryBlockData;
    sPointer += offset;
    void *etMemoryBlockDataOffset = (void*)sPointer;

// Copy the rest
    memcpy( etMemoryBlockDataOffset, dataSource, size );

/* Future
    char *cPointer = (char*)sPointer;
    char *cPointerData = (char*)data;

    int index = 0;
    for( index = 0; index < size; index++ ){
        cPointer[index] = cPointerData[index];
    }
*/

// Return
    *p_data = etMemoryBlockData;
    return etID_YES;
}


// Optimisation

etID_STATE                  etMemoryBlockGet( void *dataSource, etMemoryBlock **p_etMemoryBlockActual ){
    return __etMemoryBlockListBlockGet( etMemoryList, dataSource, p_etMemoryBlockActual );
}
// Special things
/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Print out the whole etMemory-structure

@return If function work normaly \n
*- @ref etID_YES - Everything okay
*- @ref etID_LEVEL_CRITICAL - Memory system not init
*/
etID_STATE                  etMemoryDump( void *Userdata, void (*IteratorFunction)(    int state, size_t size ) ){


    etMemoryBlockListDump( etMemoryList );

// Out
    return etID_YES;
}



#ifdef __cplusplus
}
#endif











