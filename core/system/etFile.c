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

#ifndef _C_etFile
#define _C_etFile

// binary stuff we need
#include "core/etDebug.c"

#include "evillib_depends.h"

#include "core/etDebug.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @ingroup grMemory
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Check if an file exist in a filesystem

@param[in] directory The directory in where we look for the file
@param[in] entryName An Filename/Foldername you try top find
@return If the etMemory object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE      etFileExist( const char* directory, const char* entryName ){
    etDebugCheckNull(directory);
    etDebugCheckNull(entryName);


	DIR*                DirectoryPointer;
	struct dirent*      DirectoryEntry;
	int					FileNameLen = strlen(entryName);
	int					FileFound = etID_NO;

	// Open device directory
	DirectoryPointer = opendir( directory );
	if (DirectoryPointer == NULL) {
		perror ("Couldn't open the directory");
		return etID_NO;
	}

	// Read every entry
	while( (DirectoryEntry = readdir(DirectoryPointer)) && ( DirectoryEntry != NULL ) ){

		//if( DirectoryEntry->d_type == DT_ )
		if( strncmp( entryName, DirectoryEntry->d_name, FileNameLen ) == 0 ){
			FileFound = etID_YES;
			break;
		}

	}

	// Close directory
	closedir( DirectoryPointer );


	// Return if the file was found
	return FileFound;
}




#ifdef __cplusplus
}
#endif






#endif
