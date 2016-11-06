/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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

#ifndef _H_etList
#define _H_etList

#include "evillib_depends.h"
#include "evillib_defines.h"
#include "core/etIDState.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup grMemoryList
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

*/
typedef struct etList_s etList;

struct etList_s {
    etList*     prev;
    etList*     next;
    void*       data;
};



#define         etListAlloc( list ) __etListAlloc( &list )
etID_STATE      __etListAlloc( etList** p_list );

#define         etListFree( list ) __etListFree( &list )
etID_STATE      __etListFree( etList** p_list );

#define         etListAppend( list, data ) __etListAppend( &list, data )
etID_STATE      __etListAppend( etList** p_list, void *data );

#define         etListIterate( list, iterator ) __etListIterate( list, &iterator )
etID_STATE      __etListIterate( etList* list, void** iterator );

#define         etListIterateNext( iterator, data ) __etListIterateNext( &iterator, (void**)&data )
etID_STATE      __etListIterateNext( void** iterator, void** data );


etID_STATE      etListIterateNextAviable( void* iterator );




etID_STATE      etListDataRemove( etList* list, void* data, etID_BOOL removeAll );





#ifdef __cplusplus
}
#endif

#endif
