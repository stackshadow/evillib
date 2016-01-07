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

#ifndef _H_etMemoryList
#define _H_etMemoryList

#include "core/etIDState.h"
#include "memory/etMemoryBlock.h"


/** @ingroup grMemoryList
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

*/
typedef struct etMemoryBlockList_s           etMemoryBlockList;

struct etMemoryBlockList_s {
    etMemoryBlock         *start;           /*!< The first etMemoryBlock */
    etMemoryBlock         *end;             /*!< The last etMemoryBlock */
    etMemoryBlock         *actual;          /*!< The last etMemoryBlock */
};



#define                     etMemoryBlockListAlloc( list ) __etMemoryBlockListAlloc( &list )
etID_STATE                  __etMemoryBlockListAlloc( etMemoryBlockList **p_list );

#define                     etMemoryBlockListFree( list ) __etMemoryBlockListFree( &list )
etID_STATE                  __etMemoryBlockListFree( etMemoryBlockList **p_list );


etID_STATE                  etMemoryBlockListFreeBlocks( etMemoryBlockList *list );


void                        etMemoryBlockListAppend( etMemoryBlockList *list, etMemoryBlock *block );

#define                     etMemoryBlockListBlockGet( list, memoryData, block ) __etMemoryBlockListBlockGet( list, memoryData, &block )
etID_STATE                  __etMemoryBlockListBlockGet( etMemoryBlockList *list, void *memoryData, etMemoryBlock **p_block );

#define                     etMemoryBlockListRequest( list, block, size ) __etMemoryBlockListRequest( list, &block, size )
etID_STATE                  __etMemoryBlockListRequest( etMemoryBlockList *list, etMemoryBlock **p_block, size_t size );


etID_STATE                  etMemoryBlockListDump( etMemoryBlockList *list );






#endif