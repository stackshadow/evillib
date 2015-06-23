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

etID_STATE				etDBTableStructInit( etDB *etDBActual );


etID_STATE				etDBTableAppend( etDB *etDBActual, const char *tableName );


etID_STATE				etDBTableSeek( etDB *etDBActual, const char *tableName );


etID_STATE				etDBTableGetFirst( etDB *etDBActual );


etID_STATE				etDBTableGetNext( etDB *etDBActual );




etID_STATE				etDBTableNameOriginalSet( etDB *etDBActual, const char *tableName );

#define				etDBTableNameGet( etDBActual, tableName, tableNameNew ) __etDBTableNameGet( etDBActual, &tableName, &tableNameNew )
etID_STATE				__etDBTableNameGet( etDB *etDBActual, const char **p_tableName, const char **p_tableNameNew );


etID_STATE				etDBTableVisibleNameSet( etDB *etDBActual, const char *visibleName );

#define				etDBTableVisibleNameGet( etDBActual, visibleName ) __etDBTableVisibleNameGet( etDBActual, &visibleName )
etID_STATE				__etDBTableVisibleNameGet( etDB *etDBActual, const char **p_visibleName );




etID_STATE				etDBTableActionSet( etDB *etDBActual, etjDBAction etjDBActionNew );


etjDBAction			etDBTableActionGet( etDB *etDBActual );





#endif
