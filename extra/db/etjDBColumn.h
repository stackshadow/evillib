/* etjDBTable - evillib json Table functions
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



#ifndef _H_etjDBColumn
#define _H_etjDBColumn


etID_STATE				etjDBColumnAppend( etjDB *etjDBActual, const char *columnName );


etID_STATE				etjDBColumnSeek( etjDB *etjDBActual, const char *columnName );


etID_STATE				etjDBColumnGetFirst( etjDB *etjDBActual );


etID_STATE				etjDBColumnGetNext( etjDB *etjDBActual );


etID_STATE				etjDBColumnInDBSet( etjDB *etjDBActual );

#define				etjDBColumnNameGet( etjDBActual, columnName ) __etjDBColumnNameGet( etjDBActual, &columnName )
etID_STATE				__etjDBColumnNameGet( etjDB *etjDBActual, const char **p_columnName );


etID_STATE				etjDBColumnTypeSet( etjDB *etjDBActual, etjDBColumnType columnType );

#define				etjDBColumnTypeGet( etjDBActual, p_columnType ) __etjDBColumnTypeGet( etjDBActual, &p_columnType )
etID_STATE				__etjDBColumnTypeGet( etjDB *etjDBActual, etjDBColumnType *p_columnType );


etID_STATE				etjDBColumnTypeInDBSet( etjDB *etjDBActual );




#endif
