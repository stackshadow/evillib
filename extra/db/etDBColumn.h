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



typedef enum		etDBColumnType {
	etDB_COL_TYPE_DEFAULT = 0,
	etDB_COL_TYPE_TEXT,
	etDB_COL_TYPE_INTEGER,
	etDB_COL_TYPE_COUNT
} etDBColumnType;


etID_STATE				etDBColumnInit( etDB *etDBActual );


etID_STATE				etDBColumnAppend( etDB *etDBActual, const char *columnName );


etID_STATE				etDBColumnSeek( etDB *etDBActual, const char *columnName );


etID_STATE				etDBColumnGetFirst( etDB *etDBActual );


etID_STATE				etDBColumnGetNext( etDB *etDBActual );




etID_STATE				etDBColumnNameOriginalSet( etDB *etDBActual, const char *columnName );

#define				etDBColumnNameGet( etDBActual, columnName, columnNameNew ) __etDBColumnNameGet( etDBActual, &columnName, &columnNameNew )
etID_STATE				__etDBColumnNameGet( etDB *etDBActual, const char **p_columnName, const char **p_columnNameNew );


etID_STATE				etDBColumnTypeSet( etDB *etDBActual, etDBColumnType columnType );


etID_STATE				etDBColumnTypeOriginalSet( etDB *etDBActual, etDBColumnType columnType );

#define				etDBColumnTypeGet( etDBActual, p_columnType ) __etDBColumnTypeGet( etDBActual, &p_columnType )
etID_STATE				__etDBColumnTypeGet( etDB *etDBActual, etDBColumnType *p_columnType );




etID_STATE				etDBColumnIdentifierSet( etDB *etDBActual );


etID_STATE				etDBColumnIsIdentifier( etDB *etDBActual );







#endif
