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


#ifndef _H_etDBObjectFilter
#define _H_etDBObjectFilter


#include "db/etDBObject.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    etDBFILTER_TYPE_NOTHING = 0,
    etDBFILTER_TYPE_EQUAL,
    etDBFILTER_TYPE_NOTEQUAL,
    etDBFILTER_TYPE_CONTAIN,
    etDBFILTER_TYPE_BEGIN,
    etDBFILTER_TYPE_ENDs
} 
etDBFILTER_TYPE;

typedef enum {
    etDBFILTER_OP_NOTHING = 0,
    etDBFILTER_OP_AND,
    etDBFILTER_OP_OR
} 
etDBFILTER_OP;




etID_STATE      etDBObjectFilterClear( etDBObject *dbObject );


etID_STATE      etDBObjectFilterAdd( etDBObject *dbObject, int filterGroup, etDBFILTER_OP filterOperation, const char *filterColumn, etDBFILTER_TYPE filterType, const char *filterString );


etID_STATE      etDBObjectFilterSet( etDBObject *dbObject, const char *jsonString );


etID_STATE      etDBObjectFilterNext( etDBObject *dbObject, int *filterGroup, etDBFILTER_OP *filterOperation, const char **filterColumn, etDBFILTER_TYPE *filterType, const char **filterString );










#ifdef __cplusplus
}
#endif

#endif
