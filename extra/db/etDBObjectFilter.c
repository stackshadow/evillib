

#include "db/etDBObjectFilter.h"

etID_STATE      etDBObjectFilterClear( etDBObject *dbObject ){
// check
    etDebugCheckNull( dbObject );

// vars
    json_t      *jsonFilterArray;
    json_t      *jsonFilter;


// reset the index stuff
    etDBObjectIterationReset( dbObject );

// get the array which holds the filter
    jsonFilterArray = json_object_get( dbObject->jsonRootObject, "filter" );
    if( jsonFilterArray == NULL ) return etID_YES;

// create new array
    jsonFilterArray = json_array();
    if( json_object_set_new( dbObject->jsonRootObject, "filter", jsonFilterArray ) != 0 ){
        return etID_STATE_ERR;
    }


    return etID_YES;
}


etID_STATE      etDBObjectFilterAdd( etDBObject *dbObject, int filterGroup, etDBFILTER_OP filterOperation, const char *filterColumn, etDBFILTER_TYPE filterType, const char *filterString ){
// check
    etDebugCheckNull( dbObject );

// vars
    json_t      *jsonFilterArray;
    json_t      *jsonFilter;


// reset the index stuff
    etDBObjectIterationReset( dbObject );

// get the array which holds the filter
    jsonFilterArray = json_object_get( dbObject->jsonRootObject, "filter" );
    if( jsonFilterArray == NULL ){
    // create a new table
        jsonFilterArray = json_array();
        if( json_object_set_new( dbObject->jsonRootObject, "filter", jsonFilterArray ) != 0 ){
            return etID_STATE_ERR;
        }
    }

// create json-object
    jsonFilter = json_object();
    json_object_set_new( jsonFilter, "group", json_integer(filterGroup) );
    json_object_set_new( jsonFilter, "operation", json_integer(filterOperation) );
    json_object_set_new( jsonFilter, "column", json_string(filterColumn) );
    json_object_set_new( jsonFilter, "type", json_integer(filterType) );
    json_object_set_new( jsonFilter, "string", json_string(filterString) );

    json_array_append_new( jsonFilterArray, jsonFilter );



}


etID_STATE      etDBObjectFilterNext( etDBObject *dbObject, int *filterGroup, etDBFILTER_OP *filterOperation, const char **filterColumn, etDBFILTER_TYPE *filterType, const char **filterString ){
// check
    etDebugCheckNull( dbObject );

// vars
    json_t      *jsonFilterArray;
    json_t      *jsonFilter;

// get the array which holds the filter
    if( dbObject->jsonObjectToIterate == NULL ){
        dbObject->jsonObjectToIterate = json_object_get( dbObject->jsonRootObject, "filter" );
        if( dbObject->jsonObjectToIterate == NULL ){
            return etID_STATE_ERR;
        }
    }

// check the size of the array
    if( dbObject->jsonIteratorIndex >= json_array_size(dbObject->jsonObjectToIterate) ){
        return etID_STATE_NODATA;
    }

// get the filter from the array
    jsonFilter = json_array_get( dbObject->jsonObjectToIterate, dbObject->jsonIteratorIndex );

// return values
    json_t      *jsonValue = NULL;

    jsonValue = json_object_get( jsonFilter, "group" );
    if( jsonValue != NULL ){
        *filterGroup = json_integer_value(jsonValue);
    } else {
        *filterGroup = 0;
    }

    jsonValue = json_object_get( jsonFilter, "operation" );
    if( jsonValue != NULL ){
        *filterOperation = json_integer_value(jsonValue);
    } else {
        *filterOperation = etDBFILTER_OP_NOTHING;
    }

    jsonValue = json_object_get( jsonFilter, "column" );
    if( jsonValue != NULL ){
        *filterColumn = json_string_value(jsonValue);
    } else {
        *filterColumn = '\0';
    }
    
    jsonValue = json_object_get( jsonFilter, "type" );
    if( jsonValue != NULL ){
        *filterType = json_integer_value(jsonValue);
    } else {
        *filterType = etDBFILTER_TYPE_NOTHING;
    }

    jsonValue = json_object_get( jsonFilter, "string" );
    if( jsonValue != NULL ){
        *filterString = json_string_value(jsonValue);
    } else {
        *filterString = '\0';
    }
    

// iterate the index
    dbObject->jsonIteratorIndex = dbObject->jsonIteratorIndex + 1;

// return no data
    return etID_YES;
}







