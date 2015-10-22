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



#ifndef _H_etDBColumn
#define _H_etDBColumn


/**
The etDBColumnType defines the type of an column. 
This is needed by the driver to get the real type of the underlay DB.
For example the sqlite-driver mapp the etDB_COL_TYPE_DEFAULT to "TEXT"
For developers: If you change these, it effects ALL drivers ! 
Be very, very, very careful with changes on this enum.
*/
typedef enum		etDBColumnType_e {
    etDB_COL_TYPE_NONE = -1,
    etDB_COL_TYPE_DEFAULT = 0,
    etDB_COL_TYPE_TEXT,
    etDB_COL_TYPE_INTEGER,
    etDB_COL_TYPE_COUNT
} etDBColumnType;




etID_STATE              etDBColumnAppend( etDBTable *dbTable, const char *columnName, etDebug* etDebugActual );


etID_STATE              etDBColumnSeek( etDBTable *dbTable, const char *columnName, etDebug* etDebugActual );


etID_STATE              etDBColumnIndexReset( etDBTable *dbTable, etDebug* etDebugActual );

#define                 etDBColumnGetNext( dbTable, columnName, etDebugActual ) __etDBColumnGetNextFull( dbTable, &columnName, &columnName, etDebugActual )
#define                 etDBColumnGetNextOriginal( dbTable, columnName, etDebugActual ) __etDBColumnGetNextFull( dbTable, &columnName, NULL, etDebugActual )
#define                 etDBColumnGetNextFull( dbTable, columnName, columnNameNew, etDebugActual ) __etDBColumnGetNextFull( dbTable, &columnName, &columnNameNew, etDebugActual )
etID_STATE              __etDBColumnGetNextFull( etDBTable *dbTable, const char **p_columnName, const char **p_columnNameNew, etDebug* etDebugActual );


#define                 etDBColumnNameSet( dbTable, columnName, etDebugActual ) etDBColumnNameSetFull( dbTable, NULL, columnName, etDebugActual )
#define                 etDBColumnNameOriginalSet( dbTable, columnName, etDebugActual ) etDBColumnNameSetFull( dbTable, columnName, NULL, etDebugActual )
etID_STATE              etDBColumnNameSetFull( etDBTable *dbTable, const char *columnName, const char *columnNameNew, etDebug* etDebugActual );

#define                 etDBColumnNameGet( dbTable, columnName, etDebugActual ) __etDBColumnNameGetFull( dbTable, &columnName, &columnName, etDebugActual )
#define                 etDBColumnNameOriginalGet( dbTable, columnName, etDebugActual ) __etDBColumnNameGetFull( dbTable, &columnName, NULL, etDebugActual )
#define                 etDBColumnNameGetFull( dbTable, columnName, columnNameNew, etDebugActual ) __etDBColumnNameGetFull( dbTable, &columnName, &columnNameNew, etDebugActual )
etID_STATE              __etDBColumnNameGetFull( etDBTable *dbTable, const char **p_columnName, const char **p_columnNameNew, etDebug* etDebugActual );


#define                 etDBColumnTypeSet( dbTable, columnType, etDebugActual ) etDBColumnTypeSetFull( dbTable, etDB_COL_TYPE_NONE, columnType, etDebugActual )
#define                 etDBColumnTypeOriginalSet( dbTable, columnType, etDebugActual ) etDBColumnTypeSetFull( dbTable, columnType, etDB_COL_TYPE_NONE, etDebugActual )
etID_STATE              etDBColumnTypeSetFull( etDBTable *dbTable, etDBColumnType columnType, etDBColumnType columnTypeNew, etDebug* etDebugActual );

#define                 etDBColumnTypeGet( etDBActual, columnType, etDebugActual ) __etDBColumnTypeGetFull( etDBActual, &columnType, &columnType, etDebugActual )
#define                 etDBColumnTypeOriginalGet( etDBActual, columnType, etDebugActual ) __etDBColumnTypeGetFull( etDBActual, &p_columnType, NULL, etDebugActual )
etID_STATE              __etDBColumnTypeGetFull( etDBTable *dbTable, etDBColumnType *p_columnType, etDBColumnType *p_columnTypeNew, etDebug* etDebugActual );




etID_STATE              etDBColumnIdentifierSet( etDBTable *dbTable, etDebug* etDebugActual );

#define                 etDBColumnIdentifierGet( dbTable, columnName, etDebugActual ) __etDBColumnIdentifierGet( dbTable, &columnName, etDebugActual )
etID_STATE              __etDBColumnIdentifierGet( etDBTable *dbTable, const char **p_identifierColumn, etDebug* etDebugActual );








#endif
