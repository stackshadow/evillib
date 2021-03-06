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

#include "evillib_depends.h"

#ifndef _C_etString
#define _C_etString

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemory.h"
#include "string/etString.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup grString etString - String functions
@~english
@brief Core String functions

The core functions handle allocation, freeing, reallocation and basic informations about an etString. \n
The etString uses the etMemory subsystem to allocate/free/copy/set memory \n
*/

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etStringAlloc( etStringActual )

@~english
@brief Allocate a new etString object
@see standardFunctions

@param[out] etStringActual The pointer to an etString object pointer
@return If the etString object was correctly allocated \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                __etStringAlloc( etString **p_etStringActual ){
    return __etStringAllocLen( p_etStringActual, 32 );
}

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etStringAllocLen( etStringActual, NewLen )

@~english
@brief Allocate a new etString with preallocated Memory

If your string change very frequently, us this function to speed up your application.

@param[out] p_etStringActual The pointer to an etString object pointer
@param[in] NewLen The length of the new String
@return If the etString object was correctly allocated with dedicated len\n
*- @ref etID_YES
*- @ref etID_STATE_NOMEMORY
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                __etStringAllocLen( etString **p_etStringActual, int NewLen ){
// Object okay ?
    etDebugCheckNull( p_etStringActual );

// Vars
    etString            *etStringActual = NULL;

// Allocate etString
    etMemoryAlloc( etStringActual, sizeof(etString) );
    if( etStringActual == NULL ){
        *p_etStringActual = NULL;
        return etDebugState(etID_STATE_CRIT_NOMEMORY);
    }

// String properties
    etStringActual->lengthActual = 0;
    etStringActual->lengthMax = NewLen;
    etStringActual->isWChar = etID_NO;

// Allocate memory for string
    if( NewLen > 0 ){
        etMemoryAlloc( etStringActual->data, sizeof(char)*NewLen );
    }

// Return
    *p_etStringActual = etStringActual;
    return etID_YES;
}

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Set an etString object to the initial state ( mainly for static objects ) \n
@see standardFunctions

@param[in] etStringActual The pointer to an etString object
@return If the etString object was correctly init \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                etStringInit( etString *etStringActual ){
// Object okay ?
    etObjectCheckGetter( etStringActual );

// Debug
#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p]", etStringActual );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

// String properties
    etStringActual->lengthActual = 0;
    etStringActual->lengthMax = 0;
    etStringActual->data = NULL;

    return etID_YES;
}

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Clears the etString ( memory keeps allocated )
@see standardFunctions

Overwrite the string data with zeros, but dont free the memory. \n
This is mainly to clear out an etString, if you have passwords or something else important inside. \n

@param[in] etStringActual The pointer to an etString object
@return If the etString object was correctly cleaned \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                etStringClean( etString *etStringActual ){
// Parameter check
    etObjectCheckSetter( etStringActual );

// Debug
    #ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p]", etStringActual );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
    #endif

// Clean the data
    if( etMemoryClean( etStringActual->data ) != etID_YES ){
        return etObjectStateSet( etStringActual, etID_STATE_WARN_INTERR );
    }

// set length of etString to 0
    etStringActual->lengthActual = 0;

// return
    return etID_YES;
}

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english

@brief Release the memory inside the etString

This function release the memory of an etString but not the etString-Object itselfe
@param[in] etStringActual The pointer to an etString object
@return If the etString object was correctly destroyed \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                etStringDestroy( etString *etStringActual ){
// Parameter check
    etObjectCheckSetter( etStringActual );


// Debug
    #ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p]", etStringActual );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
    #endif

// Release the data
    if( etStringActual->data != NULL ){
        etMemoryRelease( etStringActual->data );
        etStringActual->data = NULL;
    }

// String properties
    etStringActual->lengthActual = 0;
    etStringActual->lengthMax = 0;


    return etID_YES;
}

/** @ingroup grString
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etStringFree( etStringActual )

@~english
@brief Free an etString
@see standardFunctions

Frees an etString which are before created with etString_create() or etString_create_len() \n
Also the char-array itselfe will be freed !

@param[in,out] etStringActual The pointer to an etString object pointer. This will be set to NULL after the function is finished
@return If the etString object was correctly freed \n
*- @ref etID_YES
*- @ref etID_STATE_ERR_PARAMETER
*/
etID_STATE                __etStringFree( etString **p_etStringActual ){
// Parameter check
    etDebugCheckNull( p_etStringActual );
    etObjectCheckSetter( *p_etStringActual );


// Debug
    #ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p] \n", *p_etStringActual );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
    #endif

// Vars
    etString            *etStringActual = *p_etStringActual;

// release data
    if( etStringActual->data != NULL ){
        etMemoryRelease( etStringActual->data );
    }

// Relese the String itselfe
    etMemoryRelease( etStringActual );

// Return
    *p_etStringActual = NULL;
    return etID_YES;
}







#ifdef __cplusplus
}
#endif


#endif



/*
string UpToLow(string str) {
    for (int i=0;i<strlen(str.c_str());i++)
        if (str[i] >= 0x41 && str[i] <= 0x5A)
            str[i] = str[i] + 0x20;
    return str;
}
*/


