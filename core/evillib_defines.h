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

#ifndef _H_evillib_defines
#define _H_evillib_defines

/** Defines
@defgroup grCoreDefines Definitions inside the evillib
@brief Preprocessor defines which is used


@defgroup grCoreDefineStates States
@ingroup grCoreDefines

@defgroup grCoreDefineLevel Levels
@ingroup grCoreDefines

@defgroup grCoreDefinesTypes Types
@ingroup grCoreDefines

@defgroup grCoreDefinesModes Modes
@ingroup grCoreDefines

@defgroup grCoreDefinesEvents Events
@ingroup grCoreDefines


*/


#define etID unsigned long


// Values
#define etLib_Buffer_Len 256
#define etMemoryListElementsMin 10

// Positions
typedef enum {
    etID_POS_LEFT =     1,
    etID_POS_MIDDL =    2,
    etID_POS_RIGHT =    3,

    etID_POS_UP =         1,
    etID_POS_DOWN =     3
}
etID_POS;

#define etBool char
typedef enum {
    etID_FALSE,
    etID_TRUE
}
etID_BOOL;

// Dimensions
typedef enum {
    etID_POS_X,
    etID_POS_Y,
    etID_WIDTH,
    etID_HEIGTH,
    etID_XALIGN,
    etID_YALIGN
}
etID_DIM;



// Message level
/**
@~english
@ingroup grCoreDefineLevel
@brief Levels

Smaller numbers means not so urgend messages. high number = high priority
*/
typedef enum etID_LEVEL {
    etID_LEVEL_ALL,
    etID_LEVEL_DETAIL,
    etID_LEVEL_DETAIL_MEM,             	/*!< Memory Debug message */
    etID_LEVEL_DETAIL_EVENT,            /*!< Event debug messages */
    etID_LEVEL_DETAIL_PROCESS,         	/*!< Process debug messages */
    etID_LEVEL_DETAIL_NET,             	/*!< Network debug process */
    etID_LEVEL_DETAIL_THREAD,         	/*!< Threading debug process */
    etID_LEVEL_DETAIL_DB,             	/*!< Database debug messages */
	etID_LEVEL_DETAIL_BUS,
	etID_LEVEL_DETAIL_APP,				/** Application Details */
    etID_LEVEL_TEST,
    etID_LEVEL_INFO,                    /*!< Info message */
    etID_LEVEL_WARNING,                 /*!< Warning message */
    etID_LEVEL_ERR,                     /*!< Error message */

    etID_LEVEL_CRITICAL,                /*!< Critical message, exit program ! */
}
etID_LEVEL;



// Types
/**
@~english
@ingroup grCoreDefinesTypes
@brief Types
*/
typedef enum {
    etID_TYPE_NOTHING = 0,
    etID_TYPE_ANYTHING, /*!< All */

    // Types
    etID_TYPE_BASIC = 10, /*!< Basics */
    etID_TYPE_DATA, /*!< Data */
    etID_TYPE_CONNECTION, /*!< Connections */
    etID_TYPE_OPTION, /*!< Options */
    etID_TYPE_OPTION_STRUCT, /*!< Options structure */
    etID_TYPE_OPERATOR, /*!< Operator ( and, or, etc. ) */
    etID_TYPE_CHECKSUM, /*!< Checksums */
    etID_TYPE_MEMORY, /*!< Memory */

    // Basic Types
    etID_TYPE_BOOL = 30, /*!< Bool */
    etID_TYPE_INT, /*!< Integer */
    etID_TYPE_UINT, /*!< Unsigned Integer */
    etID_TYPE_FLOAT, /*!< Floating-Point */
    etID_TYPE_DOUBLE, /*!< Double */
    etID_TYPE_REAL, /*!< Real */
    etID_TYPE_CHAR, /*!< one char */
    etID_TYPE_STRING, /*!< more chars ( a String ) */
    etID_TYPE_WSTRING, /*!< wide string */
    etID_TYPE_TEXT, /*!< Text */
    etID_TYPE_WTEXT, /*!< wide Text */

    // Connected basic types
    etID_TYPE_ARRAY = 50, /*!< Array */
    etID_TYPE_LIST, /*!< List */
    etID_TYPE_LIST_ENTRY, /*!< An List entry */
    etID_TYPE_POSITION, /*!< Position */
    etID_TYPE_DIMENSION, /*!< Dimension */

    // Conected complex types
    etID_TYPE_PATH = 70, /*!< A Path ( without filename ) */
    etID_TYPE_FILE, /*!< A File ( without path ) */
    etID_TYPE_TIMESTAMP, /*!< A timestamp */
    etID_TYPE_TABLE, /*!< Table */
    etID_TYPE_COLUMN, /*!< Column */

    // Connection Types
    etID_TYPE_SOCKET = 90, /*!< Socket-Connection */
    etID_TYPE_UDP, /*!< UDP-Connection */
    etID_TYPE_TCP, /*!< TCP-Connection */
    etID_TYPE_IP4,
    etID_TYPE_IP6,


    // Checksums
    etID_TYPE_MD5 = 110, /*!< MD5-Checksum */

    // Operators
    etID_TYPE_OPERATOR_START = 130, /*!< Means ( */
    etID_TYPE_OPERATOR_END, /*!< Means ) */
    etID_TYPE_AND, /*!< AND */
    etID_TYPE_OR, /*!< OR */
    etID_TYPE_NOT, /*!< NOT */
}
etID_TYPE;

