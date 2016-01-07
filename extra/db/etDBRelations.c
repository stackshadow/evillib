/* etDBRelation - an representation on an connection between two tables
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
	#include "evillib-extra_depends.h"
    
	#include "core/etDebug.h"
	#include "core/etObject.h"
	#include "memory/etMemoryBlock.h"
	#include "memory/etMemory.h"
	#include "string/etString.h"

	#include "etDBRelations.h"
#endif

/**
@ingroup etDB
@defgroup etDBRelation etDBRelation - an representation on an connection between two tables

The etDBRelations -object can hold multiple relation descriptions. \\
An relation description describe the connection between two different columns of two different tables.
*/

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etDBRelationAlloc( etDBRelations* dbRelations )
@~english
@brief Allocate an new etDBRelations-object
@param[out] dbRelations - An pointer to an etDBRelations-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          __etDBRelationAlloc( etDBRelations** p_dbRelations, etDebug* etDebugActual ){
// check
    etDebugReturnOnError( etDebugActual );
    etDebugCheckNull( etDebugActual, p_dbRelations );

// vars
    etMemoryBlock*      etMemoryBlockActual = NULL;
    etDBRelations*      dbRelation;
    
// Allocate
    etMemoryRequest( etMemoryBlockActual, sizeof(etDBRelations) );
    etMemoryBlockDataGet( etMemoryBlockActual, dbRelation );

// Setup fields
    dbRelation->jsonArrayRelations = json_array();
    dbRelation->foreward = 1;
    dbRelation->index = 0;

// return
    *p_dbRelations = dbRelation;
    return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Clean up ( remove all relations inside the etDBRelations-object
@param dbRelations - An pointer to an etDBRelations-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
 */
etID_STATE          etDBRelationClean( etDBRelations* dbRelations, etDebug* etDebugActual ){
// Check
	etCheckNull( dbRelations );

// Clean the "new"-values
    json_array_clear( dbRelations->jsonArrayRelations );
    dbRelations->foreward = 1;
    dbRelations->index = -1;

	return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etDBRelationFree( etDBRelations* dbRelations )
@~english
@brief Free the etDBRelations-object and sets the pointer to NULL
@param[out] dbRelations - An pointer to an etDBRelations-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          __etDBRelationFree( etDBRelations** p_dbRelations, etDebug* etDebugActual ){
// Check
	etCheckNull( p_dbRelations );

    etDBRelations*      dbRelations = *p_dbRelations;
// Clean the "new"-values
	json_decref( dbRelations->jsonArrayRelations );

    *p_dbRelations = NULL;
	return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@def etDBRelationDumps( etDBRelations* dbRelations, const char* jsonChar )
@~english
@brief Dump the etDBRelations-object

This function dumps the etDBRelations-objext as char, which represent an json-array
@param dbRelations - An pointer to an etDBRelations-object
@param[out] jsonChar - Change the pointer to an new allocated const char* which holds the json-array as string. You need to free this const char* in your application !
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
etID_STATE          __etDBRelationDumps( etDBRelations* dbRelations, const char** p_jsonChar, etDebug* etDebugActual ){
// Check
	etCheckNull( dbRelations );
	etCheckNull( p_jsonChar );

    *p_jsonChar = json_dumps( dbRelations->jsonArrayRelations, JSON_INDENT(4) | JSON_PRESERVE_ORDER );
	return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Inject an json-object into the etDBRelations-object

This function append an json-OBJECT ( not the full-array which you retrieve with etDBRelationDumps() ! ) 
to the etDBRelations-object
@param dbRelations - An pointer to an etDBRelations-object
@param jsonChar - The string which represents an json-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_INTERR
 */
etID_STATE          etDBRelationInject( etDBRelations* dbRelations, const char *jsonChar, etDebug* etDebugActual ){
// Check
	etCheckNull( dbRelations );
	etCheckNull( jsonChar );

// vars
    json_t*     jsonRelations = dbRelations->jsonArrayRelations;

// Vars
	//int 				jsonReturnCode = 0;
	json_error_t        jsonError;
	json_t*             jsonRelation;
	int                 arrayLen;

    jsonRelation = json_loads( jsonChar, JSON_INDENT(4) | JSON_PRESERVE_ORDER, &jsonError );
    arrayLen = json_array_append_new( jsonRelations, jsonRelation );
    if( arrayLen > 0 ) return etID_STATE_INTERR;
	
	return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Append an relation
This function appends an relation between two columns in two tables to the internal array\\
No checking is performed in this function, you can write everything as column-name or table name
@param dbRelations - An pointer to an etDBRelations-object
@param tableA - The origin table name
@param columnA - The column of the origin table name, which has an connection to the column of the partner-column-name
@param tableB - The partner table
@param columnB - The column of the partner table, which is connected to the column of the origin-column-name
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
 */
etID_STATE          etDBRelationAppend( etDBRelations* dbRelations, 
                        const char *tableA, const char *columnA,
                        const char *tableB, const char *columnB, 
                        etDebug* etDebugActual ){


    // Check
    etCheckNull( dbRelations );

// Vars
    int             jsonReturnCode = 0;
    json_t*         jsonRelation;
    json_t*         jsonRelations = dbRelations->jsonArrayRelations;

    jsonRelation = json_object();
    jsonReturnCode |= json_object_set_new( jsonRelation, "ver", json_integer(1) );
    jsonReturnCode |= json_object_set_new( jsonRelation, "tableA", json_string(tableA) );
    jsonReturnCode |= json_object_set_new( jsonRelation, "columnA", json_string(columnA) );
    jsonReturnCode |= json_object_set_new( jsonRelation, "tableB", json_string(tableB) );
    jsonReturnCode |= json_object_set_new( jsonRelation, "columnB", json_string(columnB) );

    json_array_append_new( jsonRelations, jsonRelation );


    // return
    return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Get the amount of relations stored inside the etDBRelations-object with etDBRelationAppend()
@param dbRelations - An pointer to an etDBRelations-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*/
int                 etDBRelationCount( etDBRelations* dbRelations, etDebug* etDebugActual ){
// Check
    if( dbRelations == NULL){
        etDebugState( etID_STATE_PARAMETER_MISSUSE );
        return -1;
    }


// Vars
    json_t*         jsonRelations = dbRelations->jsonArrayRelations;

    return json_array_size(jsonRelations);
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>
@~english
@brief Reset the internal counter

Normaly with functions like etDBRelationFindNext() or etDBRelationFindNext2() you search relativ until etID_NO is returned.
But if you would like to search from the beginning again, cou can call etDBRelationsIndexReset()
@param dbRelations - An pointer to an etDBRelations-object
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
**/
etID_STATE          etDBRelationsIndexReset( etDBRelations* dbRelations, etDebug* etDebugActual ){
// Check
	etCheckNull( dbRelations );

// vars
    dbRelations->foreward = 1;
    dbRelations->index = 0;

    return etID_YES;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@def etDBRelationFindNext( etDBRelations* dbRelations, const char *tableSearch, 
                        const char *tableColumn, const char *relatedTable, const char *relatedColumn )

@~english
@brief Find the next relation of an table

@param dbRelations - An pointer to an etDBRelations-object
@param tableSearch - The table name, we try to find inside the dbRelations
@param[out] tableColumn - Returns the name of the origin column of the tableSearch, DON'T FREE THIS POINTER
@param[out] relatedTable - Returns the name of the related table, DON'T FREE THIS POINTER
@param[out] relatedColumn - Returns the name of the related column, DON'T FREE THIS POINTER
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_ERROR_INTERNAL
*/
etID_STATE          __etDBRelationFindNext( etDBRelations* dbRelations, const char *tableSearch, 
                        const char **p_tableColumn, const char **p_relatedTable, const char **p_relatedColumn, 
                        etDebug* etDebugActual ){

// Check
    etCheckNull( dbRelations );
    etCheckNull( dbRelations->jsonArrayRelations );
    etCheckNull( tableSearch );
    etCheckNull( p_tableColumn );
    etCheckNull( p_relatedTable );
    etCheckNull( p_relatedColumn );

// Vars
    json_t*                         jsonTemp = NULL;
    json_t*                         jsonRelations = dbRelations->jsonArrayRelations;
    json_t*                         jsonRelation = NULL;
    int                             relationCount = json_array_size( jsonRelations );
    const char*                     tempTable = NULL;

// clear out values
    *p_tableColumn = NULL;
    *p_relatedTable = NULL;
    *p_relatedColumn = NULL;



    for( ; dbRelations->index < relationCount; dbRelations->index++ ){

    // get the single relation
        if( (jsonRelation = json_array_get( jsonRelations, dbRelations->index )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }


        if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // origin column
            if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_tableColumn = json_string_value(jsonTemp);

        // partner table
            if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_relatedTable = json_string_value(jsonTemp);

        // partner column
            if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_relatedColumn = json_string_value(jsonTemp);

            dbRelations->index++;
            return etID_YES;
        }


        if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);        
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // origin column
            if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_tableColumn = json_string_value(jsonTemp);

        // partner table
            if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_relatedTable = json_string_value(jsonTemp);

        // partner column
            if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            *p_relatedColumn = json_string_value(jsonTemp);            

            dbRelations->index++;
            return etID_YES;
        }
    }


    return etID_NO;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@def etDBRelationFindNextColumn( etDBRelations* dbRelations, const char *tableSearch, 
                        const char *columnSearch, const char *relatedTable, const char *relatedColumn )

@~english
@brief Find the next relation of an table with column

@param dbRelations - An pointer to an etDBRelations-object
@param tableSearch - The table name, we try to find inside the dbRelations
@param columnSearch - The name of the origin column of the tableSearch, we try to find inside the dbRelations
@param[out] relatedTable - Returns the name of the related table, DON'T FREE THIS POINTER
@param[out] relatedColumn - Returns the name of the related column, DON'T FREE THIS POINTER
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_ERROR_INTERNAL
*/
etID_STATE          __etDBRelationFindNextColumn( etDBRelations* dbRelations, const char *tableSearch, const char *columnSearch,
                        const char **p_relatedTable, const char **p_relatedColumn, 
                        etDebug* etDebugActual ){

// Check
    etCheckNull( dbRelations );
    etCheckNull( dbRelations->jsonArrayRelations );
    etCheckNull( tableSearch );
    etCheckNull( columnSearch );
    etCheckNull( p_relatedTable );
    etCheckNull( p_relatedColumn );

// Vars
    json_t*                         jsonTemp = NULL;
    json_t*                         jsonRelations = dbRelations->jsonArrayRelations;
    json_t*                         jsonRelation = NULL;
    int                             relationCount = json_array_size( jsonRelations );
    const char*                     tempTable = NULL;

// clear out values
    *p_relatedTable = NULL;
    *p_relatedColumn = NULL;



    for( ; dbRelations->index < relationCount; dbRelations->index++ ){

    // get the single relation
        if( (jsonRelation = json_array_get( jsonRelations, dbRelations->index )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }


        if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // origin column
            if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            tempTable = json_string_value(jsonTemp);
            if( strncmp(tempTable,columnSearch,strlen(columnSearch)) == 0 ){

            // partner table
                if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedTable = json_string_value(jsonTemp);

            // partner column
                if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedColumn = json_string_value(jsonTemp);

            }

            dbRelations->index++;
            return etID_YES;
        }


        if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);        
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // origin column
            if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            tempTable = json_string_value(jsonTemp);
            if( strncmp(tempTable,columnSearch,strlen(columnSearch)) == 0 ){

            // partner table
                if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedTable = json_string_value(jsonTemp);

            // partner column
                if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedColumn = json_string_value(jsonTemp);
                
            }

            dbRelations->index++;
            return etID_YES;
        }
    }


    return etID_NO;
}

/** @ingroup etDBRelation
@author Martin Langlotz alias stackshadow <stackshadow@evilbrain.de>

@def etDBRelationFindNextTable( etDBRelations* dbRelations, const char *tableSearch, 
                        const char *columnSearch, const char *relatedTable, const char *relatedColumn )

@~english
@brief Find the next relation of an table with column

@param dbRelations - An pointer to an etDBRelations-object
@param tableSearch - The table name, we try to find inside the dbRelations
@param columnSearch - The name of the origin column of the tableSearch, we try to find inside the dbRelations
@param[out] relatedTable - Returns the name of the related table, DON'T FREE THIS POINTER
@param[out] relatedColumn - Returns the name of the related column, DON'T FREE THIS POINTER
@return 
*- @ref etID_YES
*- @ref etID_STATE_PARAMETER_MISSUSE
*- @ref etID_STATE_ERROR_INTERNAL
*/
etID_STATE          __etDBRelationFindNextTable( etDBRelations* dbRelations, const char *tableSearch, const char *relatedTableSearch,
                        const char **p_tableColumn,  const char **p_relatedColumn, 
                        etDebug* etDebugActual ){

// Check
    etCheckNull( dbRelations );
    etCheckNull( dbRelations->jsonArrayRelations );
    etCheckNull( tableSearch );
    etCheckNull( relatedTableSearch );
    etCheckNull( p_tableColumn );
    etCheckNull( p_relatedColumn );

// Vars
    json_t*                         jsonTemp = NULL;
    json_t*                         jsonRelations = dbRelations->jsonArrayRelations;
    json_t*                         jsonRelation = NULL;
    int                             relationCount = json_array_size( jsonRelations );
    const char*                     tempTable = NULL;

// clear out values
    *p_tableColumn = NULL;
    *p_relatedColumn = NULL;



    for( ; dbRelations->index < relationCount; dbRelations->index++ ){

    // get the single relation
        if( (jsonRelation = json_array_get( jsonRelations, dbRelations->index )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }


    // compare origin table
        if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // compare partner table
            if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            tempTable = json_string_value(jsonTemp);
            if( strncmp(tempTable,relatedTableSearch,strlen(relatedTableSearch)) == 0 ){

            // return origin column
                if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_tableColumn = json_string_value(jsonTemp);

            // return partner column
                if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedColumn = json_string_value(jsonTemp);

                dbRelations->index++;
                return etID_YES;
            }
        }



    // compare partner table
        if( (jsonTemp = json_object_get( jsonRelation, "tableB" )) == NULL ){ 
            return etDebugState( etID_STATE_ERROR_INTERNAL );
        }
        tempTable = json_string_value(jsonTemp);
        if( strncmp(tempTable,tableSearch,strlen(tableSearch)) == 0 ){

        // compare origin table
            if( (jsonTemp = json_object_get( jsonRelation, "tableA" )) == NULL ){ 
                return etDebugState( etID_STATE_ERROR_INTERNAL );
            }
            tempTable = json_string_value(jsonTemp);
            if( strncmp(tempTable,relatedTableSearch,strlen(relatedTableSearch)) == 0 ){

            // return origin column
                if( (jsonTemp = json_object_get( jsonRelation, "columnB" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_tableColumn = json_string_value(jsonTemp);

            // return partner column
                if( (jsonTemp = json_object_get( jsonRelation, "columnA" )) == NULL ){ 
                    return etDebugState( etID_STATE_ERROR_INTERNAL );
                }
                *p_relatedColumn = json_string_value(jsonTemp);

                dbRelations->index++;
                return etID_YES;
            }

        }
    }


    return etID_NO;
}








