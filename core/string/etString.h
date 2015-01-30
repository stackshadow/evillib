/* etString - String handling system
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

#ifndef _H_etString
#define _H_etString



/** @ingroup gr_etString
@~english
@brief The etString object
**/


typedef struct etString {
	int				lengthActual; /*!< The number of actual used chars */
	int				lengthMax; /*!< The number of actual used chars */

	etMemoryBlock	*data; 	/*!< The memory for the string */
} etString;





#define					etStringAlloc( etStringActual ) __etStringAlloc( &etStringActual )
etID_STATE				__etStringAlloc( etString **p_etStringActual );

#define 				etStringAllocLen( etStringActual, NewLen ) __etStringAllocLen( &etStringActual, NewLen )
etID_STATE				__etStringAllocLen( etString **p_etStringActual, int NewLen );


etID_STATE				etStringInit( etString *etStringActual );


etID_STATE				etStringClean( etString *etStringActual );


etID_STATE				etStringDestroy( etString *etStringActual );

#define 				etStringFree( etStringActual ) __etStringFree( &etStringActual )
etID_STATE				__etStringFree( etString **p_etStringActual );


















#endif

