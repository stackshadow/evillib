/*  Copyright (C) 2016 by Martin Langlotz alias stackshadow

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

#include "dbdriver/etDBDriver.h"




etID_STATE          etDBDriverConnect( etDBDriver *dbDriver ){
// check
    etDebugCheckNull(dbDriver);


// call function
    if( dbDriver->connect != NULL ){
        return dbDriver->connect( dbDriver );
    }
    
    return etID_STATE_ERR;

}


etID_STATE          etDBDriverIsConnect( etDBDriver *dbDriver ){
// check
    etDebugCheckNull(dbDriver);


// call function
    if( dbDriver->isConnected != NULL ){
        return dbDriver->isConnected( dbDriver );
    }
    
    return etID_STATE_ERR;

}




etID_STATE          etDBDriverTableAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->tableAdd != NULL ){
        return dbDriver->tableAdd( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}


etID_STATE          etDBDriverTableRemove( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->tableRemove != NULL ){
        return dbDriver->tableRemove( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}




etID_STATE          etDBDriverColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->columnAdd != NULL ){
        return dbDriver->columnAdd( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}




etID_STATE          etDBDriverDataAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataAdd != NULL ){
        return dbDriver->dataAdd( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataChange( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataChange != NULL ){
        return dbDriver->dataChange( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataGet( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// clean values
    etDBObjectValueClean( dbObject );

// call function
    if( dbDriver->dataGet != NULL ){
        return dbDriver->dataGet( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataNext( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataNext != NULL ){
        return dbDriver->dataNext( dbDriver, dbObject );
    }
    
    return etID_STATE_ERR;
}





