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

#ifndef _H_etMemory
#define _H_etMemory

#include "evillib_defines.h"
#include "memory/etMemoryBlock.h"


// Type
#define etID_MEM_TYPE_BYTE                char









// Basics

DLL_PUBLIC etID_STATE       etMemoryInit();


void                        etMemoryExit();




#define                     etMemoryAlloc( data, size ) __etMemoryAlloc( (void**)&data, size )
etID_STATE                  __etMemoryAlloc( void **p_data, size_t size );

#define                     etMemoryRequest( data, size ) __etMemoryRequest( (void**)&data, size )
etID_STATE                  __etMemoryRequest( void **p_data, size_t size );

#define                     etMemoryRelease( data ) __etMemoryRelease( (void**)&data )
void                        __etMemoryRelease( void **p_data );


etID_STATE                  etMemoryClean( void *data );

#define                     etMemorySet( dest, source, size ) __etMemorySet( (void**)&dest, source, size )
etID_STATE                  __etMemorySet( void **p_dest, void *source, size_t size );

#define                     etMemorySetOffset( data, dataSource, offset, size ) __etMemorySetOffset( (void**)&data, dataSource, offset, size )
etID_STATE                  __etMemorySetOffset( void **p_data, void *dataSource, size_t offset, size_t size );





// Special things

etID_STATE                  etMemoryDump( void *Userdata, void (*IteratorFunction)( int state, size_t size ) );



























#endif

