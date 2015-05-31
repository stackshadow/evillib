
/* etjDB - evillib json DB representation
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
	#include "evillib_defines.h"
	#include "evillib_depends.h"

	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	
	#include "etjDB.h"
	#include "etjDBDriver.h"
#endif



etID_STATE etjDBRun( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual ){
	return etjDBDriverActual->queryRun( etjDBDriverActual, etjDBActual );
}

etID_STATE etjDBNextResult( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual ){
	return etjDBDriverActual->nextResult( etjDBDriverActual, etjDBActual );
}

