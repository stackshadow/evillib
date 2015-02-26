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


# Colours
CNormal=\033[00m
CCommand=\033[1;35m
CComment=\033[36m
COK=\033[32m

# Compiler
CC = ${CROSSCOMPILE}gcc
CXX = ${CROSSCOMPILE}g++
AR = ${CROSSCOMPILE}ar
RANLIB = ${CROSSCOMPILE}ranlib
RM = rm -v -f
CP = cp -v -f
LN = ln -v -f -s
MKDIR = mkdir -p -v
RMDIR = rmdir -v

# Directorys
sourcePath=$(PWD)
buildPath=/tmp/.build
prefix=$(buildPath)/target

includeDir=$(prefix)/include
binDir=$(prefix)/bin
libDir=$(prefix)/lib
docDir=$(prefix)/share/doc/evillib/${Version}


# Version of evillib
VerMajor=00
VerMinor=14
VerPatch=04
Version=$(VerMajor).$(VerMinor)-$(VerPatch)

CoreHeader=evillib.$(Version).h
CoreSource=evillib.$(Version).c

# librarys
CLIBS =-lpthread
CLIBS+=-lc
CLIBS+=-lblkid


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
sources += net/etCommTCP.c

sourcesFull=$(addprefix $(sourcePath)/,$(sources))

headers=$(sources:.c=.h)

objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/,$(objects))






CoreAppSources+=$(shell ls app/*.c)
CoreAppSources+=$(shell ls app/apicheck/*.c)
CoreAppSourcesFull=$(addprefix $(sourcePath)/,$(CoreAppSources))

CoreAppObjects=$(addprefix $(buildPath)/,$(CoreAppSources))
CoreAppObjectsFull=$(CoreAppObjects:.c=.o)



#CoreSourceFiles+=
CoreLibraryShared=libevillib.$(Version).so
CoreLibraryStatic=libevillib.$(Version).a

.EXPORT_ALL_VARIABLES:


#################################### Standard ####################################
all: evillib-core-dev evillib-core

help:
	@echo -n "$(CComment)"
	@echo "##################### Core library #####################"
	@echo "$(MAKE) evillib-core: Build evillib-core library ( in $(buildPath)/libevillib.so )"
	@echo "$(MAKE) evillib-core-clean: Clean the build directory of core"
	@echo "$(MAKE) evillib-core-dev: Build development files ( in $(buildPath)/evillib.h )"
	@echo "$(MAKE) evillib-app: create the evillib app"
	@echo "$(MAKE) evillib-app-clean"

	@echo "$(MAKE) install: Install development headers"
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
	@$(RM) $(buildPath)/$(CoreHeader)
	@$(RM) $(includeDir)/evillib/$(CoreHeader)
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

$(buildPath)/$(CoreHeader): evillib_version.h $(buildPath)
	@echo "${CCommand}make $@ ${CNormal}"
	cat evillib_start.h > $@
	cat evillib_depends.h >> $@
	cat evillib_version.h >> $@
	cat evillib_defines.h >> $@
	cat $(headers) >> $@
	cat evillib_end.h >> $@


# Output directory
$(includeDir)/evillib/$(CoreHeader): $(buildPath)/$(CoreHeader)
	@mkdir -p $(includeDir)/evillib/
	@$(CP) $< $@
$(includeDir)/evillib.h: $(includeDir)/evillib/$(CoreHeader)
	@$(LN) evillib/$(CoreHeader) $@


#################################### Library ####################################
evillib-core: $(libDir)/libevillib.so
evillib-core-static: $(libDir)/libevillib.a
evillib-core-clean:
	@$(RM) $(buildPath)/$(CoreSource)
	@$(RM) $(buildPath)/libevillib.o
	@$(RM) $(buildPath)/$(CoreLibraryShared)
	@$(RM) $(libDir)/evillib/$(CoreLibraryShared)
	@$(RM) $(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(RM) $(libDir)/libevillib.so


$(buildPath)/$(CoreSource): evillib_version.h $(buildPath)/$(CoreHeader)
	@echo "${CCommand}make $@ ${CNormal}"
	cat $(buildPath)/$(CoreHeader) > $@
	cat evillib_start.c >> $@
	cat $(sourcesFull) >> $@
	cat evillib_end.c >> $@

$(buildPath)/libevillib.o: $(buildPath)/$(CoreSource)
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


#################################### docu target ####################################
docu: $(buildPath)/$(CoreHeader)


#################################### Targets for development ####################################
$(buildPath)/%.o: $(sourcePath)/%.c
	@mkdir -v -p $$(dirname $@)
	$(CC) -fPIC -I. -Wall -DET_SINGLEMODULE -c $(CFLAGS) $< -o $@

#################################### APP ####################################
evillib-app: $(binDir)/evillib
evillib-app-clean: evillib-core-clean
	@echo "${CCommand}make $@ ${CNormal}"
	@find $(buildPath) -name "*.o" -print -delete
	@$(RM) $(binDir)/evillib
	@$(RM) $(buildPath)/evillib
evillib-app-debug: $(objectsFull) $(CoreAppObjectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) $(objectsFull) $(CoreAppObjectsFull) $(CLIBS) \
	-o $(buildPath)/evillib

evillib-app-run: $(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib
	LD_PRELOAD=$(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib --apicheck all


$(buildPath)/evillib: $(CoreAppObjectsFull) evillib_version.h $(libDir)/libevillib.so 
	@echo "${CCommand}make $@ ${CNormal}"
	gcc $(CoreAppObjectsFull) \
	-L$(buildPath) -levillib \
	-o $(buildPath)/evillib


$(binDir)/evillib: $(buildPath)/evillib
	@$(MKDIR) $(binDir)
	@$(CP) $(buildPath)/evillib $@


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
