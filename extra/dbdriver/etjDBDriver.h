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

#ifndef _H_etjDBDriver
#define _H_etjDBDriver

typedef struct		etjDBDriver etjDBDriver;
typedef struct		etjDBDriver {

	etID_STATE		(*queryRun)( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual );
	etID_STATE		(*nextResult)( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual );
	etID_STATE		(*dump)( etjDBDriver *etjDBDriverActual, etjDB *etjDBActual );

// callback functions ( can be set from the userspace )
	etID_STATE		(*queryPreRun)( etjDBDriver *etjDBDriverActual, etString *query );
	
} etjDBDriver;




#endif

