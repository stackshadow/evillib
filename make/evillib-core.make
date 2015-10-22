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

include evillib-version.make
include evillib-core-sources.make

# librarys
CFLAGS+=-rdynamic

CLIBS =-lpthread
CLIBS+=-lc
CLIBS+=-lblkid
CLIBS+=-ldl



objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/core/,$(objects))


#CoreSourceFiles+=
CoreLibraryShared=libevillib.$(Version).so
CoreLibraryStatic=libevillib.$(Version).a

.EXPORT_ALL_VARIABLES:

.DEFAULT:
	@sleep 0

help:
	@echo -n "$(CComment)"
	@echo "##################### Core library #####################"
	@echo "$(MAKE) evillib-core: Build evillib-core library ( in $(libDir)/libevillib.so )"
	@echo "$(MAKE) evillib-core-clean: Clean the build directory of core"
	@echo -n "$(CNormal)"

clean: evillib-core-dev-clean evillib-core-clean evillib-app-clean
	@$(RM) $(buildPath)/evillib.$(Version).c
	@$(RM) $(buildPath)/libevillib.o
	@$(RM) $(buildPath)/$(CoreLibraryShared)

install: evillib-core-install

#################################### Library ####################################
evillib-core: $(buildPath)/$(CoreLibraryShared)
evillib-core-install: $(libDir)/libevillib.so $(shareDir)/pkgconfig/evillib.pc
evillib-core-uninstall: 
	@$(RM) $(libDir)/evillib/$(CoreLibraryShared)
	@$(RM) $(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(RM) $(libDir)/libevillib.so
	@$(RM) $(shareDir)/pkgconfig/evillib.pc
evillib-core-static: $(libDir)/libevillib.a


$(buildPath)/evillib.$(Version).c: evillib_version.h $(buildPath)/evillib.$(Version).h
	@echo "${CCommand}make $@ ${CNormal}"
	cat $(buildPath)/evillib.$(Version).h > $@
	cat $(sourcePath)/evillib_start.c >> $@
	cat $(sourcesFull) >> $@
	cat $(sourcePath)/evillib_end.c >> $@

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
$(libDir)/libevillib.so: $(buildPath)/$(CoreLibraryShared)
	@$(MKDIR) $(libDir)/evillib
	@$(CP) $(buildPath)/$(CoreLibraryShared) $(libDir)/evillib/
	@$(LN) evillib/$(CoreLibraryShared) $(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(LN) evillib/$(CoreLibraryShared) $(libDir)/libevillib.so

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
$(buildPath)/%.o: $(sourcePath)/%.c
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



