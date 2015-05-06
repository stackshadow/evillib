#!/bin/make
#The evillib build-system
#	Copyright (C) 2015 by Martin Langlotz
#
#	This file is part of evillib.
#
#	evillib is free software: you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	evillib is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public License
#	along with evillib.  If not, see <http://www.gnu.org/licenses/>.

include MakefileBase.make




# librarys
CLIBS =-lpthread
CLIBS+=-lc
CLIBS+=-lblkid
CLIBS+=-ldl

CFLAGS+=-rdynamic

# Main Parts
sources += core/etObject.c
sources += core/etDebug.c
sources += core/etInit.c
sources += core/etVersion.c

# Memory Functions
sources += memory/etMemoryBlock.c
sources += memory/etMemoryList.c
sources += memory/etMemory.c

# etString
sources += string/etString.c
sources += string/etStringChar.c

# System Functions
sources += system/etThread.c

# Network functions
sources += net/etComm.c
sources += net/etCommTCP.c

# Extra struff ( will be moved ! )
#sources += ../extra/etjDB.c
#CLIBS+=-ljansson

sourcesFull=$(addprefix $(sourcePath)/,$(sources))

headers=$(sources:.c=.h)

objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/core/,$(objects))







#CoreSourceFiles+=
CoreLibraryShared=libevillib.$(Version).so
CoreLibraryStatic=libevillib.$(Version).a

.EXPORT_ALL_VARIABLES:

.DEFAULT:
	@echo "$@: This target dont exist in Makefile"

#################################### Standard ####################################
all: evillib-core-dev evillib-core

help:
	@echo -n "$(CComment)"
	@echo "##################### Core library #####################"
	@echo "$(MAKE) evillib-core: Build evillib-core library ( in $(libDir)/libevillib.so )"
	@echo "$(buildPath)/evillib.$(Version).c: Create the single-source-file"
	@echo "$(MAKE) evillib-core-clean: Clean the build directory of core"
	@echo "$(MAKE) evillib-core-dev: Build development files ( in $(includeDir)/evillib.h )"
	@echo "$(MAKE) evillib-app: create the evillib app"
	@echo "$(MAKE) evillib-app-clean"
	@echo "$(MAKE) install: install everything"
	@echo -n "$(CNormal)"

clean: evillib-core-dev-clean evillib-core-clean evillib-app-clean
	@find .           -name "*.o" -print -delete
	@find $(buildPath) -name "*.o" -print -delete


folders:

install: core

#################################### Development / Headers ####################################
evillib-core-dev: $(includeDir)/evillib.h
evillib-core-dev-clean:
	@$(RM) evillib_version.h
	@$(RM) $(buildPath)/evillib.$(Version).h
	@$(RM) $(includeDir)/evillib/evillib.$(Version).h
	@$(RM) $(includeDir)/evillib.h


$(buildPath):
	mkdir -p $@

evillib_version.h:
	@echo "${CCommand}make $@ ${CNormal}"
	@echo "// Evillib - Version" > $@
	@echo "/**	@defgroup grCoreDefineVersion Versions" >> $@
	@echo "@ingroup grCoreDefines" >> $@
	@echo "*/" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "The major version of the evillib */" >> $@
	@echo "#define ET_VERSION_MAJOR $(VerMajor)" >> $@
	@echo "" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "If this is changed, a recompilation is needed */" >> $@
	@echo "#define ET_VERSION_MINOR $(VerMinor)" >> $@
	@echo "" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "If this is changed, no recompile needed */" >> $@
	@echo "#define ET_VERSION_PATCH $(VerPatch)" >> $@
	
	@echo "" >> $@
	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "The versionsstring */" >> $@
	@echo "#define ET_VERSIONSTRING \"$(VerMajor).$(VerMinor)-$(VerPatch)\"" >> $@
	
	@echo "" >> $@

