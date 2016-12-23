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

#include "evillib_depends.h"
#include "evillib-extra_depends.h"

#ifndef _C_etDBColumn
#define _C_etDBColumn

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "string/etString.h"
#include "string/etStringChar.h"

#include "db/etDBColumn.h"




/** @ingroup etDB
@defgroup gretDBObjectTable etDBObjectTable - Table handling for etDBObject
@short The etDBObject contains Tables, this functions handle this tables




*/

#ifdef __cplusplus
extern "C" {
#endif



etID_STATE          __etDBColumnAlloc( etDBColumn** p_etDBColumn ){
    etDebugCheckNull( p_etDBColumn );

// vars
    etDBColumn*             dbColumn = NULL;

// allocate memory
    etMemoryAlloc( dbColumn, sizeof(etDBColumn) );

// allocate strings
    etStringAllocLen( dbColumn->name, 64 );
    etStringAllocLen( dbColumn->displayName, 64 );

// columns
    dbColumn->type = etDBCOLUMN_TYPE_NOTHING;
    dbColumn->option = 0;
    dbColumn->value = NULL;

// return
    *p_etDBColumn = dbColumn;
    return etID_YES;
}


etID_STATE          __etDBColumnFree( etDBColumn** p_etDBColumn ){
    etDebugCheckNull( p_etDBColumn );

// vars
    etDBColumn*         dbColumn = *p_etDBColumn;

// release string
    etStringFree( dbColumn->name );
    etStringFree( dbColumn->displayName );

    if( dbColumn->value != NULL ){
        etStringFree( dbColumn->value );
    }

// release memory
    __etMemoryRelease( (void**)p_etDBColumn );

    return etID_YES;
}




etID_STATE          etDBColumnSet( etDBColumn* dbColumn, const char *name, etDBColumnType type, unsigned int option ){
// check
    etDebugCheckNull( dbColumn );
    etDebugCheckNull( name );

// set the name
    etStringCharSet( dbColumn->name, name, -1 );

// set the rest
    dbColumn->type = type;
    dbColumn->option = option;

    return etID_YES;
}


etID_STATE          __etDBColumnGet( etDBColumn* dbColumn, const char **p_name, etDBColumnType *p_type, unsigned int *p_option ){
// check
    etDebugCheckNull( dbColumn );

// get the string from the json-string
    if( p_name != NULL ){
        __etStringCharGet( dbColumn->name, p_name );
    }

    if( p_type != NULL ){
        *p_type = dbColumn->type;
    }

    if( p_option != NULL ){
        *p_option = dbColumn->option;
    }

    return etID_YES;
}


etID_STATE          etDBColumnSetDisplayName( etDBColumn* dbColumn, const char* displayName ){
    etDebugCheckNull( dbColumn );
    etDebugCheckNull( displayName );

// set the name
    return etStringCharSet( dbColumn->displayName, displayName, -1 );

}


etID_STATE          __etDBColumnGetDisplayName( etDBColumn* dbColumn, const char** p_displayName ){
// check
    etDebugCheckNull( dbColumn );

// get the string from the json-string
    if( p_displayName != NULL ){
        __etStringCharGet( dbColumn->displayName, p_displayName );
        return etID_YES;
    }

    return etID_NO;
}




etID_STATE          etDBColumnSetValue( etDBColumn* dbColumn, const char* string ){
// check
    etDebugCheckNull( dbColumn );

// alloc if needed
    if( dbColumn->value == NULL ){
        etStringAlloc( dbColumn->value );
    }

// set
    etStringCharSet( dbColumn->value, string, -1 );

// return
    return etID_YES;
}


etID_STATE          __etDBColumnGetValue( etDBColumn* dbColumn, const char** p_string ){
// check
    etDebugCheckNull( dbColumn );
    etDebugCheckNull( p_string );

    if( dbColumn->value == NULL ){
        *p_string = NULL;
        return etID_NO;
    }

    return __etStringCharGet( dbColumn->value, p_string );
}

















#ifdef __cplusplus
}
#endif


#endif
