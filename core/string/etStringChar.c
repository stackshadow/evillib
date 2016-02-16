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

#include "core/etDebug.h"
#include "core/etObject.h"
#include "memory/etMemoryBlock.h"
#include "memory/etMemory.h"
#include "string/etStringChar.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @ingroup grString
@~english

@defgroup grStringChar etStringChar - Character Handling
@brief character handling

Don't forget the \0 on set or add of an char-array !

@todo convert etString to an etObject !
**/



/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etStringCharGet( etString *etStringActual, const char *char );

@~english

@brief Get the Char-Array from the etString

This will return the internal char. You dont need to free this char.

@param[in] etStringActual The pointer to an etString object
@param[out] char The pointer to the resulting char \n
Dont free this char, it will be managed internaly by the etMemory-System \n
*- NULL if no string is aviable or etStringActual is NULL
@warning Don't free the resulting char, your application will crash !
*/
etID_STATE              __etStringCharGet( etString *etStringActual, const char **p_char ){
//ERROR CHECKING
    etObjectCheckGetter(etStringActual);
    etDebugCheckNull(p_char);

    *p_char = etStringActual->data;
    return etID_YES;
}

/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@fn etID_STATE etStringCharCopy( etString *etStringActual, char target, int maxLen )

@~english

@brief Copy the the etString to an char array

@snippet etStringChar.c etStringCharCopy


@param[in] etStringActual The pointer to an etString object
@param[out] target The pointer to an char array
@param[in] maxLen The maximum lenght of chars of the provided array
*/
etID_STATE              __etStringCharCopy( etString *etStringActual, char *target, int maxLen ){
//ERROR CHECKING
    etObjectCheckGetter(etStringActual);
    etDebugCheckNull(target);
    
    unsigned int lengthToCopy = etStringActual->lengthActual;
    
    if( lengthToCopy > maxLen ){
        lengthToCopy = maxLen;
    }
    
// copy memory
    memcpy( target, etStringActual->data, lengthToCopy * sizeof(char) );
    
    
    return etID_YES;
}

/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english

@brief Set the etString from an Character array

Set the etString from a char. \n
It doesnt matter if the character-array is in UTF8 or PLAIN-Text.

@param[in] etStringActual The pointer to an etString object pointer
@param[in] source The Char-Array from where to set the etString
@param[in] maxLen The maximal amount of chars to copy \n
If the source-char is shorter than maxLen, the len of source-char is used
@return If the char was correctly set inside the etString \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE              etStringCharSet( etString *etStringActual, const char *source, int maxLen ){
// Check
    etObjectCheckSetter(etStringActual);
    etDebugCheckNull(source);

// Debug
#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p, %p]", etStringActual, source );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

// Vars
    etID_STATE        ReturnValue = etID_YES;
    size_t            sourceLen = strlen(source);

// Maximum size
    if ( sourceLen > maxLen && maxLen != -1 ){
        sourceLen = maxLen;
    }

// Copy the memory
    size_t SourceSize = sourceLen * sizeof(char);
    etMemoryClean( etStringActual->data );
    ReturnValue = etMemorySet( etStringActual->data, (void*)source, SourceSize );

// save length
    etStringActual->lengthActual = sourceLen;

// Return
    if( ReturnValue != etID_YES ) return ReturnValue;
    return etID_YES;
}

/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english

@brief Add an char to the end of an existing etString

@param[in] etStringActual The pointer to an etString object pointer
@param[in] source The Char which is added to the end of the etString.
@return If the char was added to the etString \n
*- @ref etID_STATE_ERR_PARAMETER
*- @ref etID_YES
*/
etID_STATE              etStringCharAdd( etString *etStringActual, const char *source ){
// Parameter check
    etDebugCheckNull(etStringActual);
    etDebugCheckNull(source);


// Debug
#ifndef ET_DEBUG_OFF
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p, %p]", etStringActual, source );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

//Vars
    etID_STATE        ReturnValue = etID_YES;
    size_t            actualSize = etStringActual->lengthActual * sizeof(char);
    size_t            sourceLen = strlen(source);
    size_t            sourceSize = sourceLen * sizeof(char);


// Set the memory with an offset
    ReturnValue = etMemorySetOffset( etStringActual->data, (void*)source, actualSize, sourceSize );


#ifndef ET_DEBUG_OFF
    const char *tempString;
    etStringCharGet( etStringActual, tempString );
    snprintf( etDebugTempMessage, etDebugTempMessageLen, "New string: [%s]", tempString );
    etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

// save length
    etStringActual->lengthActual = etStringActual->lengthActual + sourceLen;

    if( ReturnValue != etID_YES ) return ReturnValue;
    return etID_YES;
}

/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english

@brief Find an Char-Array inside the etString
The search start from "Offset"

@param[in] etStringActual The pointer to an etString object
@param[in] compareString The Char-Array which will be compared with the etString
@param[in] offset From where we start the searching inside the etString
@return Return where the Char-Array was found inside the etString \n
*- >0 Start-Position where the Char-Array is located. 0 is the first char !
*- -1 An error occure, or CompareString is not included inside the etString
*/
int                     etStringCharFind( etString *etStringActual, const char *compareString, int offset ){
//ERROR CHECKING
    if( etStringActual == NULL ){
        etDebugState(etID_STATE_ERR_PARAMETER);
        return -1;
    }
    if( etStringActual->data == NULL ){
        etDebugState(etID_STATE_NODATA);
        return -1;
    }

// Get char
    const char* ActualString = NULL;
    
    
    etStringCharGet( etStringActual, ActualString );
    if( ActualString == NULL ) return -1;

// Actual String vars
    int             ActualStringIndex = 0;
    char             ActualStringChar = 0;
    int             ActualStringLen = strlen( ActualString );
// Compare String vars
    int             CompareStringIndex = 0;
    char             CompareStringChar = 0;
    int             CompareStringLen = strlen( compareString );

// Start compare
    for( ActualStringIndex = offset; ActualStringIndex <= ActualStringLen; ActualStringIndex++ ){

    // Start of the string ?
        ActualStringChar =         ActualString    [ActualStringIndex];
        CompareStringChar =     compareString    [CompareStringIndex];

        if( ActualStringChar == CompareStringChar ){

        // compare every char
            for( CompareStringIndex = 0; ActualStringChar == CompareStringChar; CompareStringIndex++ ){

                if( CompareStringIndex == CompareStringLen ){
                    return ActualStringIndex;
                }

                ActualStringChar =         ActualString    [CompareStringIndex+ActualStringIndex];
                CompareStringChar =     compareString    [CompareStringIndex];
            }

        // reset if not found the whole string
            CompareStringIndex = 0;
        }



    }


    return -1;
}

#ifdef __cplusplus
}
#endif
