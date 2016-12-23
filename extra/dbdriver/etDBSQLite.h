/*  Copyright (C) 2014-2016 by Martin Langlotz alias stackshadow

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

#ifdef __cplusplus
extern "C" {
#endif

#include "sqlite3.h"

typedef struct etDBSQLiteDriver_s {
    etString*       sqlquery;

    int             sqliteState;
    sqlite3*        sqliteHandle;
    sqlite3_stmt*   sqliteStatement;

} etDBSQLiteDriver;



etID_STATE          etDBSQLiteDriverInit( etDBDriver* dbDriver, const char* filename );














#ifdef __cplusplus
}
#endif

#endif
