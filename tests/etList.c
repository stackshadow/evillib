/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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

#include "core/etInit.c"
#include "memory/etList.c"

    static const char*     dataFirst = "First Data";
    static const char*     dataSecond = "Second Data";
    static const char*     dataThird = "Third Data";
    static const char*     dataFourth = "Fourth Data";

int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

    etList*         listStart;
    const char*     tempData = NULL;
    void*           listIterator = NULL;


    etListAlloc( listStart );
    etListDump( listStart );

    etListAppend( listStart, (void*)dataFirst );
    etListAppend( listStart, (void*)dataSecond );
    etListAppend( listStart, (void*)dataThird );
    etListAppend( listStart, (void*)dataFourth );
    etListDump( listStart );


    etListIterate( listStart, listIterator );
    while( etListIterateNext( listIterator, tempData ) == etID_YES ){
        tempData = tempData;
    }

    etListDataRemove( listStart, (void*)dataSecond, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, (void*)dataFirst, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, (void*)dataFourth, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, (void*)dataThird, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, (void*)dataFirst, etID_TRUE );
    etListDump( listStart );


// append again
    etListAppend( listStart, (void*)dataFirst );
    etListAppend( listStart, (void*)dataSecond );
    etListAppend( listStart, (void*)dataThird );
    etListAppend( listStart, (void*)dataFourth );
    etListDump( listStart );

    etListIterate( listStart, listIterator );
    etListIterateNext( listIterator, tempData );
    etListIterateNext( listIterator, tempData );
    etListIterateRemove( listStart, listIterator );
    etListDump( listStart );



}

