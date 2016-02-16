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

#ifndef _H_etIDState
#define _H_etIDState

#ifdef __cplusplus
extern "C" {
#endif


// States
/**
@~english
@ingroup grCoreDefineStates
@brief States

The states of the evillib is divided into two parts:
 * If the state is >0 its not an error, only a state
 * If the state is <0 an error occure
 * 0 means state not changed since last call
*/
typedef enum etID_STATE {

// ######################################### normal states #########################################
    etID_STATE_NOTHING = 0,                     /*!< No state */
    etID_NO = 0,                                /*!< No */
    etID_YES = 1,                               /*!< Everything okay */
    etID_OK = 1,                                /*!< Everything okay */

// Data/Memory
    etID_STATE_LOCKED,                          /*!< Data is locked */
    etID_STATE_USED,                            /*!< Data already in use */
    etID_STATE_NODATA,                          /*!< No data aviable */

    etID_STATE_READY,                           /*!< Ready */
    etID_STATE_PENDING,                         /*!< Under action */
    etID_STATE_AUTO,

// Disable enable states
    etID_STATE_DISABLED,                        /*!< Disabled */
    etID_STATE_ENABLED,                         /*!< Enabled */

// New, change, delete
    etID_STATE_NEW,                             /*!< New */
    etID_STATE_CHANGED,                         /*!< changed */
    etID_STATE_DELETED,                         /*!< deleted */

// Run, wait, end
    etID_STATE_RUN,                             /*!< Running */
    etID_STATE_WAIT,                            /*!< waiting */
    etID_STATE_BREAK,                           /*!< break */
    etID_STATE_END,                             /*!< ended */

// Data
    etID_STATE_DATA_AVIABLE,

// Compare
    etID_STATE_EQ,
    etID_STATE_GREATER,
    etID_STATE_SMALLER,


// ######################################### warning states #########################################
// is an object at this state, you can write to it but can not do something
    etID_STATE_WARN = 50,

// Sequencial error that means, that you need to run another function BEFORE use if this function
    etID_STATE_WARN_SEQERR,

// Connected / Disconnected
    etID_STATE_WARN_DISCONNECTED,                   /*!< Is disconnected ( this can be good or bad, so this is only an information message ) */

// Times
    etID_STATE_WARN_TIMEOUT,                        /*!< An timeout occures ( etDebug WARNING ) */

// Devices
    etID_STATE_WARN_DEVICE_NOT_OPEN,                /*!< Device could not be opened */
    etID_STATE_WARN_LOOP_NOT_OPEN,                  /*!< Loop Device could not be opened */
    etID_STATE_WARN_LOOP_NOT_CLEAN,                 /*!< Loop Device could not be cleaned */

// Files
    etID_STATE_WARN_FILE_NO_ACCESS,                 /*!< Access to file denied */
    etID_STATE_WARN_FILE_NOT_OPEN,                  /*!< File could not be opened */
    etID_STATE_WARN_FILE_NOT_CREATED,               /*!< File could not be created */

// Function errors
    etID_STATE_WARN_PARAMETER,                      /*!< Wrong parameter was passed to the function */

    etID_STATE_WARN_INTERR,

// ######################################### error states #########################################
// object is broken, we only can init it and try it again

    etID_STATE_ERR = 100,

    etID_STATE_ERR_NOTINLIB,                       /*!< Not compiled into this library; Function do nothing */
    etID_STATE_ERR_INTERR,                         /*!< Internal error */

    etID_STATE_ERR_PARAMETER,                      /*!< Critical parameter is missing */

// ######################################### critical states #########################################
    etID_STATE_CRIT = 200,
    etID_STATE_CRIT_NOMEMORY                       /*!< No memory aviable */

}
etID_STATE;


#ifdef __cplusplus
}
#endif





#endif

