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


sourcePath=$(sourceBasePath)
buildPath=$(buildBasePath)

sources = $(shell find ./tests -name "*.c" -printf "%p " )
sourcesFull=$(sources)

objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/,$(objects))

sharedObjects=$(sources:.c=.so)
sharedObjectsFull=$(addprefix $(buildPath)/,$(sharedObjects))

#CFLAGS=
#CLIBS=

CFLAGS+=-I ./core
CFLAGS+=-I ./extra
CFLAGS+=-g

CLIBS+=-lpthread
CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3



.EXPORT_ALL_VARIABLES:

.DEFAULT:
	@sleep 0

#################################### APP ####################################
evillib-app: $(binDir)/evillib
clean: evillib-core-clean
	@echo "${CCommand}make $@ ${CNormal}"
	@$(RM) $(objectsFull)
	@$(RM) $(sharedObjectsFull)
	@$(RM) $(binDir)/evillib
	@$(RM) $(buildPath)/evillib


evillib-app-debug: evillib-app-debug-clean $(objectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	#$(CC) $(objectsFull) \
	$(CFLAGS) \
	$(CLIBS) \
	-o $(buildPath)/evillib

evillib-app-debug-clean:
	@echo "${CCommand}make $@ ${CNormal}"
	@$(RM) $(objectsFull)
	@$(RM) -f $(tempPath)/db.sqlite
	@$(MKDIR) -p $(tempPath)


evillib-app-run: $(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib
	LD_PRELOAD=$(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib --apicheck all
evillib-app-memcheck:
	LD_PRELOAD=$(buildPath)/$(CoreLibraryShared) valgrind $(buildPath)/evillib --apicheck all


$(binDir)/evillib: $(buildPath)/evillib $(sharedObjectsFull)
	@$(MKDIR) $(binDir)
	@$(CP) $(buildPath)/evillib $@
	
$(buildPath)/evillib: app/evillib.c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -I. -Wall $(CFLAGS) -c app/evillib.c -o $@.o
	$(CC) -I. -Wall $(CFLAGS) $(CLIBS) -L$(buildPath)/core/obj -levillib.$(Version) $@.o -o $@


$(buildPath)/%.so: $(buildPath)/%.o
	@mkdir -v -p $$(dirname $@)
	@echo ""
	$(CC) -shared \
	-L$(buildPath)/core/obj -levillib.$(Version) \
	$(CFLAGS) \
	$< \
	-o $@

$(buildPath)/%.o: $(PWD)/%.c
	@mkdir -v -p $$(dirname $@)
	@echo ""
	$(CC) -fPIC -I. -Wall -c $(CFLAGS) $< -o $@