#define etID_TYPE_BOOL_TEXT "BOOL"
#define etID_TYPE_INT_TEXT "INT"
#define etID_TYPE_UINT_TEXT "UINT"
#define etID_TYPE_FLOAT_TEXT "FLOAT"
#define etID_TYPE_DOUBLE_TEXT "DOUBLE"
#define etID_TYPE_CHAR_TEXT "CHAR"
#define etID_TYPE_TEXT_TEXT "TEXT"

#define etID_TYPE_TIMESTAMP_TEXT "TIMESTAMP"
#define etID_TYPE_ARRAY_TEXT "ARRAY"

#define etID_TYPE_PATH_TEXT "PATH"
#define etID_TYPE_FILE_TEXT "FILE"

// Types
/**
@~english
@ingroup grCoreDefinesModes
@brief Modes
*/
typedef enum {
    etID_MODE_NO = 0,
    etID_MODE_ADD = 1,
    etID_MODE_EDIT = 2,
    etID_MODE_REMOVE = 4,
    etID_MODE_ALL = 255
}
etID_MODE;

//Events
/**
@~english
@ingroup grCoreDefinesEvents
@brief Triggers
*/
typedef enum {
    etID_TRIGGER_ALL_OBJECTS = -3,    /*!< Run this event all the time */
    etID_TRIGGER_ALL_EVENTS = -2,    /*!< Run this event for an object all the time */
    etID_TRIGGER_NOTHING = -1,        /*!< Do nothing */
    etID_TRIGGER_TIMER,                /*!< An standartd-timer trigger */
    etID_TRIGGER_EVENT_TIMEOUT,        /*!< This trigger occures when an event runs into timeout */
    etID_TRIGGER_CLICK,                /*!< Click ( Hold mouse-button down ) */
    etID_TRIGGER_CLICKED,                /*!< When the mouse-button was released */
    etID_TRIGGER_ENUM                    /*!< Last enum number; Use this to start you custom enum */
}
etID_TRIGGER;


typedef enum {
    etID_OS_NOTHING,

// the classic ones
    etID_OS_UNIX,
    etID_OS_MAC,
    etID_OS_WIN

}
etID_OS;

//bit_fld |= (1 << n)

// Options

#define etID_OPT_NOTHING 0
#define etID_OPT_FD_BLOCKING 1
#define etID_OPT_FD_NONBLOCKING 2


#define etID_OPT_ALL 1024


#define etID_PIPE_READING 0
#define etID_PIPE_WRITING 1




#endif



// etList Macros
#define             etList_next( etListActual ) ((etListActual) ? etListActual->next : NULL)
#define             etList_prev( etListActual ) ((etListActual) ? etListActual->prev : NULL)



#if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
    #define DLL_PUBLIC
    #define DLL_LOCAL
#endif








