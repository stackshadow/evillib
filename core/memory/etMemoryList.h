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

#ifndef _H_etMemoryList
#define _H_etMemoryList


#include "memory/etMemoryBlock.h"


/** @ingroup grMemoryList
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

*/
typedef struct             etMemoryList_t {
    struct etMemoryListElement_t {
        etMemoryBlock         *start;        /*!< The first etMemoryBlock */
        etMemoryBlock         *end;        /*!< The last etMemoryBlock */
    } blocks;
} etMemoryList_;










void                    etMemoryListInit();


void                    etMemoryListFree();


void                    etMemoryListAppend( etMemoryBlock *etMemoryBlockActual );

#define                    etMemoryListGetFreeBlock( etMemoryBlockFree, size ) __etMemoryListGetFreeBlock( &etMemoryBlockFree, size )
etID_STATE                __etMemoryListGetFreeBlock( etMemoryBlock **p_etMemoryBlockFree, size_t size );


etID_STATE                etMemoryListFreeBlocks();


etID_STATE                etMemoryListDump();










#endif