$(buildPath)/evillib.$(Version).h: evillib_version.h $(buildPath)
	@echo "${CCommand}make $@ ${CNormal}"
	cat evillib_start.h > $@
	cat evillib_depends.h >> $@
	cat evillib_version.h >> $@
	cat evillib_defines.h >> $@
	cat $(headers) >> $@
	cat evillib_end.h >> $@


# Output directory
$(includeDir)/evillib.h: $(buildPath)/evillib.$(Version).h
	@mkdir -p $(includeDir)/evillib/
	@$(CP) $(buildPath)/evillib.$(Version).h $(includeDir)/evillib/	
	@$(LN) evillib/evillib.$(Version).h $@


#################################### Library ####################################
evillib-core: $(libDir)/libevillib.so $(shareDir)/pkgconfig/evillib.pc
evillib-core-static: $(libDir)/libevillib.a
evillib-core-clean:
	@$(RM) $(buildPath)/evillib.$(Version).c
	@$(RM) $(buildPath)/libevillib.o
	@$(RM) $(buildPath)/$(CoreLibraryShared)
	@$(RM) $(libDir)/evillib/$(CoreLibraryShared)
	@$(RM) $(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(RM) $(libDir)/libevillib.so


$(buildPath)/evillib.$(Version).c: evillib_version.h $(buildPath)/evillib.$(Version).h
	@echo "${CCommand}make $@ ${CNormal}"
	@echo "$(PWD)"
	cat $(buildPath)/evillib.$(Version).h > $@
	cat evillib_start.c >> $@
	cat $(sourcesFull) >> $@
	cat evillib_end.c >> $@

$(buildPath)/libevillib.o: $(buildPath)/evillib.$(Version).c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) $< -o $@

$(buildPath)/$(CoreLibraryShared): $(buildPath)/libevillib.o
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -shared -Wl,-soname,libevillib.so \
	$(buildPath)/libevillib.o \
	$(CLIBS) \
	-o $@

# Output directory
$(libDir)/evillib/$(CoreLibraryShared): $(buildPath)/$(CoreLibraryShared)
	@$(MKDIR) $(libDir)/evillib
	@$(CP) $(buildPath)/$(CoreLibraryShared) $@

$(libDir)/libevillib.$(VerMajor).$(VerMinor).so: $(libDir)/evillib/$(CoreLibraryShared)
	@$(LN) evillib/$(CoreLibraryShared) $@

$(libDir)/libevillib.so: $(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(LN) evillib/$(CoreLibraryShared) $@

# pkgconf
$(shareDir)/pkgconfig/evillib.pc:
	@$(MKDIR) $(shareDir)/pkgconfig
	@echo "Name: evillib" > $@
	@echo "Version: $(Version)" >> $@
	@echo "Description: evillib" >> $@
	@echo "Requires:" >> $@
	@echo "Libs: -L/usr/lib/evillib -levillib" >> $@
	@echo "Cflags: -I/usr/include/evillib/" >> $@



#################################### docu target ####################################
docu: $(buildPath)/$(CoreHeader)


#################################### Targets for development ####################################
$(buildPath)/core/%.o: $(sourcePath)/%.c
	@mkdir -v -p $$(dirname $@)
	$(CC) -fPIC -I. -Wall -DET_SINGLEMODULE -c $(CFLAGS) $< -o $@

# this holds apicheckObjects
#include $(sourceCoreDir)/app/apichecks/Makefile

#apicheckObjects += $(buildPath)/app/evillib.o

#evillib-app: $(buildDir)/evillib 
#$(buildDir)/evillib: $(sourceCoreDir)/evillib_version.h $(apicheckObjects) $(CoreDeployDir)
#	gcc -L $(buildPath) $(apicheckObjects) -levillib.00.14-04 -o $(buildDir)/evillib

#evillib-app-single: $(buildDir)/evillib-single
#$(buildDir)/evillib-single: $(SingleObjects)
#	gcc $(apicheckObjects) $(SingleObjects) -o $(buildDir)/evillib

output:
	@echo $(objects)
	@echo $(CoreAppObjectsFull)



