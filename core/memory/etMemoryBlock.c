/*  Copyright (C) 2015 by Martin Langlotz alias stackshadow

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

#include "core/etObject.h"
#include "core/etDebug.h"
#include "memory/etMemoryBlock.h"


/** @internal
@defgroup grMemoryBlock etMemoryBlock - a single Block which hold the memory
@ingroup grMemory
The etMemoryBlock represents a single Block with allocated memory. \n
Use this function to set, copy or get data from/to the etMemoryBlock
*/










// Aloc / free

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etMemoryBlockAlloc( etMemoryBlockActual, size )

@~english
@brief Allocate a single etMemoryBlock
@param[out] etMemoryBlockActual The etMemoryBlock which will be allocated ( this pointer change its address !!! )
@param[in] size The amount of bytes to allocate 
@return If the etMemoryBlock was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*/
void                        __etMemoryBlockAlloc( etMemoryBlock **p_etMemoryBlock, size_t size ){
// Check
    etDebugCheckNullVoid( p_etMemoryBlock );
    
// Allocate new memory
    etMemoryBlock *etMemoryBlockNew = (etMemoryBlock*)malloc( sizeof(etMemoryBlock) );
    if( etMemoryBlockNew == NULL ){
        *p_etMemoryBlock = NULL;
        etDebugState( etID_STATE_CRIT_NOMEMORY );
        return;
    }
    memset( etMemoryBlockNew, 0, sizeof(etMemoryBlock) );

// set properties
    etMemoryBlockNew->state = etID_MEM_STATE_ALLOCED | etID_MEM_STATE_USED;
    etMemoryBlockNew->size = size;

// allocate payload
    etMemoryBlockNew->data = (etMemoryBlock*)malloc( size );
    if( etMemoryBlockNew->data == NULL ){
        *p_etMemoryBlock = NULL;
        etDebugState( etID_STATE_CRIT_NOMEMORY );
        return;
    }
    memset( etMemoryBlockNew->data, 0, size );


// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p alloced %li", etMemoryBlockNew, size );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif


// Return
    *p_etMemoryBlock = etMemoryBlockNew;
    return;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Release a etMemoryBlock for future use

If you release an etMemoryBlock it will be ready for future use and can be returned by etMemoryRequest(). \n
If ET_SECURE_MEMORY_OFF is not defined, the memory-block will be cleaned.

@param[in] etMemoryBlockActual The pointer to an etMemoryBlock object \n
*/
void                        etMemoryBlockRelease( etMemoryBlock *etMemoryBlockActual, etID_BOOL releaseIt ){
// Check
    etDebugCheckNullVoid( etMemoryBlockActual );


// Clean
    #ifndef ET_SECURE_MEMORY_OFF
    if( etMemoryBlockClean( etMemoryBlockActual ) != etID_YES ){
        etDebugState( etID_STATE_ERR_INTERR );
        return;
    }
    #endif

// Set properties
    if( releaseIt == etID_TRUE ){
        etMemoryBlockActual->state &= ~etID_MEM_STATE_USED;
        etMemoryBlockActual->state |= etID_MEM_STATE_FREE;

    // Debug
        #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p->state = free", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif

    } else {
        etMemoryBlockActual->state &= ~etID_MEM_STATE_FREE;
        etMemoryBlockActual->state |= etID_MEM_STATE_USED;        

    // Debug
        #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p->state = used", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif

    }



// Out
    return;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Free an etMemoryBlock

Free an etMemoryBlock directly. \n
This is for internaly use only !
@param[in] etMemoryBlockActual The pointer to an etMemoryBlock object \n
*/
void                        etMemoryBlockFree( etMemoryBlock *etMemoryBlockActual ){
    etDebugCheckNullVoid( etMemoryBlockActual );

    #ifndef ET_SECURE_MEMORY_OFF
    if( etMemoryBlockClean( etMemoryBlockActual ) != etID_YES ){
        etDebugState( etID_STATE_ERR_INTERR );
        return;
    }
    #endif


    if( etMemoryBlockActual->state & etID_MEM_STATE_ALLOCED ){
        free( etMemoryBlockActual->data );
    }

    
    free(etMemoryBlockActual);

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p freed", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Check if the etMemoryBlock is ready for use
If the etMemoryBlock is locked or in use, this function return etID_NO

@param[in] etMemoryBlockActual The pointer to an etMemoryBlock object
@return If the etMemoryBlock can be used \n
*- @ref etID_YES - etMemoryBlock can be used
*- @ref etID_NO - etMemoryBlock is locked or in use
*/
etID_STATE                  etMemoryBlockIsFree( etMemoryBlock *etMemoryBlockActual ){

// Block is in use
    if( ! (etMemoryBlockActual->state & etID_MEM_STATE_FREE) || (etMemoryBlockActual->state & etID_MEM_STATE_USED) ){

    // Debug
        #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p is already in use\n", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif

    // Out
         return etID_NO;
    }

// Block is locked
    if( etMemoryBlockActual->state & etID_MEM_STATE_LOCKED ){

        // Debug
        #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p is locked\n", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif

    // Out
        return etID_NO;
    }

    // Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p can be used\n", etMemoryBlockActual );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

    return etID_YES;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Check if the etMemoryBlock can hold a specific amount of memory

@param etMemoryBlockActual The pointer to an etMemoryBlock object
@param size The size to compare
@return If the etMemoryBlock can be used \n
*- @ref etID_YES - etMemoryBlock has enough space
*- @ref etID_NO - etMemoryBlock has not enough space
*/
etID_STATE                  etMemoryBlockHasSpace( etMemoryBlock *etMemoryBlockActual, size_t size ){
// Check
    if ( etMemoryBlockActual == NULL ){
        return etID_NO;
    }

// Check if size is enough
    if( size > etMemoryBlockActual->size ){

    // Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has only %li and can not hold %li", etMemoryBlockActual, etMemoryBlockActual->size, size );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif
    // Out
        return etID_NO;
    }

// Debug
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p has enoug space(%li) and can hold %li", etMemoryBlockActual, etMemoryBlockActual->size, size );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

// Out
    return etID_YES;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@def etMemoryBlockDataGetOffset( etMemoryBlockActual, offset, data )
@brief Get the date from an etMemoryBlock with an offset

@param etMemoryBlockActual The pointer to an etMemoryBlock object
@param offset The offset in bytes
@param data Pointer to an void object
@return The void-pointer with an offset which is allocated inside the etMemoryBlock \n
@warning Dont free or edit this pointer over the limit. This will crash your application. \n
*- The void-pointer with an offset
*- NULL if an error occure
*/
etID_STATE                  __etMemoryBlockDataGetOffset( etMemoryBlock *etMemoryBlockActual, size_t offset, void **data ){
// Check
    etDebugCheckNull( data );
    
    if( etMemoryBlockActual == NULL ){
        *data = NULL;
        return etID_STATE_NODATA;
    }

    if( offset >= etMemoryBlockActual->size ){
        *data = NULL;
        return etID_STATE_NODATA;
    }

    size_t sPointer = (size_t)etMemoryBlockActual->data;
    sPointer += offset;
    *data = (void*)sPointer;
// Out
    return etID_YES;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etMemoryBlockDataGet( etMemoryBlockActual, data )

@~english
@brief Return an void-pointer

You can use the returned void pointer directly, but at your own risk. \n
Pease use etMemoryBlockDataSet() to set the value of an etMemoryBlock \n

@param etMemoryBlockActual The pointer to an etMemoryBlock object
@param data Pointer to an void object
@return The void-pointer which is allocated inside the etMemoryBlock which holds your data \n
*- The void-pointer which represent the preallocated data
*- NULL if an error occure
*/
etID_STATE                  __etMemoryBlockDataGet( etMemoryBlock *etMemoryBlockActual, void **data ){
// Check
    etDebugCheckNull( etMemoryBlockActual );
    etDebugCheckNull( data );
    
    *data = etMemoryBlockActual->data;

    return etID_YES;
}


// Manipulate
/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Clean out an etMemoryBlock

@param etMemoryBlockActual The pointer to an etMemoryBlock object
@return If the data of the etMemoryBlock was correctly cleaned \n
*- @ref etID_YES - data of the etMemoryBlock was correctly cleaned
*- or an @ref etID_STATE
*/
etID_STATE                  etMemoryBlockClean( etMemoryBlock *etMemoryBlockActual ){
// Check
    etDebugCheckNull( etMemoryBlockActual );

// Overwrite Data ( only if not 0 )
    if( etMemoryBlockActual->size > 0 ){
        memset( etMemoryBlockActual->data, 0, etMemoryBlockActual->size );

        #ifndef ET_DEBUG_OFF
            snprintf( etDebugTempMessage, etDebugTempMessageLen, "%p cleaned", etMemoryBlockActual );
            etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
        #endif
    }

/* Future
    char *cPointer = (char*)etMemoryBlockPointer;

    int index = 0;
    for( index = 0; index < etMemoryBlockActual->Size; index++ ){
        cPointer[index] = 0;
    }
*/
// Out
    return etID_YES;
}

/** @ingroup grMemoryBlock
@internal
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Copy an etMemoryBlock to another etMemoryBlock
First the size if both blocks will be compared. \n
- if the destination is bigger than the source, the source size is used
- if the source is bigger than the destination, the destination size is used
- the size parameter limit the maximal copyed bytes

@warning the etMemoryBlockDest and etMemoryBlockSource get not cleaned !

@param etMemoryBlockDest The pointer to the destination etMemoryBlock-object
@param etMemoryBlockSource The pointer to the source etMemoryBlock-object from where the data will be copyed
@param size The maximum amount of bytes, which will be copyed
@return If the data was copyed from the source etMemoryBlock to the destination etMemoryBlock \n
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE                  etMemoryBlockCopy( etMemoryBlock *etMemoryBlockDest, etMemoryBlock *etMemoryBlockSource, size_t size ){
//Checks
    if( etMemoryBlockDest == NULL ) return etDebugState( etID_STATE_ERR_PARAMETER );
    if( etMemoryBlockSource == NULL ) return etDebugState( etID_STATE_ERR_PARAMETER );

// Vars
    size_t etMemoryBlockSizeCopy = 0;

// Target is bigger than the Source
    if( etMemoryBlockDest->size > etMemoryBlockSource->size )
        etMemoryBlockSizeCopy = etMemoryBlockSource->size;

// Source is bigger than the Target
    if( etMemoryBlockSource->size >= etMemoryBlockDest->size )
        etMemoryBlockSizeCopy = etMemoryBlockDest->size;

// Copy Size
    if( size < etMemoryBlockSizeCopy )
        etMemoryBlockSizeCopy = size;

// Copy the data
    if( etMemoryBlockSizeCopy > 0 ){
    // Data
        void *etMemoryBlockDstPointer = NULL;
        void *etMemoryBlockSrcPointer = NULL;
        
        etMemoryBlockDataGet( etMemoryBlockDest, etMemoryBlockDstPointer );
        etMemoryBlockDataGet( etMemoryBlockSource, etMemoryBlockSrcPointer );

    // Copy data
    #ifndef ET_DEBUG_OFF
        snprintf( etDebugTempMessage, etDebugTempMessageLen, "copy %li from %p to %p", etMemoryBlockSizeCopy, etMemoryBlockSource, etMemoryBlockDest );
        etDebugMessage( etID_LEVEL_DETAIL_MEM, etDebugTempMessage );
    #endif

        memcpy( etMemoryBlockDstPointer, etMemoryBlockSrcPointer, etMemoryBlockSizeCopy );
    }

// Out
    return etID_YES;
}





































