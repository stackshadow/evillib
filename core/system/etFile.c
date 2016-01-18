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
#include "evillib-extra_depends.h"

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "system/etFile.h"


int etFileArraySize = 10;




etID_STATE          __etFileAlloc( etFile **p_etFileActual ){
    etDebugCheckNull( p_etFileActual );

// vars
    etFile          *etFileNew = NULL;


// allocate a one pointer array
    etMemoryRequest( etFileNew, sizeof(etFile) );

// alloc array
    etMemoryRequest( etFileNew->pathArray, sizeof(etString*) );
    etStringAlloc( etFileNew->checksum );


// setup etFile
    etFileNew->pathArraySize = 1;


// return
    *p_etFileActual = etFileNew;
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


etID_STATE          etFileAppend( etFile *file, etString *path ){
// check
    etDebugCheckNull( file );
    etDebugCheckNull( path );

// vars
    int                 etFileArrayMemSize = file->pathArraySize * sizeof(etString*);

// try to set
    for( int index = 0; index < file->pathArraySize; index++ ){

        if( file->pathArray[index] == NULL ){
            file->pathArray[index] = path;
            return etID_YES;
        }

    }

// oh no more memory, create bigger one
    etString    **pathArray = NULL;
    etMemoryRequest( pathArray, etFileArrayMemSize * 2 );
    memcpy( pathArray, file->pathArray, etFileArrayMemSize );
    etMemoryRelease( file->pathArray );
    file->pathArray = pathArray;
    
    
    file->pathArray[file->pathArraySize] = path;
    file->pathArraySize = etFileArrayMemSize * 2;
    



    return etID_YES;
}


etID_STATE          etFileValidateChecksum( etFile *file ){
    
// vars
    MD5_CTX     ctx;
    size_t      bytes;
    char        fileData[1024];
    

    MD5_Init( &ctx );

    fopen(  )

    while( (bytes = fread( fileData, 1, 1024, inFile )) != 0 ){
        MD5_Update( &ctx, fileData, bytes );
    }




}





