/*  Copyright (C) 2014-2016 by Martin Langlotz alias stackshadow

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

#ifndef _H_etStringChar
#define _H_etStringChar

#include "evillib_defines.h"
#include "string/etString.h"

#ifdef __cplusplus
extern "C" {
#endif


#define                 etStringCharGet( etStringActual, char ) __etStringCharGet( etStringActual, &char )
etID_STATE              __etStringCharGet( etString *etStringActual, const char **p_char );

#define                 etStringWCharGet( etStringActual, p_wchar ) __etStringWCharGet( etStringActual, &p_wchar )
etID_STATE              __etStringWCharGet( etString *etStringActual, wchar_t **p_wchar );


#define                 etStringCharCopy( etStringActual, dest, maxLen ) __etStringCharCopy( etStringActual, (char*)(&dest), maxLen )
etID_STATE              __etStringCharCopy( etString *etStringActual, char *target, int maxLen );


etID_STATE              etStringCharSet( etString *etStringActual, const char *source, int maxLen );


etID_STATE              etStringWCharSet( etString *etStringActual, wchar_t *source, int maxLen );


etID_STATE              etStringCharAdd( etString *etStringActual, const char *source );


etID_STATE              etStringWCharAdd( etString *etStringActual, wchar_t *source );


int                     etStringCharFind( etString *etStringActual, const char *compareString, int offset );


#ifdef __cplusplus
}
#endif

#endif
