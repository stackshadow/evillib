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

ifneq ($(VARS),1)
	include make/vars.make
endif



sourcePathCore=$(sourcePath)/core
buildPathCore=$(buildPath)/core

# librarys
CFLAGS+=-rdynamic
CFLAGS+=-I $(sourcePathCore)
CFLAGS+=-I $(sourcePathCore)/specialheaders
CFLAGS+=-I $(buildPathCore)
CFLAGS+=-DET_INTERNAL


CLIBS =-lpthread
CLIBS+=-lc
CLIBS+=-lblkid
CLIBS+=-ldl


sources  = $(shell find $(sourcePathCore) -name "*.c" ! -wholename "*specialheaders/*" ! -wholename "*develop/*" ! -wholename "*tests/*" -printf "%p " )
headers  = $(sources:.c=.h)
headers += evillib_depends.h
headers += evillib_version.h
headers += evillib_defines.h
objects  = $(sources:.c=.o)



.EXPORT_ALL_VARIABLES:

.DEFAULT:
	@sleep 0

help:
	@echo ""
	@echo -n "$(CComment)"
	@echo "##################### Core library #####################"
	@echo "$(MAKE) -f make/Makefile evillib-core: Build evillib-core library ( in $(libDir)/libevillib.so )"
	@echo "$(MAKE) -f make/Makefile evillib-core-clean: Clean the build directory of core"
	@echo "$(MAKE) -f make/Makefile evillib-core-install: copy the created evillib to $(libDir)/libevillib.so"
	@echo "$(MAKE) -f make/Makefile evillib-core-uninstall: remove the evillib"
	@echo "FullSources: $(sourcesFull)"
	@echo "FullObjects: $(objects)"
	@echo -n "$(CNormal)"




install: evillib-core-install


#################################### Headers ####################################

headersRel = $(subst $(sourcePathCore)/,,$(headers))
headersTarget = $(addprefix $(includeDir)/evillib-$(Version)/,$(headersRel))

evillib-app: $(sourcePathCore)/evillib_version.h
$(sourcePathCore)/evillib_version.h:
	@echo "${CCommand}make $@ ${CNormal}"
	$(MKDIR) $(buildPath)
	@echo "// Evillib - Version" > $@
	@echo "#define ET_VERSION_MAJOR $(VerMajor)" >> $@
	@echo "#define ET_VERSION_MINOR $(VerMinor)" >> $@
	@echo "#define ET_VERSION_PATCH $(VerPatch)" >> $@
	@echo "#define ET_VERSIONSTRING \"$(VerMajor).$(VerMinor)-$(VerPatch)\"" >> $@

# Output directory
dev-install: $(sourcePathCore)/evillib_version.h $(headersTarget) $(includeDir)/evillib
$(includeDir)/evillib:
	@$(LN) -f -s -v $(includeDir)/evillib-$(Version) $(includeDir)/evillib

# this copy all source headers to target
$(includeDir)/evillib-$(Version)/%.h: $(sourcePathCore)/%.h
	@$(MKDIR) $(shell dirname $@)
	@$(CP) $< $@

dev-clean:
	@$(RM) $(sourcePathCore)/evillib_version.h


#################################### Library ####################################
#CoreSourceFiles+=
CoreLibraryShared=libevillib.$(Version).so

library: $(buildPathCore)/$(CoreLibraryShared)

$(buildPathCore)/$(CoreLibraryShared): $(buildPathCore)/libevillib.o
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -shared -Wl,-soname,libevillib.so \
	$(buildPathCore)/libevillib.o \
	$(CLIBS) \
	-o $@

$(buildPathCore)/libevillib.o: $(buildPathCore)/evillib.$(Version).c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) $< -o $@

$(buildPathCore)/evillib.$(Version).c: $(buildPathCore)/evillib.$(Version).prep.c
	@echo "${CCommand}make $@ ${CNormal}"
	cat core/evillib_depends.h > $@
	cat $< >> $@

$(buildPathCore)/evillib.$(Version).prep.c: $(buildPathCore)/evillib.$(Version).concat.c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -E \
	-D_H_etDepends \
	$(CFLAGS) \
	$< > $@

$(buildPathCore)/evillib.$(Version).concat.c: $(sourcePathCore)/evillib_version.h
	@echo "${CCommand}make $@ ${CNormal}"
	@$(MKDIR) $(buildPathCore)
	@echo "" > $@
	cat $(sources) >> $@


library-install: $(libDir)/libevillib.so $(shareDir)/pkgconfig/evillib.pc

library-clean:
	@$(RM) $(buildPathCore)/evillib.$(Version).concat.c
	@$(RM) $(buildPathCore)/evillib.$(Version).prep.c
	@$(RM) $(buildPathCore)/evillib.$(Version).c
	@$(RM) $(buildPathCore)/libevillib.o
	@$(RM) $(buildPathCore)/$(CoreLibraryShared)

# Output directory
$(libDir)/libevillib.so: $(buildPathCore)/$(CoreLibraryShared)
	@$(MKDIR) $(libDir)/evillib
	@$(CP) $(buildPathCore)/$(CoreLibraryShared) $(libDir)/evillib/
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
	@echo "Cflags: -I$(includeDir)/evillib" >> $@



clean: dev-clean library-clean

touch:
	find . ! -wholename "*.git/*" ! -wholename "*ide/*" -print -exec touch --no-create {} \;