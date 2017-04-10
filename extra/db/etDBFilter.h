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


#ifndef etDBFilter_H
#define etDBFilter_H



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

typedef struct etDBFilterElement_s etDBFilterElement;
struct etDBFilterElement_s {
    int                 group;
    etDBFILTER_OP       operation;
    etString*           column;
    etDBFILTER_TYPE     type;
    etString*           value;

};

typedef struct etDBFilter_s etDBFilter;
struct etDBFilter_s {
    json_t*     jsonArray;
    int         jsonArrayIndex;
    int         jsonArrayLen;
    json_t*     jsonArrayElement;
};


#define             etDBFilterAlloc( dbFilter ) __etDBFilterAlloc( &dbFilter )
etID_STATE          __etDBFilterAlloc( etDBFilter** p_dbFilter );

#define             etDBFilterFree( dbFilter ) __etDBFilterFree( &dbFilter )
etID_STATE          __etDBFilterFree( etDBFilter** p_dbFilter );


etID_STATE          etDBFilterAppend( etDBFilter* dbFilter, int group, etDBFILTER_OP operation, const char* columnName, etDBFILTER_TYPE type, const char* value );


etID_STATE          etDBFilterIterateReset( etDBFilter* dbFilter );


etID_STATE          etDBFilterIterate( etDBFilter* dbFilter );

#define             etDBFilterElementGet( dbFilter, group, operation, columnName, type, value ) __etDBFilterElementGet( dbFilter, &group, &operation, &columnName, &type, &value )
etID_STATE          __etDBFilterElementGet( etDBFilter* dbFilter, int* group, etDBFILTER_OP* operation, const char** p_columnName, etDBFILTER_TYPE* type, const char** p_value );


#ifdef __cplusplus
}
#endif

#endif
