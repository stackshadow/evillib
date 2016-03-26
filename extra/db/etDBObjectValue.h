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


#ifndef _H_etDBObjectValue
#define _H_etDBObjectValue


#ifdef __cplusplus
extern "C" {
#endif




etID_STATE          etDBObjectValueClean( etDBObject *dbObject );


etID_STATE          etDBObjectValueSet( etDBObject *dbObject, const char *columnName, const char *value );

#define             etDBObjectValueGet( dbObject, columnName, value ) __etDBObjectValueGet( dbObject, columnName, &value )
etID_STATE          __etDBObjectValueGet( etDBObject *dbObject, const char *columnName, const char **value );

#define             etDBObjectValueNext( dbObject, columnName, value ) __etDBObjectValueNext( dbObject, &columnName, &value )
etID_STATE          __etDBObjectValueNext( etDBObject *dbObject, const char **p_columnName, const char **p_value );















#ifdef __cplusplus
}
#endif

#endif
