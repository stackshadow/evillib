/* etFolder - Functions to handle folders
	Copyright (C) 2013 by Martin Langlotz

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

#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"
	#include "etDebug.h"

	#include "System/etFolder.h"
#endif



etID_STATE				etFolder_create_recursiv( const char *dir, mode_t mode, int IgnoreLevel ){
// Error handling
	if( dir == NULL ){
		return etDebugState( etID_STATE_PARAMETER_MISSUSE );
	}


	const char *c = dir + 1;
	int mkdirstatus = 0;
	int LevelCount = 0;

	while( *c != '\0' ){

        if(*c == '/'){

        // Increment Level
        	LevelCount++;

        	if( LevelCount > IgnoreLevel ){

			// Set the '/' to 0 ( string ends here )
				*((char*)c) = '\0';

			// Create directory
				mkdirstatus = mkdir( dir, mode );

				if( mkdirstatus == -1 && errno != EEXIST ){
				//if( mkdirstatus == -1 ){
				#ifdef ET_DEBUG
					snprintf( etDebugTempMessage, etDebugTempMessageLen, "could not create directory %s", dir );
				#endif
					etDebugMessage( etID_STATE_ERR, etDebugTempMessage );
					return etID_STATE_ERR;
				} else {
				#ifdef ET_DEBUG
					snprintf( etDebugTempMessage, etDebugTempMessageLen, "directory %s created", dir );
					etDebugMessage( etID_STATE_INFO, etDebugTempMessage );
				#endif
				}

			// Switch back
				*((char*)c) = '/';

			}
		}

		c++;
	}


    return etID_YES;
}


etID_STATE				etFolder_remove( const char *dir ){
// Error handling
	if( dir == NULL ){
		return etDebugState( etID_STATE_PARAMETER_MISSUSE );
	}


	int rmdirstatus;
	rmdirstatus = rmdir( dir );
	switch( rmdirstatus ){
		case EEXIST:
		#ifdef ET_DEBUG
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "directory %s not exist", dir );
		#endif
			etDebugMessage( etID_STATE_ERR, etDebugTempMessage );
			return etID_STATE_ERR;
			break;
		case ENOTEMPTY:
		#ifdef ET_DEBUG
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "directory %s is not empty", dir );
		#endif
			etDebugMessage( etID_STATE_ERR, etDebugTempMessage );
			return etID_STATE_ERR;
			break;
		case 0:
			break;
		default:
		#ifdef ET_DEBUG
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "error %i occure on directory %s", rmdirstatus, dir );
		#endif
			etDebugMessage( etID_STATE_ERR, etDebugTempMessage );
			return etID_STATE_ERR;
	}

#ifdef ET_DEBUG
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "%s deleted", dir );
	etDebugMessage( etID_STATE_INFO, etDebugTempMessage );
#endif

	return etID_YES;
}

