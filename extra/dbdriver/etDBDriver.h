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

typedef struct etDBDriver_s etDBDriver;
typedef struct etDBDriver_s {
// stuff to create querys
    etID_STATE      (*queryColumnTypeAdd)( etString *sqlquery, etDBColumnType columnType );
    etID_STATE      (*queryColumnOptionAdd)( etString *sqlquery, int option );

// handle table
    etID_STATE      (*tableAdd)( etDBDriver *dbDriver, etDBObject *dbObject );
    etID_STATE      (*tableRemove)( etDBDriver *dbDriver, etDBObject *dbObject );
// handle table data
    etID_STATE      (*dataAdd)( etDBDriver *dbDriver, etDBObject *dbObject );
    etID_STATE      (*dataChange)( etDBDriver *dbDriver, etDBObject *dbObject );
    etID_STATE      (*dataGet)( etDBDriver *dbDriver, etDBObject *dbObject );
    etID_STATE      (*dataNext)( etDBDriver *dbDriver, etDBObject *dbObject );


    void            *dbDriverData;
} etDBDriver;


etID_STATE          etDBDriverTableAdd( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverTableRemove( etDBDriver *dbDriver, etDBObject *dbObject );




etID_STATE          etDBDriverDataAdd( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverDataChange( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverDataGet( etDBDriver *dbDriver, etDBObject *dbObject );


etID_STATE          etDBDriverDataNext( etDBDriver *dbDriver, etDBObject *dbObject );







#ifdef __cplusplus
}
#endif

#endif