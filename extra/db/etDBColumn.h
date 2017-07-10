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

#include "db/etDBTable.h"

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

#define etDBCOLUMN_OPTION_IGNORE 0
#define etDBCOLUMN_OPTION_NOTHING 0
#define etDBCOLUMN_OPTION_PRIMARY 1
#define etDBCOLUMN_OPTION_NOTNULL 2
#define etDBCOLUMN_OPTION_UNIQUE 4
#define etDBCOLUMN_OPTION_DISPLAY 8




etID_STATE          etDBColumnAppend( etDBTable* dbTable, const char *name, etDBColumnType type, unsigned int option );


etID_STATE          etDBColumnRemove( etDBTable* dbTable, const char* name );


etID_STATE          etDBColumnRemoveAll( etDBTable* dbTable );



#define             etDBColumnGet( dbTable, name, type, option ) __etDBColumnGet( dbTable, &name, &type, &option )
etID_STATE          __etDBColumnGet( etDBTable* dbTable, const char **p_name, etDBColumnType *p_type, unsigned int *p_option );


etID_STATE          etDBColumnSetDisplayName( etDBTable* dbTable, const char* displayName );

#define             etDBColumnGetDisplayName( dbTable, displayName ) __etDBColumnGetDisplayName( dbTable, &displayName )
etID_STATE          __etDBColumnGetDisplayName( etDBTable* dbTable, const char** p_displayName );




etID_STATE          etDBColumnSetValue( etDBTable* dbTable, const char* string );

#define             etDBColumnGetValue( dbTable, string ) __etDBColumnGetValue( dbTable, &string )
etID_STATE          __etDBColumnGetValue( etDBTable* dbTable, const char** p_string );


etID_STATE          etDBColumnCleanValue( etDBTable* dbTable );


etID_STATE          etDBColumnCleanAllValues( etDBTable* dbTable );




etID_STATE          etDBColumnIterateReset( etDBTable* dbTable );


etID_STATE          etDBColumnIterate( etDBTable* dbTable );


etID_STATE          etDBColumnSelect( etDBTable* dbTable, const char* name );


etID_STATE          etDBColumnSelectWithOption( etDBTable* dbTable, unsigned int option );










#ifdef __cplusplus
}
#endif

#endif
