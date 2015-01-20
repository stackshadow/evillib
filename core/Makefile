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


#!/bin/make
sourceCoreDir=$(sourceDir)/core
buildCoreDir=$(buildDir)/core
CoreHeader=evillib.$(Version).h
CoreSource=evillib.$(Version).c


CoreSources = $(shell ls core/*.c)
CoreSources+= $(shell ls memory/*.c)
CoreSources+= $(shell ls string/*.c)
CoreSourcesFull=$(addprefix $(sourceCoreDir)/,$(CoreSources))

CoreObjects=$(addprefix $(buildCoreDir)/,$(CoreSources))
CoreObjectsFull=$(CoreObjects:.c=.o)

CoreAppSources+=$(shell ls app/*.c)
CoreAppSources+=$(shell ls app/apicheck/*.c)
CoreAppSourcesFull=$(addprefix $(sourceCoreDir)/,$(CoreAppSources))

CoreAppObjects=$(addprefix $(buildCoreDir)/,$(CoreAppSources))
CoreAppObjectsFull=$(CoreAppObjects:.c=.o)



#CoreSourceFiles+=
CoreLibraryShared=libevillib.$(Version).so

.EXPORT_ALL_VARIABLES:


#################################### Standard ####################################
all: evillib-core-dev evillib-core

help:
	@echo -n "$(CComment)"
	@echo "##################### Core library #####################"
	@echo "$(MAKE) evillib-core: Build evillib-core library ( in $(buildCoreDir)/libevillib.so )"
	@echo "$(MAKE) evillib-core-clean: Clean the build directory of core"
	@echo "$(MAKE) evillib-core-dev: Build development files ( in $(buildCoreDir)/evillib.h )"
	@echo "$(MAKE) evillib-app: create the evillib app"
	@echo "$(MAKE) evillib-app-clean"

	@echo "$(MAKE) install: Install development headers"
	@echo -n "$(CNormal)"

clean: evillib-core-clean evillib-core-dev-clean evillib-app-clean
	@find .           -name "*.o" -print -delete
	@find $(buildDir) -name "*.o" -print -delete


folders:

install: core

#################################### Development / Headers ####################################
evillib-core-dev: $(outputDir)$(includeDir)/evillib.h
evillib-core-dev-clean:
	@$(RM) evillib_version.h
	@$(RM) $(CoreHeader).prep
	@$(RM) $(CoreHeader)
	@$(RM) $(outputDir)$(includeDir)/evillib/$(CoreHeader)
	@$(RM) $(outputDir)$(includeDir)/evillib.h

$(outputDir)$(includeDir)/evillib/$(CoreHeader): $(CoreHeader)
	@mkdir -p $(outputDir)$(includeDir)/evillib/
	@$(CP) $(CoreHeader) $@
$(outputDir)$(includeDir)/evillib.h: $(outputDir)$(includeDir)/evillib/$(CoreHeader)
	@$(LN) evillib/$(CoreHeader) $@

$(buildCoreDir):
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


$(CoreHeader).prep: $(buildCoreDir)
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -Wall -fPIC -I. -E -P -C ./evillib_full.h -o $@

$(CoreHeader): $(CoreHeader).prep evillib_version.h
	@echo "${CCommand}make $@ ${CNormal}"
	cat evillib_start.h > $@
	cat evillib_depends.h >> $@
	cat evillib_version.h >> $@
	cat evillib_defines.h >> $@
	cat $(CoreHeader).prep >> $@
	cat evillib_end.h >> $@


#################################### Library ####################################
evillib-core: $(outputDir)$(libDir)/libevillib.so $(outputDir)$(libDir)/libevillib.$(VerMajor).$(VerMinor).so
evillib-core-clean:
	@$(RM) $(CoreSource).prep
	@$(RM) $(CoreSource)
	@$(RM) $(buildCoreDir)/libevillib.o
	@$(RM) $(buildCoreDir)/$(CoreLibraryShared)
	@$(RM) $(outputDir)$(libDir)/evillib/$(CoreLibraryShared)
	@$(RM) $(outputDir)$(libDir)/libevillib.$(VerMajor).$(VerMinor).so
	@$(RM) $(outputDir)$(libDir)/libevillib.so


$(CoreSource).prep: evillib_version.h
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -Wall -fPIC -I. -E -P -C ./evillib_full.c -o $@

$(CoreSource): $(CoreSource).prep
	@echo "${CCommand}make $@ ${CNormal}"
	cat evillib_start.h > $@
	cat evillib_depends.h >> $@
	cat evillib_version.h >> $@
	cat $(CoreSource).prep >> $@
	cat evillib_end.h >> $@

$(buildCoreDir)/libevillib.o: $(buildCoreDir) $(CoreSource)
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) $(CoreSource) -o $@

$(buildCoreDir)/$(CoreLibraryShared): $(buildCoreDir)/libevillib.o
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -shared -Wl,-soname,libevillib.so \
	$(buildCoreDir)/libevillib.o \
	-lc \
	-lblkid \
	-o $@


# Output directory
$(outputDir)$(libDir)/evillib/$(CoreLibraryShared): $(buildCoreDir)/$(CoreLibraryShared)
	@$(MKDIR) $(outputDir)$(libDir)/evillib
	@$(CP) $(buildCoreDir)/$(CoreLibraryShared) $@

$(outputDir)$(libDir)/libevillib.$(VerMajor).$(VerMinor).so: $(outputDir)$(libDir)/evillib/$(CoreLibraryShared)
	@$(LN) evillib/$(CoreLibraryShared) $@

$(outputDir)$(libDir)/libevillib.so: $(outputDir)$(libDir)/evillib/$(CoreLibraryShared)
	@$(LN) evillib/$(CoreLibraryShared) $@


#################################### docu target ####################################
docu: $(buildCoreDir) $(buildCoreDir)/$(CoreSource).prep
docu-clean:
	@$(RM) $(buildCoreDir)/$(CoreSource).prep

#################################### Targets for development ####################################
$(buildCoreDir)/%.o: $(sourceCoreDir)/%.c
	@mkdir -v -p $$(dirname $@)
	$(CC) -fPIC -I. -Wall -DET_SINGLEMODULE -c $(CFLAGS) $< -o $@

#################################### APP ####################################
evillib-app: $(outputDir)$(binDir)/evillib
evillib-app-clean: evillib-core-clean
	@echo "${CCommand}make $@ ${CNormal}"
	@find $(buildCoreDir) -name "*.o" -print -delete
	@$(RM) $(outputDir)$(binDir)/evillib
	@$(RM) $(buildDir)/evillib
evillib-app-debug: $(CoreObjectsFull) $(CoreAppObjectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	gcc $(CoreObjectsFull) $(CoreAppObjectsFull) \
	-o $(buildDir)/evillib

evillib-app-run: $(buildCoreDir)/$(CoreLibraryShared) $(buildDir)/evillib
	LD_PRELOAD=$(buildCoreDir)/$(CoreLibraryShared) $(buildDir)/evillib --apicheck


$(outputDir)$(binDir)/evillib: $(buildDir)/evillib
	@$(MKDIR) $(outputDir)$(binDir)
	@$(CP) $(buildDir)/evillib $@

$(buildDir)/evillib: $(buildCoreDir)/$(CoreLibraryShared) $(CoreAppObjectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	gcc $(CoreAppObjectsFull) \
	-L$(buildCoreDir) -levillib.$(Version) \
	-o $(buildDir)/evillib



# this holds apicheckObjects
#include $(sourceCoreDir)/app/apichecks/Makefile

#apicheckObjects += $(buildCoreDir)/app/evillib.o

#evillib-app: $(buildDir)/evillib 
#$(buildDir)/evillib: $(sourceCoreDir)/evillib_version.h $(apicheckObjects) $(CoreDeployDir)
#	gcc -L $(buildCoreDir) $(apicheckObjects) -levillib.00.14-04 -o $(buildDir)/evillib

#evillib-app-single: $(buildDir)/evillib-single
#$(buildDir)/evillib-single: $(SingleObjects)
#	gcc $(apicheckObjects) $(SingleObjects) -o $(buildDir)/evillib

output:
	@echo $(CoreObjectsFull)
	@echo $(CoreAppObjectsFull)
