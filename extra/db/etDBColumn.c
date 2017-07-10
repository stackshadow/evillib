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

typedef struct etDBColumn_s {

	etString*			name;
	etString*			displayName;
	etDBColumnType		type;
	unsigned int		options;

	etString*			value;

} etDBColumn;


etID_STATE          __etDBColumnFree( etDBColumn** p_dbColumn ){

// vars
	etDBColumn*	dbColumn = *p_dbColumn;

// remove the old element
	etStringFree( dbColumn->name );
	etStringFree( dbColumn->displayName );
	etStringFree( dbColumn->value );
	etMemoryRelease( dbColumn );

	*p_dbColumn = NULL;

	return etID_YES;
}


etID_STATE          etDBColumnAppend( etDBTable* dbTable, const char* name, etDBColumnType type, unsigned int option ){
    etDebugCheckNull( dbTable );

// allocate memory
	etDBColumn*	dbColumn = NULL;
	etMemoryAlloc( dbColumn, sizeof(etDBColumn) );

// alloc
	etStringAlloc( dbColumn->name );
	etStringAlloc( dbColumn->displayName );

// name
	etStringCharSet( dbColumn->name, name, -1 );

// type / options
	dbColumn->type = type;
	dbColumn->options = option;

// append it to the list
	if( etListAppend( dbTable->columns, dbColumn ) == etID_YES ){
		return etID_YES;
	}

// WE could not add it to the list, free the column
	__etDBColumnFree( &dbColumn );

// return
    return etID_NO;
}


etID_STATE			__etDBColumnFind( etDBTable* dbTable, const char* name, unsigned int option, etDBColumn** p_dbColumn ){

// vars
	void*				iterator;
	etDBColumn*			dbColumn;
	const char*			dbColumnNameCharArray;

	etListIterate( dbTable->columns, iterator );
	while( etListIterateNext( iterator, dbColumn ) == etID_YES ){

		if( name != NULL ){
			etStringCharGet( dbColumn->name, dbColumnNameCharArray );

			if( etStringCharCompare( dbColumn->name, name ) != 0 ){
				continue;
			}
		}

		if( option != etDBCOLUMN_OPTION_IGNORE ){
			if( ! (dbColumn->options & option) ){
				continue;
			}
		}

		*p_dbColumn = dbColumn;
		return etID_YES;
	}

	*p_dbColumn = NULL;
	return etID_NO;
}


etID_STATE          etDBColumnRemove( etDBTable* dbTable, const char* name ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( name );

// vars
	etDBColumn*			dbColumn;

	if( __etDBColumnFind(dbTable,name,etDBCOLUMN_OPTION_IGNORE,&dbColumn) != etID_YES ) return etID_STATE_ERR_INTERR;

// cleanup and remove memory
// etStringFree overwrite the string with 0 if secure-memory is enabled ( by default )
	etStringFree( dbColumn->name );
	dbColumn->type = etDBCOLUMN_TYPE_NOTHING;
	dbColumn->options = etDBCOLUMN_OPTION_NOTHING;

    return etID_YES;
}


etID_STATE          etDBColumnRemoveAll( etDBTable* dbTable ){

// vars
	void*				iterator;
	etDBColumn*			dbColumn;

	etListIterate( dbTable->columns, iterator );
	while( etListIterateNext( iterator, dbColumn ) == etID_YES ){
		__etDBColumnFree( &dbColumn );
	}

	return etID_YES;
}




etID_STATE          __etDBColumnGet( etDBTable* dbTable, const char **p_name, etDBColumnType *p_type, unsigned int *p_option ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;


// get the string from the json-string
    if( p_name != NULL ){
        __etStringCharGet( dbColumn->name, p_name );
    }

    if( p_type != NULL ){
        *p_type = dbColumn->type;
    }

    if( p_option != NULL ){
        *p_option = dbColumn->options;
    }

    return etID_YES;
}


etID_STATE          etDBColumnSetDisplayName( etDBTable* dbTable, const char* displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( displayName );
    etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;

// set the display name
	etStringCharSet( dbColumn->displayName, displayName, -1 );


    return etID_YES;
}


etID_STATE          __etDBColumnGetDisplayName( etDBTable* dbTable, const char** p_displayName ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;

	if( __etStringCharGet( dbColumn->displayName, p_displayName ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}




etID_STATE          etDBColumnSetValue( etDBTable* dbTable, const char* string ){
// check
    etDebugCheckNull( dbTable );
	etDebugCheckNull( string );
	etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;

	if( dbColumn->value == NULL ){
		if( etStringAlloc( dbColumn->value ) != etID_YES ){
			return etID_NO;
		}
	}

	if( etStringCharSet( dbColumn->value, string, -1 ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          __etDBColumnGetValue( etDBTable* dbTable, const char** p_string ){
// check
    etDebugCheckNull( dbTable );
	etDebugCheckNull( p_string );
	etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;
	const char*		dbColumnValueCharArray = NULL;

	if( etStringCharGet( dbColumn->value, dbColumnValueCharArray ) == etID_YES ){
		*p_string = dbColumnValueCharArray;
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          etDBColumnCleanValue( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );
	etDebugCheckNull( dbTable->column );

// temp vars
	etDBColumn*		dbColumn = (etDBColumn*)dbTable->column;

// clean the string
	if( etStringClean( dbColumn->value ) == etID_YES ){
		return etID_YES;
	}

// return
    return etID_NO;
}


etID_STATE          etDBColumnCleanAllValues( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->column );

    etDBColumnIterateReset(dbTable);
    while( etDBColumnIterate(dbTable) == etID_YES ){
        etDBColumnCleanValue( dbTable );
    }

    return etID_YES;
}




etID_STATE          etDBColumnIterateReset( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );

    dbTable->columnIterator = NULL;
	dbTable->column = NULL;

    return etID_YES;
}


etID_STATE          etDBColumnIterate( etDBTable* dbTable ){
// check
    etDebugCheckNull( dbTable );

// start or continue
    if( dbTable->columnIterator == NULL ){
		etListIterate( dbTable->columns, dbTable->columnIterator );
		if( dbTable->column != NULL ){
			dbTable->column = NULL;
			return etID_NO;
		}
    }

// get the data
	if( etListIterateNext( dbTable->columnIterator, dbTable->column ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          etDBColumnSelect( etDBTable* dbTable, const char* name ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->columns );
    etDebugCheckNull( name );

	if( __etDBColumnFind( dbTable, name, etDBCOLUMN_OPTION_IGNORE, (etDBColumn**)&dbTable->column ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}


etID_STATE          etDBColumnSelectWithOption( etDBTable* dbTable, unsigned int option ){
    etDebugCheckNull( dbTable );
    etDebugCheckNull( dbTable->columns );


	if( __etDBColumnFind( dbTable, NULL, option, (etDBColumn**)&dbTable->column ) == etID_YES ){
		return etID_YES;
	}

    return etID_NO;
}







#ifdef __cplusplus
}
#endif


#endif
