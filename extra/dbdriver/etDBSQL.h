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

#ifndef _H_etDBSQL
#define _H_etDBSQL

#ifdef __cplusplus
extern "C" {
#endif


etID_STATE          etDBSQLTableCreate( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery, const char *columnEnclose );


etID_STATE          etDBSQLTableRemove( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery );


etID_STATE          etDBSQLColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery, const char *columnEnclose );


etID_STATE          etDBSQLDataAdd( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery );


etID_STATE          etDBSQLDataChange( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery );


etID_STATE          etDBSQLDataRemove( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery, const char *columnEnclose );


etID_STATE          etDBSQLSelect( etDBDriver *dbDriver, etDBObject *dbObject, etString *sqlquery );



















#ifdef __cplusplus
}
#endif

#endif
