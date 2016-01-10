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

#ifndef _H_etDBObject
#define _H_etDBObject



typedef struct etDBObject { 
    json_t*     jsonTables;     // this is an json_array inside jsonTable
    json_t*     jsonTable;


    json_t*     jsonColumns;
    json_t*     jsonColumn;
    void*       jsonColumnIterator;
} etDBObject;




#define             etDBObjectAlloc( dbObject ) __etDBObjectAlloc( &dbObject )
etID_STATE          __etDBObjectAlloc( etDBObject **p_dbObject );

#define             etDBObjectFree( dbObject ) __etDBObjectFree( &dbObject )
etID_STATE          __etDBObjectFree( etDBObject **p_dbObject );



#endif