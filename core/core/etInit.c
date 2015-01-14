/* The evillib version functions
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

/** @cond DEV */

#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"
	#include "evillib_version.h"

	#include "core/etDebug.h"
	#include "core/etInit.h"

	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
#endif


// Define some apicheck functions
etID_STATE	APICHECK_etMemory();
void		APICHECK_etList();

/** @defgroup grCoreCommandLine Default Command line options

The default command line option is aviable in every program which uses the evillib

# Command line options
## --debug
enable all debugging informations

## --version
return the version of th evillib

## -h
print out the aviable command line options and enabled options


*/

/** @ingroup grCore
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Init the evillib

Call this function before you do something with the evillib.

@param argc Parameter count from main()-function
@param argv Parameter array from main()-function
*/
etID_STATE		etInit( int argc, const char* argv[] ){



// Command line parameters
	if( argc > 0 ){
		int index = 1;
		for ( index = 1; index <= argc; index ++ ){

				const char *Argument1 = argv[index];

				if( Argument1 != NULL ){


					if( strncmp( Argument1, "--debug", 7 ) == 0 ){
						etDebug_Level_set( etID_LEVEL_ALL );
						continue;
					}

					if( strncmp( Argument1, "--version", 9 ) == 0 ){
						fprintf( stdout, "%s\n", ET_VERSIONSTRING );
						exit(0);
						return etID_OK;
					}

					if( strncmp( Argument1, "-h", 2 ) == 0 || strncmp( Argument1, "--help", 6 ) == 0 ){
						fprintf( stdout, "evillib version %s\n", ET_VERSIONSTRING );
						fprintf( stdout, "Copyleft by Martin Langlotz aka stackshadow\n" );
						fprintf( stdout, "www.evilbrain.de\n\n" );
						

				// Feature List
						fprintf( stdout, "Features:" );

					// Debugging
						fprintf( stdout, "\n- Debugging is " );
						#ifndef ET_DEBUG_OFF
							fprintf( stdout, "aviable" );
						#else
							fprintf( stdout, "not aviable" );
						#endif

					// Secure Memory
						fprintf( stdout, "\n- Secure Memory is " );
						#ifndef ET_SECURE_MEMORY_OFF
							fprintf( stdout, "enabled" );
						#else
							fprintf( stdout, "disabled" );
						#endif

					// Block device
						fprintf( stdout, "\n- Blockdevice support is " );
						#ifndef ET_BLOCKDEVICE_OFF
							fprintf( stdout, "enabled, blkid-version: %s", BLKID_VERSION );
						#else
							fprintf( stdout, "disabled" );
						#endif


						fprintf( stdout, "\n\n" );
						fprintf( stdout, "Help:\n" );
						fprintf( stdout, "--debug:      enable debugging messages\n" );
						fprintf( stdout, "--version:    print out the evillib version and exit\n" );
						fprintf( stdout, "\n" );
					}

				}
			}
	}



// Set debug-program-name

// A global Mutex
	pthread_mutex_unlock( &etDebugEvillib->Sync );

// Init Memory system
	if( etMemoryInit() != etID_YES ) return etID_NO;



// From here - Userspace
// Set program
	etDebug_ProgramName_set( "program" );
// Dont show any Debugging as default
	etDebug_Level_set( etID_LEVEL_CRITICAL );


	return etID_YES;
}


/** @endcond DEV */