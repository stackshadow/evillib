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

#ifndef _H_etDBDriver
#define _H_etDBDriver

#ifdef __cplusplus
extern "C" {
#endif

#include "string/etString.h"
#include "db/etDBTable.h"
#include "db/etDBColumn.h"

enum etDBDriverType_e {
    etDB_DRIVER_NONE = 0,
    etDB_DRIVER_SQLITE,
    etDB_DRIVER_PGSQL
};
typedef enum etDBDriverType_e etDBDriverType;


typedef struct etDBDriver_s etDBDriver;
struct etDBDriver_s {

    etDBDriverType      type;

// stuff to create querys
    etID_STATE          (*queryColumnTypeAdd)( etString *sqlquery, etDBColumnType columnType );
    etID_STATE          (*queryColumnOptionAdd)( etString *sqlquery, int option );

// optional callback-functions
    // this function is called before the query is executed, if the
    // callback function return etID_NO the query will not be executed
    etID_STATE          (*queryAcknowledge)( etDBDriver *dbDriver, etDBTable* dbTable, etString *sqlquery );

// connection handling
    etID_STATE          (*connect)( etDBDriver* dbDriver );
    etID_STATE          (*isConnected)( etDBDriver* dbDriver );
    etID_STATE          (*disconnect)( etDBDriver* dbDriver );

// some driver supports execution of bare metal SQL-Commands
    etID_STATE          (*queryExecute)( etDBDriver* dbDriver, etDBTable* dbTable, const char* sqlCommand );

// handle table
    etID_STATE          (*tableAdd)( etDBDriver* dbDriver, etDBTable* dbTable );
    etID_STATE          (*tableRemove)( etDBDriver* dbDriver, etDBTable* dbTable );
    etID_STATE          (*tableExists)( etDBDriver* dbDriver, etDBTable* dbTable );
	etID_STATE          (*tableList)( etDBDriver* dbDriver, etDBTable* dbTable );

// handle columns
    etID_STATE          (*columnAdd)( etDBDriver* dbDriver, etDBTable* dbTable, const char* columnName );
    etID_STATE          (*columnRemove)( etDBDriver* dbDriver, etDBTable* dbTable, const char* columnName );
// handle table data
    etID_STATE          (*dataAdd)( etDBDriver* dbDriver, etDBTable* dbTable );
    etID_STATE          (*dataChange)( etDBDriver* dbDriver, etDBTable* dbTable );
    etID_STATE          (*dataRemove)( etDBDriver* dbDriver, etDBTable* dbTable );
    etID_STATE          (*dataGet)( etDBDriver* dbDriver, etDBTable* dbTable, etDBFilter* dbFilter );
    etID_STATE          (*dataGetWithLimit)( etDBDriver* dbDriver, etDBTable* dbTable, int start, int amount, etDBFilter* dbFilter );
    etID_STATE          (*dataNext)( etDBDriver* dbDriver, etDBTable* dbTable );

// cleanup stuff
	etID_STATE			(*destroy)( etDBDriver* dbDriver );

    void                *dbDriverData;
};



#define             etDBDriverAlloc( dbDriver ) __etDBDriverAlloc( &dbDriver )
etID_STATE          __etDBDriverAlloc( etDBDriver** p_dbDriver );

#define             etDBDriverFree( dbDriver ) __etDBDriverFree( &dbDriver )
etID_STATE          __etDBDriverFree( etDBDriver** p_dbDriver );



#define             etDBDriverName( name ) __etDBDriverName( &name )
etID_STATE          __etDBDriverName( const char** p_name );





etID_STATE          etDBDriverConnect( etDBDriver* dbDriver );


etID_STATE          etDBDriverIsConnect( etDBDriver* dbDriver );


etID_STATE          etDBDriverDisConnect( etDBDriver* dbDriver );



etID_STATE 			etDBDriverQueryExecute( etDBDriver* dbDriver, etDBTable* dbTable, const char* sqlCommand );



etID_STATE          etDBDriverTableAdd( etDBDriver* dbDriver, etDBTable* dbTable );


etID_STATE          etDBDriverTableRemove( etDBDriver* dbDriver, etDBTable* dbTable );


etID_STATE          etDBDriverTableExists( etDBDriver* dbDriver, etDBTable* dbTable );


etID_STATE          etDBDriverTableList( etDBDriver* dbDriver, etDBTable* dbTable );




etID_STATE          etDBDriverDataAdd( etDBDriver* dbDriver, etDBTable* dbTable );


etID_STATE          etDBDriverDataChange( etDBDriver* dbDriver, etDBTable* dbTable );


etID_STATE          etDBDriverDataRemove( etDBDriver *dbDriver, etDBTable* dbTable );
/*


etID_STATE          etDBDriverColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverColumnRemove( etDBDriver *dbDriver, etDBObject *dbObject );




etID_STATE          etDBDriverDataAdd( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverDataChange( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverDataRemove( etDBDriver *dbDriver, etDBObject *dbObject );
*/

etID_STATE          etDBDriverDataGet( etDBDriver* dbDriver, etDBTable* dbTable, etDBFilter* dbFilter );


etID_STATE          etDBDriverDataGetWithLimits( etDBDriver* dbDriver, etDBTable* dbTable, int start, int amount, etDBFilter* dbFilter );


etID_STATE          etDBDriverDataNext( etDBDriver* dbDriver, etDBTable* dbTable );







#ifdef __cplusplus
}
#endif

#endif
