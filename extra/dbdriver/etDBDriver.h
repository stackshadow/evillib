/* etDB - evillib json DB representation
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

#ifndef _H_etjDBDriver
#define _H_etjDBDriver

#ifdef ET_SINGLEMODULE
	#include "evillib_defines.h"
	#include "evillib_depends.h"
	
	#include "etDB.h"
#endif

typedef struct		etDBDriver etDBDriver;
typedef struct		etDBDriver {

	etID_STATE		(*queryRun)( etDBDriver *etjDBDriverActual, etDB *etDBActual );
	etID_STATE		(*nextResult)( etDBDriver *etjDBDriverActual, etDB *etDBActual );
	etID_STATE		(*dump)( etDBDriver *etjDBDriverActual, etDB *etDBActual );

// callback functions ( can be set from the userspace )
	etID_STATE		(*queryPreRun)( etDB *etDBActual, etString *query );
	
} etDBDriver;



etID_STATE etDBRun( etDBDriver *etjDBDriverActual, etDB *etDBActual );
etID_STATE etDBNextResult( etDBDriver *etjDBDriverActual, etDB *etDBActual );


#endif

