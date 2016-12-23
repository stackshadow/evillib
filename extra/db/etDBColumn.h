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


#ifndef _H_etDBColumn
#define _H_etDBColumn




#ifdef __cplusplus
extern "C" {
#endif


#define etDBObject_TYPE_COLUMN "column"

typedef enum etDBColumnType_t {
// nothing
    etDBCOLUMN_TYPE_NOTHING = 0,

// basic types
    etDBCOLUMN_TYPE_INT = 1,
    etDBCOLUMN_TYPE_FLOAT = 2,
    etDBCOLUMN_TYPE_STRING = 3,

// special
    etDBCOLUMN_TYPE_BLOB = 20


} etDBColumnType;

#define etDBCOLUMN_OPTION_NOTHING 0
#define etDBCOLUMN_OPTION_PRIMARY 1
#define etDBCOLUMN_OPTION_NOTNULL 2
#define etDBCOLUMN_OPTION_UNIQUE 4
#define etDBCOLUMN_OPTION_DISPLAY 8


typedef struct etDBColumn_t {
    etString*           name;
    etString*           displayName;

    etDBColumnType      type;
    unsigned int        option;

    etString*           value;
} etDBColumn;

#define             etDBColumnAlloc( dbColumn ) __etDBColumnAlloc( &dbColumn )
etID_STATE          __etDBColumnAlloc( etDBColumn** p_etDBColumn );

#define             etDBColumnFree( dbColumn ) __etDBColumnFree( &dbColumn )
etID_STATE          __etDBColumnFree( etDBColumn** p_etDBColumn );




etID_STATE          etDBColumnSet( etDBColumn* dbColumn, const char *name, etDBColumnType type, unsigned int option );

#define             etDBColumnGet( dbColumn, name, type, option ) __etDBColumnGet( dbColumn, &name, &type, &option )
etID_STATE          __etDBColumnGet( etDBColumn* dbColumn, const char **p_name, etDBColumnType *p_type, unsigned int *p_option );


etID_STATE          etDBColumnSetDisplayName( etDBColumn* dbColumn, const char* displayName );

#define             etDBColumnGetDisplayName( dbColumn, displayName ) __etDBColumnGetDisplayName( dbColumn, &displayName )
etID_STATE          __etDBColumnGetDisplayName( etDBColumn* dbColumn, const char** p_displayName );




etID_STATE          etDBColumnSetValue( etDBColumn* dbColumn, const char* string );

#define             etDBColumnGetValue( dbColumn, string ) __etDBColumnGetValue( dbColumn, &string )
etID_STATE          __etDBColumnGetValue( etDBColumn* dbColumn, const char** p_string );





















#ifdef __cplusplus
}
#endif

#endif
