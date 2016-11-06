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


#include "evillib_defines.h"
#include "evillib_depends.h"


#include "core/etInit.h"
#include "core/etInit.c"

#include "system/etFile.h"
#include "system/etFile.c"

#include "app/etApicheck.h"
#include "app/etApicheck.c"


int                     main( int argc, const char* argv[] ){
    etInit( argc, argv );
    etDebugLevelSet( etID_LEVEL_ALL );
    etMemoryDump( NULL, NULL );


    etApicheckTimer( "etFileExist: check" );

// parameter check
    if( etFileExist( NULL, "fstab" ) != etID_STATE_ERR_PARAMETER ){
        return etID_LEVEL_CRITICAL;
    }
    if( etFileExist( "/etc", NULL ) != etID_STATE_ERR_PARAMETER ){
        return etID_LEVEL_CRITICAL;
    }

// this file should NOT exist !
    if( etFileExist( "/etc", "DontExist" ) != etID_NO ){
        etDebugMessage( etID_LEVEL_CRITICAL, "etFileExist sees a file which not exists !" );
        return etID_LEVEL_CRITICAL;
    }

// this file MUST exist in a unix system !
    if( etFileExist( "/etc", "fstab" ) != etID_YES ){
        etDebugMessage( etID_LEVEL_CRITICAL, "etFileExist sees a file which not exists !" );
        return etID_LEVEL_CRITICAL;
    }




    etApicheckTimer( "OK" );



    etMemoryDump( NULL, NULL );
    return 0;
}
