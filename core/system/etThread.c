/* etThread - threading functions of the evillib
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
	
	#include "system/etThread.h"
#endif

/** @cond DEV */
/** @defgroup grThread etThread - Threading functions
@brief Threading functions in evillib

Threads are background task. Background tasks are used to run some functions in the background of the main Thread.
The threading functions in evillib use pthread (so we need an c-library)
@todo Add Timeout and "Ping" Functions. "Ping"-Function should "ping" the thread, the background thread should respond in an specific amount of time

@dotfile system/etThreadUsage.dot "Typical usage graph" width=10cm


Here is an example for an etThread use:

@snippet app/apicheck/etThread.c etThread function
@snippet app/apicheck/etThread.c etThread
*/


/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etThreadAlloc( etThreadActual )

@~english
@brief Allocate a new etThread object

@param[out] etThreadActual The pointer to an etThread object
@return If the etThread-object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*/
etID_STATE				__etThreadAlloc( etThread **p_etThreadActual ){
// Check
	etCheckNull( p_etThreadActual );


// Vars
	etMemoryBlock 	*etMemoryBlockNew = NULL;
	etThread 		*etThreadActual = NULL;


// Allocate etString
	etMemoryAlloc( etMemoryBlockNew, sizeof(etThread) );
	etMemoryBlockDataGet( etMemoryBlockNew, etThreadActual );
	if( etThreadActual == NULL ){
			*p_etThreadActual = NULL;
			return etDebugState(etID_STATE_NOMEMORY);
	}

// Debug
#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "etThreadAlloc: %p", etThreadActual );
	etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif


// Thread is empty
	etThreadActual->state = etID_STATE_READY;


// Return
	*p_etThreadActual = etThreadActual;
	return etID_YES;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etThreadFree( etThreadActual )

@~english
@brief Free an etThread object

This also kills an running thread
@param[out] etThreadActual The pointer to an etThread object
@return If the etThread-object was correctly freed \n
*- @ref etID_YES
*/
etID_STATE				__etThreadFree( etThread **p_etThreadActual ){
// Check
	etCheckNull( p_etThreadActual );
	
// Vars
	etMemoryBlock 	*etMemoryBlockActual = NULL;

// Kill Thread before free
	etThreadKill( *p_etThreadActual );
	
// release memory
	etMemoryBlockFromData( *p_etThreadActual, etMemoryBlockActual );
	etMemoryRelease( etMemoryBlockActual );

// return
	*p_etThreadActual = NULL;
	return etID_YES;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Set the function which should be run in background

The function is just set and will not run until you call etThreadRun().
@param[in] etThreadActual The etThread object
@param[in] ThreadFunction The function which get called inside the thread
*/
void					etThreadSetFunction( etThread *etThreadActual, void (*ThreadFunction)(void*) ){
// Check
	etCheckNullVoid( etThreadActual );

	etThreadActual->ThreadFunction = (void* (*)(void*))ThreadFunction;

	return;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Set the userdate

This sets the userdata which can be obtained by etThreadGetUserdata() inside the thread
@param[in] etThreadActual The etThread object
@param[in] userData The userdata which can be recieved inside the thread with etThreadGetUserdata()
*/
void					etThreadSetUserdata( etThread *etThreadActual, void *userData ){
// Check
	etCheckNullVoid( etThreadActual );

	etThreadActual->userData = userData;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn void etThreadGetUserdata( etThreadActual, userData )

@~english
@brief Get the userdata from the etThread-object

Get the userdata from the etThread-object which was set before etThreadSetUserdata()
@param[in] etThreadActual The etThread object
@param[out] userData The userdata which can be recieved inside the thread with etThreadGetUserdata()
*/
void					__etThreadGetUserdata( etThread *etThreadActual, void **p_userData ){
// Check
	etCheckNullVoid( etThreadActual );

	*p_userData = etThreadActual->userData;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Start the function in background which was set before with etThreadSetFunction()

This function start an background thread of the function wich was set with etThreadSetFunction() \n
The thread only starts if etThread is in etID_STATE_READY state \n
@param[in] etThreadActual The etThread object
@return If the thread was correctly started \n
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_YES
*- @ref etID_STATE_USED - Function already run in an thread
*- @ref etID_STATE_NODATA - No Function was set with etThreadSetFunction()
*/
etID_STATE				etThreadRun( etThread *etThreadActual ){
// Check
	etCheckNull( etThreadActual );
	
// Check state
	if( etThreadActual->state != etID_STATE_READY ){
		etDebugMessage( etID_LEVEL_ERR, "Thread already running !" );
		return etID_STATE_USED;
	}

// Run Thread if function exist
	if( etThreadActual->ThreadFunction != NULL ){
		etThreadActual->state = etID_STATE_RUN;

		pthread_create( &etThreadActual->thread, NULL, etThreadActual->ThreadFunction, etThreadActual );

	// Debug
		#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "[RUN] %li", etThreadActual->thread );
			etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
		#endif

	} else {
		etDebugMessage( etID_LEVEL_ERR, "Can not run the function.. no function is present to run" );
		return etID_STATE_NODATA;
	}


	return etID_YES;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Kill an running thread

If the thread is not in "run"-state nothing will be done
@param[in] etThreadActual The etThread object
@return If the thread was killed \n
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE				etThreadKill( etThread *etThreadActual ){
// Check
	etCheckNull( etThreadActual );

	if( etThreadActual->state == etID_STATE_RUN ){

	// Debug
		#ifndef ET_DEBUG_OFF
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "[KILL] %li", etThreadActual->thread );
		etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
		#endif
		
		
		if( pthread_cancel(etThreadActual->thread) == 0 ){
			#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "[KILLED] %li", etThreadActual->thread );
			etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
			#endif
			
			etThreadActual->state = etID_STATE_BREAK;

			return etID_YES;
		} else {
			#ifndef ET_DEBUG_OFF
			snprintf( etDebugTempMessage, etDebugTempMessageLen, "[NOT KILLED] %li", etThreadActual->thread );
			etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
			#endif
			
			return etID_NO;
		}
	}
	
	return etID_NO;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Send an signal to the calling process that this thread is finished.

This function can only be called inside the thread ! It will set the thread state
to ready. If you run tis function outside an thread nothing will be done.
@param[in] etThreadActual The etThread object
@return If the thread was set to finished \n
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_YES
*- @ref etID_NO
*/
etID_STATE				etThreadFinish( etThread *etThreadActual ){
// Check
	etCheckNull( etThreadActual );
	
// Works only inside the thread
	if( etThreadActual->thread == pthread_self() ){
		etThreadActual->state = etID_STATE_READY;
		
	// Debug
		#ifndef ET_DEBUG_OFF
		snprintf( etDebugTempMessage, etDebugTempMessageLen, "[SET FINISHED] %li", etThreadActual->thread );
		etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
		#endif

		pthread_detach(etThreadActual->thread);
		pthread_exit(NULL);
		
		return etID_YES;
	} else {
		etDebugMessage( etID_LEVEL_ERR, "This thread is not the thread you would like to finish" );
	}

	return etID_NO;
}

/** @ingroup grThread
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english
@brief Wait for an thread to finish.

@param[in] etThreadActual The etThread object
*/
void					etThreadWait( etThread *etThreadActual ){
// Check
	etCheckNullVoid( etThreadActual );

// Debug
	#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "[WAIT FOR FINISH] %li", etThreadActual->thread );
	etDebugMessage( etID_LEVEL_DETAIL_THREAD, etDebugTempMessage );
	#endif

// Wait for thread
	pthread_join( etThreadActual->thread, NULL );

}



/** @endcond */
