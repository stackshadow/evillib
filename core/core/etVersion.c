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

#ifdef ET_SINGLEMODULE
	#include "evillib_depends.h"
	#include "evillib_defines.h"
	#include "evillib_version.h"

	#include "core/etDebug.h"
	#include "core/etVersion.h"

	#include "memory/etMemoryBlock.h"
#endif

/** @cond DEV */


/** @ingroup grCore
@~english

@brief Return the versionstring of the evillib
*/
const char*		etLibVersion(){
	return ET_VERSIONSTRING;
}

/** @ingroup grCore
@~english

@brief Check the version of the evillib
This function check the correct Version during the runtime. \n
If the version does not match, just an message is printed
*/
etID_STATE		etVersionCheck( int Main, int Minor, int Patch ){

	if( Main == ET_VERSION_MAJOR )
	if( Minor == ET_VERSION_MINOR )
	if( Patch == ET_VERSION_PATCH ){

		// Message
		fprintf( stdout, "evillib version ok\n" );

		return etID_YES;
	}

	fprintf( stdout, "evillib version has changed\n" );
	return etID_NO;
}










/** @endcond */
