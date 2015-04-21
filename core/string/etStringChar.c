/* etString_char - char extension for etString
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
	#include "memory/etMemoryList.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"
	#include "string/etStringChar.h"
#endif

/** @ingroup grString
@~english

@defgroup grStringChar etStringChar - Character Handling
@brief character handling
**/



/** @ingroup grStringChar
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@~english

@fn etStringCharGet( etStringActual, char )
@brief Get the Char-Array from the etString

This will return the internal char. You dont need to free this char.

@param[in] etStringActual The pointer to an etString object
@param[out] char The pointer to the resulting char \n
Dont free this char, it will be managed internaly by the etMemory-System \n
*- NULL if no string is aviable or etStringActual is NULL
@warning Don't free the resulting char, your application will crash !
*/
etID_STATE			__etStringCharGet( etString *etStringActual, const char **p_char ){
//ERROR CHECKING
	etCheckNull(etStringActual);
	etCheckNull(p_char);

	const char* ActualString = NULL;
	etMemoryBlockDataGet( etStringActual->data, ActualString );


	*p_char = ActualString;
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
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_YES
*/
etID_STATE			etStringCharSet( etString *etStringActual, const char *source, int maxLen ){
// Check
	etCheckNull(etStringActual);
	etCheckNull(source);

// Debug
#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p, %p]", etStringActual, source );
	etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

// Vars
	etID_STATE		ReturnValue = etID_YES;
	size_t			sourceLen = strlen(source);

// Maximum size
	if ( sourceLen > maxLen && maxLen != -1 ){
		sourceLen = maxLen;
	}

// Copy the memory
	size_t SourceSize = sourceLen * sizeof(char);
	etMemoryBlockClean( etStringActual->data );
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
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_YES
*/
etID_STATE			etStringCharAdd( etString *etStringActual, const char *source ){
// Parameter check
	etCheckNull(etStringActual);
	etCheckNull(source);


// Debug
#ifndef ET_DEBUG_OFF
	snprintf( etDebugTempMessage, etDebugTempMessageLen, "CALL [%p, %p]", etStringActual, source );
	etDebugMessage( etID_LEVEL_DETAIL, etDebugTempMessage );
#endif

//Vars
	etID_STATE		ReturnValue = etID_YES;
	size_t			actualSize = etStringActual->lengthActual * sizeof(char);
	size_t			sourceLen = strlen(source);
	size_t			sourceSize = sourceLen * sizeof(char);

// Set the memory with an offset
	ReturnValue = etMemorySetOffset( etStringActual->data, (void*)source, actualSize, sourceSize );

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
int					etStringCharFind( etString *etStringActual, const char *compareString, int offset ){
//ERROR CHECKING
	if( etStringActual == NULL ){
		etDebugState(etID_STATE_PARAMETER_MISSUSE);
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
	int 			ActualStringIndex = 0;
	char 			ActualStringChar = 0;
	int 			ActualStringLen = strlen( ActualString );
// Compare String vars
	int 			CompareStringIndex = 0;
	char 			CompareStringChar = 0;
	int 			CompareStringLen = strlen( compareString );

// Start compare
	for( ActualStringIndex = offset; ActualStringIndex <= ActualStringLen; ActualStringIndex++ ){

	// Start of the string ?
		ActualStringChar = 		ActualString	[ActualStringIndex];
		CompareStringChar = 	compareString	[CompareStringIndex];

		if( ActualStringChar == CompareStringChar ){

		// compare every char
			for( CompareStringIndex = 0; ActualStringChar == CompareStringChar; CompareStringIndex++ ){

				if( CompareStringIndex == CompareStringLen ){
					return ActualStringIndex;
				}

				ActualStringChar = 		ActualString	[CompareStringIndex+ActualStringIndex];
				CompareStringChar = 	compareString	[CompareStringIndex];
			}

		// reset if not found the whole string
			CompareStringIndex = 0;
		}



	}


	return -1;
}

