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


#ifndef _H_etFile
#define _H_etFile


typedef enum etFileChecksumType {
    etFileChecksum_NONE = 0,
    etFileChecksum_SHA1
} etFileChecksumType;

typedef struct etFile {
// paths
    etString                **pathArray;
    int                     pathArraySize;

// file infos
    unsigned long int       sizeInBytes;

// checksumming stuff
    etFileChecksumType      checksumType;
    etString                *checksum;

} etFile;


#define             etFileAlloc( file ) __etFileAlloc( &file )
etID_STATE          __etFileAlloc( etFile **p_etFileActual );

#define             etFileFree( file ) __etFileFree( &file )
etID_STATE          __etFileFree( etFile **p_etFileActual );


etID_STATE          etFileAppend( etFile *file, etString *path );










#endif
