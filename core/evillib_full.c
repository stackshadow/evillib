/* The evillib core source file which includes all the seperate files
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


#define ET_INTERNAL
#include "evillib_defines.h"
#include "evillib_version.h"
#include "evillib_full.h"


// Main Parts
	#include "core/etObject.c"
	#include "core/etDebug.c"
	#include "core/etInit.c"
	#include "core/etVersion.c"

// Thirdparty things

// Memory Functions
	#include "memory/etMemoryBlock.c"
	#include "memory/etMemoryList.c"
	#include "memory/etMemory.c"
//	#include "Memory/etSharedMemory.c"
//	#include "memory/etList.c"
//	#include "Memory/etLock.c"

//
//// Basics
//	#include "develop/etObject.c"
//
//// etString
//	#include "string/etString.c"
//	#include "string/etString_char.c"
//	#include "string/etString_int.c"
//
//// System Functions
//	#include "System/etThread.c"
//	#include "System/etProcess.c"

// Block device funktions
//	#include "device/etBlkDevice.c"
//	#include "etBlkDevice/etBlkDevice_cryptsetup.c"
//	#include "etBlkDevice/etBlkDevice_loop.c"
//	#include "etBlkDevice/etBlkDevice_mount.c"







