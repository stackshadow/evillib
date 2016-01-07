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
#include "evillib_version.h"


#include "core/etInit.h"

#include "core/etDebug.c"
#include "core/etVersion.c"
#include "core/etInit.c"

#include <dlfcn.h>

/** @defgroup grApp The evillib-app
@brief The evillib App can be used to check, if the installed library works correctly

just start the app with -h to get help :)
*/


int runSharedObjectFunction( const char *path, const char *name ){

// vars
    char        filename[512];
    int         pathLen = strlen( path );
    int         nameLen = strlen( name );

// check boundrys
    if( (pathLen + nameLen + 2) > 512 ) exit(-1);

// check if the file ended with .so
    if( strncmp( &name[nameLen-3], ".so", 3 ) != 0 ){
        fprintf( stdout, "%s is not an shared Object\n", name );
        return -1;
    }


// build the full path filename
    memcpy( &filename[0], path, pathLen );
    memcpy( &filename[pathLen], "/", 1 );
    memcpy( &filename[pathLen+1], name, nameLen );
    filename[pathLen+nameLen+1] = 0;
    
    void *handle = dlopen( filename, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 0;
    }                        
    
// build the function name
    memcpy( &filename[0], name, nameLen );
    memcpy( &filename[nameLen-3], "ApiCheck", 8 );
    filename[nameLen-3+8] = 0;

// try to run this function
    int (*symbol)(void);
    symbol = dlsym ( handle, filename );
    if ( symbol == NULL ) {
        fprintf(stderr, "%s\n", dlerror());
        dlclose(handle);
        return 0;
    }
    
    return symbol();
    
    dlclose(handle);
}


int main( int argc, const char* argv[] ){

    __builtin_return_address(0);


    const char *argument1 = NULL;
    const char *argument2 = NULL;
    const char *argument3 = NULL;
                
    if( argc > 0 ){

        int index = 1;
        for ( index = 1; index <= argc; index ++ ){

        // arguement 1
            argument1 = argv[index];
            
        // argument 2
            if( index+1 <= argc ){
                argument2 = argv[index+1];
            } else {
                argument2 = NULL;
            }

        // argument 3
            if( index+2 <= argc ){
                argument3 = argv[index+2];
            } else {
                argument3 = NULL;
            }


            if( argument1 != NULL ){

                if( strncmp(argument1, "--apicheck", 10) == 0 ){
                    etDebugLevelSet( etID_LEVEL_WARNING );
                    etDebugLevelSet( etID_LEVEL_ALL );
                    etDebugProgramNameSet( "apicheck" );
                    
                // we need args 2 + 3
                    if( argument2 == NULL || argument3 == NULL ){
                        exit(-1);
                    }
                    

                    DIR                 *directory;
                    struct dirent       *directoryEntry;
                    
                        
                    // Open device directory
                    directory = opendir( argument2 );
                    if( directory == NULL ) {
                        perror ("Couldn't open the directory");
                        exit(0);
                    }

                    if( strncmp(argument3, "all", 3) == 0 ){
                        
                        // Read every entry
                        while( (directoryEntry = readdir(directory)) && ( directoryEntry != NULL ) ){
                            
                        // ignore . / ..
                            if( strncmp( directoryEntry->d_name, ".", 1 ) == 0 ) continue;
                            
                            runSharedObjectFunction( argument2, directoryEntry->d_name );
                        }
                    } else {
                        runSharedObjectFunction( argument2, argument3 );
                    }
                    
                    closedir(directory);



                    
                    continue;
                }
                    

                if( strncmp( argument1, "-h", 2 ) == 0 || strncmp( argument1, "--help", 6 ) == 0 ){
                    fprintf( stdout, "APP:\n" );
                    fprintf( stdout, "--apicheck <path> <all|name>:  check the api and integrity of the evillib\n" );
                    fprintf( stdout, "\n" );
                }

        
            }
        }
    }


    return 0;
}




