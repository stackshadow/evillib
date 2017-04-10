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


etID_STATE          __etDBDriverAlloc( etDBDriver** p_dbDriver ){
// check
    etDebugCheckNull(p_dbDriver);

// vars
    etDBDriver*         dbDriver = NULL;

// allocate memory
    etMemoryAlloc( dbDriver, sizeof(etDBDriver) );

// set to none driver
    dbDriver->type = etDB_DRIVER_NONE;

// return
    *p_dbDriver = dbDriver;
    return etID_YES;
}


etID_STATE          __etDBDriverFree( etDBDriver** p_dbDriver ){
// check
    etDebugCheckNull(p_dbDriver);

// release memory
    __etMemoryRelease( (void**)p_dbDriver );

    return etID_YES;
}






etID_STATE          etDBDriverConnect( etDBDriver* dbDriver ){
// check
    etDebugCheckNull(dbDriver);


// call function
    if( dbDriver->connect != NULL ){
        return dbDriver->connect( dbDriver );
    }

    return etID_STATE_ERR;

}


etID_STATE          etDBDriverIsConnect( etDBDriver* dbDriver ){
// check
    etDebugCheckNull(dbDriver);


// call function
    if( dbDriver->isConnected != NULL ){
        return dbDriver->isConnected( dbDriver );
    }

    return etID_STATE_ERR;

}


etID_STATE          etDBDriverDisConnect( etDBDriver* dbDriver ){
// check
    etDebugCheckNull(dbDriver);


// call function
    if( dbDriver->disconnect != NULL ){
        return dbDriver->disconnect( dbDriver );
    }

    return etID_STATE_ERR;

}




etID_STATE          etDBDriverTableAdd( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbTable);

// call function
    if( dbDriver->tableAdd != NULL ){
        return dbDriver->tableAdd( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverTableRemove( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbTable);

// call function
    if( dbDriver->tableRemove != NULL ){
        return dbDriver->tableRemove( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverTableExists( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbTable);

// call function
    if( dbDriver->tableExists != NULL ){
        return dbDriver->tableExists( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}
/*

etID_STATE          etDBDriverColumnAdd( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->columnAdd != NULL ){
        return dbDriver->columnAdd( dbDriver, dbObject );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverColumnRemove( etDBDriver *dbDriver, etDBObject *dbObject ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->columnRemove != NULL ){
        return dbDriver->columnRemove( dbDriver, dbObject );
    }

    return etID_STATE_ERR;
}
*/



etID_STATE          etDBDriverDataAdd( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataAdd != NULL ){
        return dbDriver->dataAdd( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataChange( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataChange != NULL ){
        return dbDriver->dataChange( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataRemove( etDBDriver *dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbDriver);

// call function
    if( dbDriver->dataRemove != NULL ){
        return dbDriver->dataRemove( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataGet( etDBDriver* dbDriver, etDBTable* dbTable, etDBFilter* dbFilter ){
// check
    etDebugCheckNull(dbDriver);

// clean values
    etDBColumnCleanAllValues( dbTable );

// call function
    if( dbDriver->dataGet != NULL ){
        return dbDriver->dataGet( dbDriver, dbTable, dbFilter );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataGetWithLimits( etDBDriver* dbDriver, etDBTable* dbTable, int start, int amount, etDBFilter* dbFilter ){
// check
    etDebugCheckNull(dbDriver);

// clean values
    etDBColumnCleanAllValues( dbTable );

// call function
    if( dbDriver->dataGetWithLimit != NULL ){
        return dbDriver->dataGetWithLimit( dbDriver, dbTable, start, amount, dbFilter );
    }

    return etID_STATE_ERR;
}


etID_STATE          etDBDriverDataNext( etDBDriver* dbDriver, etDBTable* dbTable ){
// check
    etDebugCheckNull(dbDriver);

// clean values
    etDBColumnCleanAllValues( dbTable );

// call function
    if( dbDriver->dataNext != NULL ){
        return dbDriver->dataNext( dbDriver, dbTable );
    }

    return etID_STATE_ERR;
}









