/* etjDBCondition - contidion for columns
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



#ifndef _H_etDBFilter
#define _H_etDBFilter

typedef json_t etDBFilter;

typedef enum        etDBFilterMode {
    etDB_FILTER_NONE = 0,
    etDB_FILTER_EQUAL,
    etDB_FILTER_NOTEQUAL,
    etDB_FILTER_CONTAINS,
    etDB_FILTER_COUNT,
    etDB_FILTER_ISNULL,
    etDB_FILTER_NOTNULL
} etDBFilterMode;











#define             etDBFilterAlloc( dbFilter, etDebugActual ) __etDBFilterAlloc( &dbFilter, etDebugActual )
etID_STATE          __etDBFilterAlloc( etDBFilter **p_dbFilter, etDebug* etDebugActual );

#define             etDBFilterDump( dbFilter, jsonChar, etDebugActual ) __etDBFilterDump( dbFilter, &jsonChar, etDebugActual )
etID_STATE          __etDBFilterDump( etDBFilter *dbFilter, const char** p_jsonChar, etDebug* etDebugActual );


etID_STATE          etDBFilterInject( etDBFilter *dbFilter, const char *jsonChar, etDebug* etDebugActual );


etID_STATE          etDBFilterClean( etDBFilter *dbFilter, etDebug* etDebugActual );


etID_STATE          etDBFilterAppend( 
                        etDBFilter *dbFilter, 
                        etID_TYPE linkMode,
                        int group, 
                        const char *column, 
                        etDBFilterMode compareMode, 
                        const char *value,
                        etDebug* etDebugActual
                    );


etID_STATE          etDBFilterGet( 
                        etDBFilter *dbFilter, 
                        int index,
                        etID_TYPE *p_linkMode,
                        int *p_group, 
                        const char **p_column, 
                        etDBFilterMode *p_compareMode, 
                        const char **p_value,
                        etDebug* etDebugActual 
                    );


int                 etDBFilterCount( etDBFilter *dbFilter, etDebug* etDebugActual );


#endif