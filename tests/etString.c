/* Apicheck Function for etString
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


#include "evillib_defines.h"
#include "evillib_depends.h"


#include "core/etDebug.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etString.h"

#include "app/etApicheck.h"
#include "app/etApicheck.c"


etID_STATE            etStringTest(){
    etDebugMessage( etID_LEVEL_DETAIL, "Start test.." );
    

// Vars
    etString    *etStringActual = NULL;

// Allocate a new etString
    etStringAlloc( etStringActual );
    etStringClean( etStringActual );
    etStringDestroy( etStringActual );
    etStringFree( etStringActual );



    etStringAllocLen( etStringActual, 50 );
    etStringClean( etStringActual );
    etStringDestroy( etStringActual );
    etStringFree( etStringActual );


// End Timer
    etDebugMessage( etID_LEVEL_DETAIL, "Test finished" );
    return etID_YES;
}

