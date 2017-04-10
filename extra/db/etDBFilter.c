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



#ifndef etDBFilter_C
#define etDBFilter_C

#include "core/etDebug.c"
#include "core/etObject.c"
#include "memory/etMemory.c"
#include "string/etString.c"
#include "string/etStringChar.c"

#include "db/etDBFilter.h"

#ifdef __cplusplus
extern "C" {
#endif


etID_STATE          __etDBFilterAlloc( etDBFilter** p_dbFilter ){
    etDebugCheckNull( p_dbFilter );

// vars
    etDBFilter*             dbFilter = NULL;

// allocate memory
    etMemoryAlloc( dbFilter, sizeof(etDBFilter) );

// filter
    dbFilter->jsonArray = json_array();
    dbFilter->jsonArrayIndex = 0;
    dbFilter->jsonArrayLen = 0;
    dbFilter->jsonArrayElement = NULL;

// return
    *p_dbFilter = dbFilter;
    return etID_YES;
}


etID_STATE          __etDBFilterFree( etDBFilter** p_dbFilter ){
    etDebugCheckNull( p_dbFilter );

// vars
    etDBFilter*             dbFilter = *p_dbFilter;

// clean
    json_decref( dbFilter->jsonArray );

// free the memory for our class
    etMemoryRelease( dbFilter );

// return
    *p_dbFilter = NULL;
    return etID_YES;
}


etID_STATE          etDBFilterAppend( etDBFilter* dbFilter, int group, etDBFILTER_OP operation, const char* columnName, etDBFILTER_TYPE type, const char* value ){
    etDebugCheckNull( dbFilter );

// a new element
    json_t*         jsonFilterElement = json_object();

    json_object_set_new( jsonFilterElement, "group", json_integer(group) );
    json_object_set_new( jsonFilterElement, "operation", json_integer(operation) );
    json_object_set_new( jsonFilterElement, "colName", json_string(columnName) );
    json_object_set_new( jsonFilterElement, "type", json_integer(type) );
    json_object_set_new( jsonFilterElement, "value", json_string(value) );

    json_array_append( dbFilter->jsonArray, jsonFilterElement );

    dbFilter->jsonArrayLen++;

    return etID_YES;
}


etID_STATE          etDBFilterIterateReset( etDBFilter* dbFilter ){
    etDebugCheckNull( dbFilter );

    dbFilter->jsonArrayIndex = 0;

    return etID_YES;
}


etID_STATE          etDBFilterIterate( etDBFilter* dbFilter ){
    etDebugCheckNull( dbFilter );

// finished ?
    if( dbFilter->jsonArrayIndex >= dbFilter->jsonArrayLen ){
        dbFilter->jsonArrayLen = json_array_size(dbFilter->jsonArray);
        dbFilter->jsonArrayIndex = 0;
        return etID_STATE_END;
    }

    dbFilter->jsonArrayElement = json_array_get( dbFilter->jsonArray, dbFilter->jsonArrayIndex );
    dbFilter->jsonArrayIndex++;

    return etID_YES;
}


etID_STATE          __etDBFilterElementGet( etDBFilter* dbFilter, int* group, etDBFILTER_OP* operation, const char** p_columnName, etDBFILTER_TYPE* type, const char** p_value ){
// check
    etDebugCheckNull( dbFilter );
    etDebugCheckNull( dbFilter->jsonArrayElement );

// values
    json_t*     jsonValue = NULL;

    if( group != NULL ){
        *group = 0;
        jsonValue = json_object_get( dbFilter->jsonArrayElement, "group" );
        if( jsonValue != NULL ){
            *group = json_integer_value(jsonValue);
        }
    }

    if( operation != NULL ){
        *operation = 0;
        jsonValue = json_object_get( dbFilter->jsonArrayElement, "operation" );
        if( jsonValue != NULL ){
            *operation = json_integer_value(jsonValue);
        }
    }

    if( p_columnName != NULL ){
        *p_columnName = NULL;
        jsonValue = json_object_get( dbFilter->jsonArrayElement, "colName" );
        if( jsonValue != NULL ){
            *p_columnName = json_string_value(jsonValue);
        }
    }

    if( type != NULL ){
        *type = 0;
        jsonValue = json_object_get( dbFilter->jsonArrayElement, "type" );
        if( jsonValue != NULL ){
            *type = json_integer_value(jsonValue);
        }
    }

    if( p_value != NULL ){
        *p_value = NULL;
        jsonValue = json_object_get( dbFilter->jsonArrayElement, "value" );
        if( jsonValue != NULL ){
            *p_value = json_string_value(jsonValue);
        }
    }


    return etID_YES;
}


#ifdef __cplusplus
}
#endif


#endif
