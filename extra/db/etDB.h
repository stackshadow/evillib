/* etDB - evillib json DB representation
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

#ifndef _H_etjDB
#define _H_etjDB



/** @ingroup etDB
Actions of etDB
*/
typedef enum        etDB_ACTION {
    etDB_ACTION_NONE = 0,


    etDB_ACTION_TABLELIST_GET, /*!< This will query all tables inside an connected etDB. You can query the containing tables with etDBNextResult. \n
The tables are saved inside the column with the name "table" 
##### Needed parameter for etDBRun:
 - no additional parameter needed\n
*/
    etDB_ACTION_TABLE_ADD, /*!< @todo implement etDB_ACTION_TABLE_CHANGE */
    etDB_ACTION_TABLE_CHANGE,
/** Remove an etDBTable from an connected etDB */
    etDB_ACTION_TABLE_REMOVE,
    etDB_ACTION_TABLE_GET,
    etDB_ACTION_COLUMN_ADD,
    etDB_ACTION_COLUMN_CHANGE,
    etDB_ACTION_COLUMN_REMOVE,
    etDB_ACTION_DATA_ADD,
    etDB_ACTION_DATA_CHANGE,
    etDB_ACTION_DATA_REMOVE,
    etDB_ACTION_DATA_GET,
    etDB_ACTION_DONE,
    etDB_ACTION_COUNT
} etDB_ACTION;

extern const char*  etDB_ACTION_NAMES[etDB_ACTION_COUNT];

typedef struct			etDB etDB;
typedef struct 		etDB {

    etDB_ACTION     action;


// functions
    etID_STATE      (*preRun)( etDB *etDBActual, etDebug* etDebugActual );

    struct {
    // free
        etID_STATE      (*free)( etDB *etDBActual, etDebug* etDebugActual );
        
    // function that provide infos about the driver
        etID_STATE      (*isConnected)( etDB *etDBActual, etDebug* etDebugActual );

    // Special stuff
        etID_STATE      (*tableListGet)( etDB *etDBActual, etDebug* etDebugActual );
        
    // table
        etID_STATE      (*tableAdd)( etDB *etDBActual, etDBTable *dbTable, etDebug* etDebugActual );
        etID_STATE      (*tableChange)( etDB *etDBActual, etDBTable *dbTable, etDebug* etDebugActual );
        etID_STATE      (*tableRemove)( etDB *etDBActual, etDBTable *dbTable, etDebug* etDebugActual );
        etID_STATE      (*tableGet)( etDB *etDBActual, etDBTable *dbTable, etDebug* etDebugActual );

    // data
        etID_STATE      (*dataAdd)( etDB *etDBActual, etDBTable *dbTable, etDBValue *dbValues, etDebug* etDebugActual );
        etID_STATE      (*dataChange)( etDB *etDBActual, etDBTable *dbTable, etDBValue *dbValues, etDebug* etDebugActual );
        etID_STATE      (*dataRemove)( etDB *etDBActual, etDBTable *dbTable, etDBValue *dbValues, etDebug* etDebugActual );
        etID_STATE      (*dataGet)( etDB *etDBActual, etDBTable *dbTable, etDBFilter *dbFilter, etDebug* etDebugActual );

        etID_STATE      (*nextResult)( etDB *etDBActual, etDBValue *nodeValues, etDebug* etDebugActual );
    } functions;

// data
    void*           data;
} etDB;



#define                 etDBAlloc( etDBActual, etDebugActual ) __etDBAlloc( &etDBActual, etDebugActual )
etID_STATE              __etDBAlloc( etDB** p_etDBActual, etDebug* etDebugActual );

#define                 etDBFree( etDBActual, etDebugActual ) __etDBFree( &etDBActual, etDebugActual )
etID_STATE              __etDBFree( etDB** p_etDBActual, etDebug* etDebugActual );


etID_STATE              __etDBStringGet( json_t *jsonObject, 
                            const char *keyNormal, 
                            const char *keyNew, 
                            const char **p_value,
                            const char **p_valueNew, 
                            etDebug* etDebugActual );


etID_STATE              __etDBIntegerGet( json_t *jsonObject, 
                            const char *keyNormal, 
                            const char *keyNew, 
                            int *p_value,
                            int *p_valueNew, 
                            etDebug* etDebugActual );

etID_STATE              etDBInDBSet( json_t *jsonObject, const char *keyNormal, const char *keyNew, etDebug* etDebugActual );


etID_STATE              etDBActionSet( etDB *etDBActual, etDB_ACTION etDBActionNew, etDebug* etDebugActual );




etDB_ACTION             etDBActionGet( etDB *etDBActual, etDebug* etDebugActual );


etID_STATE              etDBIsConnected( etDB *etDBActual, etDebug* etDebugActual );


etID_STATE              etDBRun( etDB *etDBActual, etDBTable *dbTable, etDBFilter *dbFilter, etDBValue *dbValues, etDebug* etDebugActual );


etID_STATE              etDBNextResult( etDB *etDBActual, etDBValue *nodeValues, etDebug* etDebugActual );












#endif