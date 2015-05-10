/* etComm - Communication with other apps
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

#ifndef _H_etComm
#define _H_etComm

typedef struct etComm etComm;
typedef void etCommConnection;

/**
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@date 03/28/15
@brief The common etComm Type for all communication Types
 */
typedef struct etComm {
	etID_TYPE 			commType;

	etID_STATE			(*serve)(etComm *etCommActual, etID_STATE (*newClientConnection)(etComm*) );
	etID_STATE			(*read)(etCommConnection *etCommConnectionActual, etID_STATE (*readData)(etComm*, char*, int) );

	etCommConnection	*connection;
} etComm;


#define					etCommAlloc( etCommActual ) __etCommAlloc( &etCommActual )
etID_STATE				__etCommAlloc( etComm **p_etCommActual );






#endif