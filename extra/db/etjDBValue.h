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



#ifndef _H_etjDBValue
#define _H_etjDBValue













etID_STATE				etjDBValueInit( etjDB *etjDBActual );


etID_STATE				etjDBValueSet( etjDB *etjDBActual, const char *columnName, const char *value );

#define 				etjDBValueGet( etjDBActual, columnName, value ) __etjDBValueGet( etjDBActual, columnName, &value );
etID_STATE				__etjDBValueGet( etjDB *etjDBActual, const char *columnName, const char **p_value );




#endif
