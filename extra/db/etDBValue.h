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


typedef struct etDBValue {
    json_t*     values;
    void*       iterator;    
} etDBValue;









#define             etDBValueAlloc( etDBValues ) __etDBValueAlloc( &etDBValues )
etID_STATE          __etDBValueAlloc( etDBValue **p_etDBValues );


etID_STATE          etDBValuesClean( etDBValue *etDBValues );


etID_STATE          etDBValueSet( etDBValue *etDBValues, const char *columnName, const char *value );

#define             etDBValueGet( etDBValues, columnName, value ) __etDBValueGet( etDBValues, columnName, &value )
etID_STATE          __etDBValueGet( etDBValue *etDBValues, const char *columnName, const char **p_value );

#define             etDBValueGetNext( etDBValues, columnName, value ) __etDBValueGetNext( etDBValues, &columnName, &value )
etID_STATE          __etDBValueGetNext( etDBValue *etDBValues, const char **p_columnName, const char **p_value );

#define             etDBValueFree( etDBValues ) __etDBValueFree( &etDBValues )
etID_STATE          __etDBValueFree( etDBValue **p_etDBValues );


#endif
