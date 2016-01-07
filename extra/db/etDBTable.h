/* etDBTable - evillib json Table functions
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



#ifndef _H_etDBTable
#define _H_etDBTable


typedef struct etDBTable { 
    json_t*     jsonTables;     // this is an json_array inside jsonTable
    json_t*     jsonTable;


    json_t*     jsonColumns;
    json_t*     jsonColumn;
    void*       jsonColumnIterator;
} etDBTable;


#define                 etDBTableAlloc( etDBTable, etDebugActual ) __etDBTableAlloc( &etDBTable, etDebugActual )
etID_STATE              __etDBTableAlloc( etDBTable **p_etDBTable, etDebug* etDebugActual );

#define                 etDBTableFree( etDBTable, etDebugActual ) __etDBTableFree( &etDBTable, etDebugActual )
etID_STATE              __etDBTableFree( etDBTable **p_etDBTable, etDebug* etDebugActual );

#define                 etDBTableDumps( etDBTable, jsonChar, etDebugActual ) __etDBTableDumps( etDBTable, &jsonChar, etDebugActual )
etID_STATE              __etDBTableDumps( etDBTable* dbTable, const char** p_jsonChar, etDebug* etDebugActual );


etID_STATE              etDBTableAppend( etDBTable *dbTable, const char *tableName, etDebug* etDebugActual );


etID_STATE              etDBTableSeek( etDBTable *dbTable, const char *tableName, etDebug* etDebugActual );


etID_STATE              etDBTableDumpf( etDBTable *dbTable, etDebug* etDebugActual );


#define                 etDBTableNameSet( etDBTable, tableName, etDebugActual ) etDBTableNameSetFull( etDBTable, NULL, tableName, etDebugActual )
#define                 etDBTableNameOriginalSet( etDBTable, tableName, etDebugActual ) etDBTableNameSetFull( etDBTable, tableName, NULL, etDebugActual )
etID_STATE              etDBTableNameSetFull( etDBTable *etDBTableActual, const char *tableName, const char *tableNameNew, etDebug* etDebugActual );


#define                 etDBTableNameGet( etDBTable, tableName, etDebugActual ) __etDBTableNameGetFull( etDBTable, &tableName, &tableName, etDebugActual )
#define                 etDBTableNameOriginalGet( etDBTable, tableName, etDebugActual ) __etDBTableNameGetFull( etDBTable, &tableName, NULL, etDebugActual )
#define                 etDBTableNameGetFull( etDBTable, tableName, tableNameNew, etDebugActual ) __etDBTableNameGetFull( etDBTable, &tableName, &tableNameNew, etDebugActual )
etID_STATE              __etDBTableNameGetFull( etDBTable *etDBTableActual, const char **p_tableName, const char **p_tableNameNew, etDebug* etDebugActual );


etID_STATE              etDBTableVisibleNameSet( etDBTable *etDBTableActual, const char *visibleName, etDebug* etDebugActual );

#define                 etDBTableVisibleNameGet( etDBActual, visibleName, etDebugActual ) __etDBTableVisibleNameGet( etDBActual, &visibleName, etDebugActual )
etID_STATE              __etDBTableVisibleNameGet( etDBTable *etDBTableActual, const char **p_visibleName, etDebug* etDebugActual );






#endif
