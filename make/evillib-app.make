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

include make/evillib-version.make


sourcePath=$(sourceBasePath)
buildPath=$(buildBasePath)

sources = $(shell find ./tests -name "*.c" -printf "%p " )
sourcesFull=$(sources)

objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/,$(objects))

sharedObjects=$(sources:.c=.so)
sharedObjectsFull=$(addprefix $(buildPath)/,$(sharedObjects))

progs=$(sources:.c=.check)
progsFull=$(addprefix $(buildPath)/,$(progs))

progsObjects=$(sources:.c=)
progsObjectsFull=$(addprefix $(buildPath)/,$(progsObjects))


#CFLAGS=
#CLIBS=

CFLAGS+=-I $(buildPath)/core
CFLAGS+=-I ./
CFLAGS+=-I ./core
CFLAGS+=-I ./extra
CFLAGS+=-I ./extra/db
CFLAGS+=-g

CLIBS+=-lpthread
CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3
CLIBS+=-L$(buildPath)/obj 
#CLIBS+=-levillib.$(Version)
#CLIBS+=-levillib-extra.$(Version)

.EXPORT_ALL_VARIABLES:

.DEFAULT:
	@sleep 0

#################################### APP ####################################
#evillib-app: $(binDir)/evillib
evillib-app: $(progsObjectsFull)
clean: evillib-core-clean
	@echo "${CCommand}make $@ ${CNormal}"
	@$(RM) $(objectsFull)
	@$(RM) $(sharedObjectsFull)
	@$(RM) $(binDir)/evillib
	@$(RM) $(buildPath)/evillib
	@$(RM) $(buildPath)/evillib.o
	@$(RM) $(progsObjectsFull)


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
	LD_PRELOAD=/tmp/evillib/build/core/obj/libevillib.00.15-00.so \
	valgrind  --suppressions=/usr/src/projects/evillib/tests/valgrind.supp \
	$(buildPath)/evillib --apicheck /tmp/evillib/build/tests etMemoryBlockList.so --debug




$(buildPath)/%: $(PWD)/%.c
	@mkdir -v -p $$(dirname $@)
	@echo ""
	$(CC) \
	$(CFLAGS) \
	$(CLIBS) \
	$< \
	-o $@












