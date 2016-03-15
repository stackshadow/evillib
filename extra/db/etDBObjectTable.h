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


#ifndef _H_etDBObjectTable
#define _H_etDBObjectTable


#define etDBObject_TYPE_TABLE "table"


#ifdef __cplusplus
extern "C" {
#endif








etID_STATE      etDBObjectTableAdd( etDBObject *dbObject, const char *tableName );





etID_STATE      etDBObjectTableNext( etDBObject *dbObject );


etID_STATE      etDBObjectTablePick( etDBObject *dbObject, const char *tableName );




etID_STATE      etDBObjectTableNameSet( etDBObject *dbObject, const char *tableName );

#define         etDBObjectTableNameGet( dbObject, tableName ) __etDBObjectTableNameGet( dbObject, &tableName )
etID_STATE      __etDBObjectTableNameGet( etDBObject *dbObject, const char **p_tableName );








#ifdef __cplusplus
}
#endif

#endif
