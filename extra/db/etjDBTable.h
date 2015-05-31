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



#ifndef _H_etjDBTable
#define _H_etjDBTable

etID_STATE				etjDBTableAppend( etjDB *etjDBActual, const char *tableName );





etID_STATE				etjDBTableInDBSet( etjDB *etjDBActual );

#define				etjDBTableNameGet( etjDBActual, tableName ) __etjDBTableNameGet( etjDBActual, &tableName )
etID_STATE				__etjDBTableNameGet( etjDB *etjDBActual, const char **p_tableName );


etID_STATE				etjDBTableVisibleNameSet( etjDB *etjDBActual, const char *visibleName );

#define				etjDBTableVisibleNameGet( etjDBActual, visibleName ) __etjDBTableVisibleNameGet( etjDBActual, &visibleName )
etID_STATE				__etjDBTableVisibleNameGet( etjDB *etjDBActual, const char **p_visibleName );


etID_STATE				etjDBTableActionSet( etjDB *etjDBActual, etjDBAction etjDBActionNew );


etjDBAction			etjDBTableActionGet( etjDB *etjDBActual );


etID_STATE				etjDBTableSeek( etjDB *etjDBActual, const char *tableName );


etID_STATE				etjDBTableGetFirst( etjDB *etjDBActual );


etID_STATE				etjDBTableGetNext( etjDB *etjDBActual );




#endif
