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



etID_STATE          etDBColumnAppend( etDBTable* dbTable, const char *name, etDBColumnType type, unsigned int option ){
    etDebugCheckNull( dbTable );

// create column
    dbTable->actualColumn = json_object();
    json_object_set_new( dbTable->actualColumn, "name", json_string(name) );
    json_object_set_new( dbTable->actualColumn, "type", json_integer(type) );
    json_object_set_new( dbTable->actualColumn, "opts", json_integer(option) );

    json_object_set_new( dbTable->jsonObjectColumns, name, dbTable->actualColumn );

// return
    return etID_YES;
}


etID_STATE          etDBColumnRemove( etDBTable* dbTable, const char* name ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( name );

    json_object_del( dbTable->jsonObjectColumns, name );

    return etID_YES;
}




etID_STATE          __etDBColumnGet( etDBTable* dbTable, const char **p_name, etDBColumnType *p_type, unsigned int *p_option ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );

// temp vars
    json_t*     tempJsonValue;

// get the string from the json-string
    if( p_name != NULL ){
        *p_name = NULL;
        tempJsonValue = json_object_get( dbTable->actualColumn, "name" );
        if( tempJsonValue != NULL ){
            *p_name = json_string_value(tempJsonValue);
        }
    }

    if( p_type != NULL ){
        *p_type = 0;
        tempJsonValue = json_object_get( dbTable->actualColumn, "type" );
        if( tempJsonValue != NULL ){
            *p_type = json_integer_value(tempJsonValue);
        }

    }

    if( p_option != NULL ){
        *p_option = 0;
        tempJsonValue = json_object_get( dbTable->actualColumn, "opts" );
        if( tempJsonValue != NULL ){
            *p_option = json_integer_value(tempJsonValue);
        }

    }

    return etID_YES;
}


etID_STATE          etDBColumnSetDisplayName( etDBTable* dbTable, const char* displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( displayName );
    etDebugCheckNull( dbTable->actualColumn );

// set the name
    json_object_set_new( dbTable->actualColumn, "displayName", json_string(displayName) );

    return etID_YES;
}


etID_STATE          __etDBColumnGetDisplayName( etDBTable* dbTable, const char** p_displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );

// get name
    if( p_displayName != NULL ){
        json_t* jsonDisplayName = json_object_get( dbTable->jsonObjectTable, "displayName" );
        if( jsonDisplayName == NULL ) return etID_STATE_ERR;
        *p_displayName = json_string_value( jsonDisplayName );
        return etID_YES;
    }

    return etID_NO;
}




etID_STATE          etDBColumnSetValue( etDBTable* dbTable, const char* string ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );

// set the name
    json_object_set_new( dbTable->actualColumn, "value", json_string(string) );

// return
    return etID_YES;
}


etID_STATE          __etDBColumnGetValue( etDBTable* dbTable, const char** p_string ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( p_string );
    etDebugCheckNull( dbTable->actualColumn );

// get name
    if( p_string != NULL ){
        json_t* jsonValue = json_object_get( dbTable->jsonObjectTable, "value" );
        if( jsonValue == NULL ) return etID_STATE_ERR;
        *p_string = json_string_value( jsonValue );
        return etID_YES;
    }

    return etID_NO;
}


etID_STATE          etDBColumnCleanValue( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );

// set the name
    json_object_del( dbTable->actualColumn, "value" );

// return
    return etID_YES;
}


etID_STATE          etDBColumnCleanAllValues( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );

    etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate(dbTable) == etID_YES ){
        etDBColumnCleanValue( dbTable );
    }

    return etID_YES;
}




etID_STATE          etDBColumnIterateReset( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );

    dbTable->iteratorColumn = NULL;

    return etID_YES;
}


etID_STATE          etDBColumnIterate( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );

// start or continue
    if( dbTable->iteratorColumn == NULL ){
        dbTable->iteratorColumn = json_object_iter( dbTable->jsonObjectColumns );
    } else {
        dbTable->iteratorColumn = json_object_iter_next( dbTable->jsonObjectColumns, dbTable->iteratorColumn );
    }

// we reached the end of the list
    if( dbTable->iteratorColumn == NULL ) return etID_STATE_END;

    dbTable->actualColumn = json_object_iter_value( dbTable->iteratorColumn );

    return etID_YES;
}


etID_STATE          etDBColumnSelect( etDBTable* dbTable, const char* name ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );
    etDebugCheckNull( name );

    dbTable->actualColumn = json_object_get( dbTable->jsonObjectColumns, name );
    if( dbTable->actualColumn == NULL ) return etID_STATE_NOTHING;

    return etID_YES;
}


etID_STATE          etDBColumnSelectWithOption( etDBTable* dbTable, unsigned int option ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->actualColumn );


// vars
    unsigned int columnOption = 0;

    etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate(dbTable) == etID_YES ){
        __etDBColumnGet(dbTable,NULL,NULL,&columnOption);
        if( option & columnOption ){
            return etID_YES;
        }
    }

    return etID_STATE_NODATA;
}







#ifdef __cplusplus
}
#endif


#endif
