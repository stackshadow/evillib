/* The evillib app
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
#include "evillib_version.h"
#include "evillib_full.h"

#include "app/apicheck/etApicheck.h"
#include "core/etInit.h"


/** @defgroup grApp The evillib-app
@brief The evillib App can be used to check, if the installed library works correctly
*/

int main( int argc, const char* argv[] ){
	etInit( argc, argv );

	__builtin_return_address(0);


/*
    void *handle;
    double (*cosine)(double);
    char *error;

   handle = dlopen("libm.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

   dlerror();



   *(void **) (&cosine) = dlsym(handle, "cos");

   if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

	return 0;
*/

	const char *Argument1 = NULL;
	const char *Argument2 = NULL;
				
	if( argc > 0 ){

		int index = 1;
		for ( index = 1; index <= argc; index ++ ){

				Argument1 = argv[index];
				
				if( index+1 <= argc ){
					Argument2 = argv[index+1];
				} else {
					Argument2 = NULL;
				}

				if( Argument1 != NULL ){

					if( strncmp(Argument1, "--apicheck", 10) == 0 ){
						etDebugLevelSet( etID_LEVEL_ALL );
						etDebugProgramNameSet( "apicheck" );
						
						int checkAll = 0;
						
						if( Argument2 != NULL ){
							
							if( strncmp(Argument2, "all", 8) == 0 ){
								checkAll = 1;
							}

							if( strncmp(Argument2, "etMemory", 8) == 0 || checkAll == 1 ){
								etMemoryBlockTest();
								etMemoryTest();
							}
							if( strncmp(Argument2, "etString", 8) == 0 || checkAll == 1 ){
								etStringTest();
								etStringCharTest();
							}
							if( strncmp(Argument2, "etThread", 8) == 0 || checkAll == 1 ){
								etThreadTest();
							}
							if( strncmp(Argument2, "etComm", 8) == 0 || checkAll == 1 ){
								etCommTest();
							}
							
							
						}


						
						continue;
					}
					

					if( strncmp( Argument1, "-h", 2 ) == 0 || strncmp( Argument1, "--help", 6 ) == 0 ){
						fprintf( stdout, "APP:\n" );
						fprintf( stdout, "--apicheck:   check the api and integrity of the evillib\n" );
						fprintf( stdout, "\n" );
					}

				}
			}
	}


	return 0;
}




