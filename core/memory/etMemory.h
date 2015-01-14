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

#ifndef _H_evillib_etMemory
#define _H_evillib_etMemory

// Type
#define etID_MEM_TYPE_BYTE				char

typedef struct 			etMemoryListElement_t etMemoryListElement;









// Basics

etID_STATE				etMemoryInit();


void					etMemoryExit();




#define 				etMemoryAlloc( etMemoryBlock, size ) __etMemoryAlloc( &etMemoryBlock, size )
etID_STATE				__etMemoryAlloc( etMemoryBlock **p_etMemoryBlockActual, size_t size );

#define					etMemoryRequest( etMemoryBlock, size ) __etMemoryRequest( &etMemoryBlock, size )
etID_STATE				__etMemoryRequest( etMemoryBlock **p_etMemoryBlockActual, size_t size );

#define					etMemoryRelease( etMemoryBlock ) __etMemoryRelease( &etMemoryBlock )
void					__etMemoryRelease( etMemoryBlock **p_etMemoryBlockActual );

#define					etMemorySet( etMemoryBlock, dataSource, size ) __etMemorySet( &etMemoryBlock, dataSource, size )
etID_STATE				__etMemorySet( etMemoryBlock **p_etMemoryBlockActual, void *dataSource, size_t size );

#define					etMemorySetOffset( etMemoryBlock, dataSource, offset, size ) __etMemorySetOffset( &etMemoryBlock, dataSource, offset, size )
etID_STATE				__etMemorySetOffset( etMemoryBlock **p_etMemoryBlockActual, void *dataSource, size_t offset, size_t size );


// Optimisation

etID_STATE				etMemoryOptFreeUnused();


etID_STATE				etMemoryOptResort();



// Special things

etID_STATE				etMemoryDump( void *Userdata, void (*IteratorFunction)(	int state, size_t size ) );



























#endif

