/*  Copyright (C) 2015 by Martin Langlotz alias stackshadow

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

#ifndef _H_etThread
#define _H_etThread


/** @ingroup grThread
@~english

@brief This object, define one thread
*/

#include "evillib_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct         etThread {
    pthread_t            thread;                            /*!< The pthread-instance */

    etID_STATE            state;                            /*!< State of the thread. Can be @ref etID_STATE_READY, @ref etID_STATE_RUN */
    void*                 (*ThreadFunction)(void*);    /*!< The function which is called inside the thread */
    void*                userData;                        /*!< The Userdata which is passed to the ThreadFunction */
} etThread;










#define                    etThreadAlloc( etThreadActual ) __etThreadAlloc( &etThreadActual )
etID_STATE                __etThreadAlloc( etThread **p_etThreadActual );

#define                    etThreadFree( etThreadActual ) __etThreadFree( &etThreadActual )
etID_STATE                __etThreadFree( etThread **p_etThreadActual );


void                    etThreadSetFunction( etThread *etThreadActual, void (*ThreadFunction)(void*) );


void                    etThreadSetUserdata( etThread *etThreadActual, void *userData );

#define                    etThreadGetUserdata( etThreadActual, userData ) __etThreadGetUserdata( etThreadActual, &userData )
void                    __etThreadGetUserdata( etThread *etThreadActual, void **p_userData );


etID_STATE                etThreadRun( etThread *etThreadActual );


etID_STATE                etThreadKill( etThread *etThreadActual );


etID_STATE                etThreadFinish( etThread *etThreadActual );


void                    etThreadWait( etThread *etThreadActual );





#ifdef __cplusplus
}
#endif

#endif
