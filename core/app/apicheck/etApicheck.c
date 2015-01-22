/* Apicheck Functions
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

#include "evillib_defines.h"
#include "evillib_depends.h"
#include "evillib_version.h"

#include "core/etDebug.h"
#include "app/apicheck/etApicheck.h"


etApicheck etApicheckActualStatic;
etApicheck *etApicheckActual = &etApicheckActualStatic;


etID_TYPE		etApicheckTimerInit(){
	etApicheckActual->timeRunning = etID_FALSE;
	
	return etID_YES;
}


etID_TYPE		etApicheckTimer( const char *Function, int FunctionLine ){

// Timer is not running, start it
	if( etApicheckActual->timeRunning == etID_FALSE ){
		
		etApicheckActual->timeRunning = etID_TRUE;
		etApicheckActual->timerStart = clock();
		etApicheckActual->timeTaken = 0;
		
		return etID_YES;
	}
	
// Timer is running, calculate difference
	if(  etApicheckActual->timeRunning == etID_TRUE ){
		
		etApicheckActual->timeRunning = etID_FALSE;
		etApicheckActual->timerEnd = clock();
		etApicheckActual->timeTaken = etApicheckActual->timerEnd - etApicheckActual->timerStart;
		
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "Running: %f ticks", etApicheckGetTime() );
		_etDebugMessage( etID_LEVEL_DETAIL, Function, FunctionLine, etDebugTempMessage );

		return etID_YES;
	}

	return etID_YES;
}


double			etApicheckGetTime(){
	return (double)etApicheckActual->timeTaken;
}



