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

#include "evillib_depends.h"

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "system/etFile.h"


int etFileArraySize = 10;


etID_STATE          __etFileAlloc( etFile **p_etFileActual ){
    etDebugCheckNull( p_etFileActual );

// vars
    etFile      *etFileNew = NULL;

// allocate
    etMemoryRequest( etFileNew, sizeof(etFile) );

// init
    etFileNew->pathArraySize = etFileArraySize;
    etMemoryRequest( etFileNew->pathArray, etFileNew->pathArraySize * sizeof(void*) );

// return
    *p_etFileActual = etFileNew;
    return etID_YES;
}


etID_STATE          etFileAppend( etFile *file, etString *path ){
// check
    etDebugCheckNull( file );
    etDebugCheckNull( path );

// vars
    etString        *actualPath = NULL;
    etString        **newPathArray = NULL;

// try to set
    for( int index = 0; index < file->pathArraySize; index++ ){

        actualPath = file->pathArray[index];
        
        if( actualPath == NULL ){
            file->pathArray[index] = path;
            return etID_YES;
        }

    }

// oh now more memory, create bigger one
    etMemoryRequest( newPathArray, (file->pathArraySize+etFileArraySize) * sizeof(void*) );
    memcpy( newPathArray, file->pathArray, file->pathArraySize * sizeof(void*) );
    etMemoryRelease( file->pathArray );
    file->pathArray = newPathArray;
    
    file->pathArray[file->pathArraySize] = path;
    
    file->pathArraySize = file->pathArraySize + etFileArraySize;



    return etID_YES;
}


etID_STATE          __etFileFree( etFile **p_etFileActual ){
    etDebugCheckNull( p_etFileActual );
    etDebugCheckNull( *p_etFileActual );
    
    
    etMemoryRelease( (*p_etFileActual)->pathArray );
    etMemoryRelease( *p_etFileActual );
    
    
    *p_etFileActual = NULL;
    return etID_YES;
}








