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



#ifndef _H_etDBRelations
#define _H_etDBRelations

typedef struct etDBRelations{
    json_t*     jsonArrayRelations;
    int         foreward;
    int         index;
} etDBRelations;


#define             etDBRelationAlloc( dbRelations, etDebugActual ) __etDBRelationAlloc( &dbRelations, etDebugActual )
etID_STATE          __etDBRelationAlloc( etDBRelations** p_dbRelations, etDebug* etDebugActual );


etID_STATE          etDBRelationClean( etDBRelations* dbRelations, etDebug* etDebugActual );

#define             etDBRelationFree( dbRelations, etDebugActual ) __etDBRelationFree( &dbRelations, etDebugActual )
etID_STATE          __etDBRelationFree( etDBRelations** p_dbRelations, etDebug* etDebugActual );

#define             etDBRelationDumps( etDBActual, jsonChar, etDebugActual ) __etDBRelationDumps( etDBActual, &jsonChar, etDebugActual )
etID_STATE          __etDBRelationDumps( etDBRelations* dbRelations, const char** p_jsonChar, etDebug* etDebugActual );


etID_STATE          etDBRelationInject( etDBRelations* dbRelations, const char *jsonChar, etDebug* etDebugActual );


etID_STATE          etDBRelationAppend( etDBRelations* dbRelations, 
                        const char *tableA, const char *columnA,
                        const char *tableB, const char *columnB, 
                        etDebug* etDebugActual );


int                 etDBRelationCount( etDBRelations* dbRelations, etDebug* etDebugActual );


etID_STATE          etDBRelationsIndexReset( etDBRelations* dbRelations, etDebug* etDebugActual );

#define             etDBRelationFindNext( dbRelations, tableSearch, tableColumn, relatedTable, relatedColumn, etDebugActual ) __etDBRelationFindNext( dbRelations, tableSearch, &tableColumn, &relatedTable, &relatedColumn, etDebugActual )
etID_STATE          __etDBRelationFindNext( etDBRelations* dbRelations, const char *tableSearch, 
                        const char **p_tableColumn, const char **p_relatedTable, const char **p_relatedColumn, 
                        etDebug* etDebugActual );

#define             etDBRelationFindNextColumn( dbRelations, tableSearch, columnSearch, relatedTable, relatedColumn, etDebugActual ) __etDBRelationFindNextColumn( dbRelations, tableSearch, columnSearch, &relatedTable, &relatedColumn, etDebugActual )
etID_STATE          __etDBRelationFindNextColumn( etDBRelations* dbRelations, const char *tableSearch, const char *columnSearch,
                        const char **p_relatedTable, const char **p_relatedColumn, 
                        etDebug* etDebugActual );

#define             etDBRelationFindNextTable( dbRelations, tableSearch, relatedTableSearch, tableColumn, relatedColumn, etDebugActual ) __etDBRelationFindNextTable( dbRelations, tableSearch, relatedTableSearch, &tableColumn, &relatedColumn, etDebugActual )
etID_STATE          __etDBRelationFindNextTable( etDBRelations* dbRelations, const char *tableSearch, const char *relatedTableSearch,
                        const char **p_tableColumn,  const char **p_relatedColumn, 
                        etDebug* etDebugActual );

#endif

