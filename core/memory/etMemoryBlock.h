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

#ifndef _H_evillib_etMemoryBlock
#define _H_evillib_etMemoryBlock

/** @cond DEV */
/** @ingroup grMemoryBlock
@defgroup grMemoryBlockStates States of memory blocks

@ingroup grMemoryBlockStates
@{
*/
// States
	#define etID_MEM_STATE_ALLOCED			1		/**< Memory Block is allocated by alloc */
	#define etID_MEM_STATE_FREE				2		/**< Memory Block is free for request */
	#define etID_MEM_STATE_USED				4		/**< Memory Block is in use */
	#define etID_MEM_STATE_LOCKED			8		/**< Memory Block is locked by an process */
	#define etID_MEM_STATE_ERROR			128		/**< Memory Block is on error state */
/* @} */




/** @ingroup grMemoryBlock
@~english

@brief This object, define one Block of memory inside the memory-system
*/
typedef struct etMemoryBlock etMemoryBlock;
typedef struct 		etMemoryBlock {
	int					Properties;				/**< Properties of the Block @see grMemoryBlockStates */
	size_t				Size;					/**< The size in bytes */

	etMemoryBlock		*next;					/**< The next Block in the line */
} etMemoryBlock;



// Aloc / free
#define					etMemoryBlockAlloc( etMemoryBlockActual, size ) __etMemoryBlockAlloc( &etMemoryBlockActual, size )
void					__etMemoryBlockAlloc( etMemoryBlock **p_etMemoryBlock, size_t size );


void					etMemoryBlockRelease( etMemoryBlock *etMemoryBlockActual );


void					etMemoryBlockFree( etMemoryBlock *etMemoryBlockActual );


etID_STATE				etMemoryBlockIsFree( etMemoryBlock *etMemoryBlockActual );


etID_STATE				etMemoryBlockHasSpace( etMemoryBlock *etMemoryBlockActual, size_t size );

#define					etMemoryBlockDataGetOffset( etMemoryBlockActual, offset, data ) __etMemoryBlockDataGetOffset( etMemoryBlockActual, offset, (void**)(&data) );
etID_STATE				__etMemoryBlockDataGetOffset( etMemoryBlock *etMemoryBlockActual, size_t offset, void **data );

#define					etMemoryBlockDataGet( etMemoryBlockActual, data ) __etMemoryBlockDataGet( etMemoryBlockActual, (void**)(&data) );
etID_STATE				__etMemoryBlockDataGet( etMemoryBlock *etMemoryBlockActual, void **data );

#define					etMemoryBlockFromData( data, etMemoryBlockActual ) __etMemoryBlockFromData( data, &etMemoryBlockActual );
etID_STATE				__etMemoryBlockFromData( void *data, etMemoryBlock **p_etMemoryBlockActual );



// Manipulate

etID_STATE				etMemoryBlockClean( etMemoryBlock *etMemoryBlockActual );


etID_STATE				etMemoryBlockCopy( etMemoryBlock *etMemoryBlockDest, etMemoryBlock *etMemoryBlockSource, size_t size );


etID_STATE				etMemoryBlockDataSet( etMemoryBlock *etMemoryBlockActual, void *data, size_t offset, size_t size );




















/** @endcond */
#endif

