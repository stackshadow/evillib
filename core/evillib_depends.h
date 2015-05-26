/* The evillib dependencies
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

#ifndef _H_evillib_depends
#define _H_evillib_depends

	#include <errno.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <time.h>
	#include <stddef.h>
	#include <malloc.h>

	#include <pty.h> // for openpty and forkpty
	#include <utmp.h> // for login_tty

	#include <fcntl.h> // fcntl

	#include <linux/loop.h>
	#include <sys/ioctl.h>
	#include <fcntl.h>
	#include <sys/stat.h> // mkfifo
	#include <dirent.h> //opendir
	#include <mntent.h>
	#include <unistd.h>

	#include <wchar.h>
	#include <pthread.h>

//Shared memory
	#include <sys/shm.h>

// Dynamic open
	#include <dlfcn.h>

#ifndef ET_BLOCKDEVICE_OFF
	#include <blkid/blkid.h>
#else
	#define blkid_cache int
	#define blkid_dev_iterate int
	#define blkid_dev int
#endif

// etComm
	#include <netdb.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>

// Threading
	#include <pthread.h>

#endif
