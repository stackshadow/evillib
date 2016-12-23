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



int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );

    etList*         listStart;
    etList*         listEnd;

    const char*     dataFirst = "First Data";
    const char*     dataSecond = "Second Data";
    const char*     dataThird = "Third Data";
    const char*     dataFourth = "Fourth Data";

    etListAlloc( listStart );
    etListAppend( listStart, dataFirst );
    etListAppend( listStart, dataSecond );
    etListAppend( listStart, dataThird );
    etListAppend( listStart, dataFourth );
    etListDump( listStart );

    void* listIterator = NULL;
    etListIterate( listStart, listIterator );
    while( etListIterateNext( listIterator, dataFirst ) == etID_YES ){
        dataSecond = dataFirst;
    }

    etListDataRemove( listStart, dataSecond, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, dataFirst, etID_TRUE );
    etListDump( listStart );

    etListDataRemove( listStart, dataFourth, etID_TRUE );
    etListDump( listStart );




/*

// allocate
    etListAlloc( listStart );
    listEnd = listStart;

    int testValueInt = 1024;
    char testValueString[] = "test\0";
    char* tempString = testValueString;
    void*   iterator = NULL;
    void*   iteratorData = NULL;

// test a single element
    etListAppend( listEnd, &testValueString );
    etListIterate( listEnd, iterator );
    etListIterateNext( iterator, iteratorData );





// append
    etListAppend( listEnd, &testValueInt );
    etListAppend( listEnd, &testValueInt );
    etListAppend( listEnd, &testValueString );
    etListAppend( listEnd, &testValueInt );
    etListAppend( listEnd, &testValueInt );
    etListAppend( listEnd, &testValueInt );

    etListIterate( listEnd, iterator );
    etListIterateNext( iterator, iteratorData );
    testValueInt = *(int*)iteratorData;
    etListIterateNext( iterator, iteratorData );
    etListIterateNext( iterator, iteratorData );
    tempString = (char*)iteratorData;
    etListIterateNext( iterator, iteratorData );

    etListDump( listStart );
// test the remove

// remove in the middle
    etListElementRemove( listStart->next );
    etListDump( listStart );

// remove the start
    etListElementRemove( listStart );
    etListDump( listStart );

// remove the end
    etListElementRemove( listEnd );
    etListDump( listStart );

// remove the string from the list
    etListDataRemove( listStart, &testValueString, etID_FALSE );
    etListDump( listStart );

    etListFree( listStart );
*/
}

