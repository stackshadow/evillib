/* Apicheck helper functions
    Copyright (C) 2014 by Martin Langlotz

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


#ifndef _H_evillib_etApicheck
#define _H_evillib_etApicheck

#include "evillib_depends.h"

#include "core/etIDState.h"
#include "evillib_defines.h"

typedef struct etApicheck_s {
    etID_BOOL       timeRunning;
    clock_t         timerStart;
    clock_t         timerEnd;
    clock_t         timeTaken;
} etApicheck;






etID_TYPE        etApicheckTimerInit();


etID_TYPE        etApicheckTimer( const char *testText );


double            etApicheckTimeGet();





etID_STATE            etMemoryTest();
etID_STATE            etMemoryBlockTest();
etID_STATE            etStringTest();
etID_STATE            etStringCharTest();

#endif