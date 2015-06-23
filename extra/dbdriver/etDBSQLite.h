/* etDB - evillib json DB representation
	Copyright (C) 2015 by Martin Langlotz alias stackshadow

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



#ifndef _H_etDBSQLite
#define _H_etDBSQLite

#include "jansson.h"
#include "sqlite3.h"

typedef struct etDBSQLite {
// The driver struct ( as static var to allocate the needed memory for etDBDriver )
	etDBDriver		driverClass;

// Our data
	etString 			*query;

	sqlite3 			*dbSqlite;
	sqlite3_stmt		*dbSqliteResult;
	int					sqliteResultIndex;
	int					dbSQLiteState;

} etDBSQLite;

#define etjDB_TYPE_SQLITE_MAX 3
extern const char *etjDB_TYPE_SQLITE[etjDB_TYPE_SQLITE_MAX];

#define etjDB_COMP_SQLITE_MAX 2
extern const char *etjDB_COMP_SQLITE[etjDB_COMP_SQLITE_MAX];









#define				etDBSQLiteAlloc( etjDBDriverActual ) __etDBSQLiteAlloc( &etjDBDriverActual )
etID_STATE			__etDBSQLiteAlloc( etDBDriver **p_etjDBDriverActual );


etID_STATE			etDBSQLiteConnect( etDBDriver *etjDBDriverActual, const char *path );


#endif

