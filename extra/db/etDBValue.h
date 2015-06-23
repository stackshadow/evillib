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



#ifndef _H_etDBValue
#define _H_etDBValue













etID_STATE				etDBValueInit( etDB *etDBActual );


etID_STATE				etDBValuesNewClean( etDB *etDBActual );


etID_STATE				etDBValueSet( etDB *etDBActual, const char *columnName, const char *value );

#define 				etDBValueGet( etDBActual, columnName, value, valueNew ) __etDBValueGet( etDBActual, columnName, &value, &valueNew );
etID_STATE				__etDBValueGet( etDB *etDBActual, 
							const char *columnName, 
							const char **p_value, 
							const char **p_valueNew
						);




#endif
