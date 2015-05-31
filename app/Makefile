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


include ../core/MakefileBase.make
CFLAGS=
CLIBS=

CFLAGS+=-DET_SINGLEMODULE
CFLAGS+=-I ../core
CFLAGS+=-I ../extra/db
CFLAGS+=-I ../extra/dbdriver
CFLAGS+=-g

CLIBS+=-lpthread
CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3

sources+=$(shell ls *.c)
sourcesFull=$(addprefix $(PWD)/,$(sources))

objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/app/,$(objects))


#################################### APP ####################################
evillib-app: $(binDir)/evillib
evillib-app-clean: evillib-core-clean
	@echo "${CCommand}make $@ ${CNormal}"
	@find $(buildPath) -name "*.o" -print -delete
	@$(RM) $(binDir)/evillib
	@$(RM) $(buildPath)/evillib


evillib-app-debug: evillib-app-debug-clean $(objectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) $(objectsFull) \
	$(CFLAGS) \
	$(CLIBS) \
	-o $(buildPath)/evillib

evillib-app-debug-clean:
	@$(RM) $(objectsFull)


evillib-app-run: $(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib
	LD_PRELOAD=$(buildPath)/$(CoreLibraryShared) $(buildPath)/evillib --apicheck all


$(buildPath)/app/%.o: $(PWD)/%.c
	@mkdir -v -p $$(dirname $@)
	$(CC) -fPIC -I. -Wall -DET_SINGLEMODULE -c $(CFLAGS) $< -o $@


$(buildPath)/evillib: $(CoreAppObjectsFull) evillib_version.h $(libDir)/libevillib.so 
	@echo "${CCommand}make $@ ${CNormal}"
	gcc $(CoreAppObjectsFull) \
	-L$(buildPath) -levillib \
	$(CFLAGS) \
	-o $(buildPath)/evillib


$(binDir)/evillib: $(buildPath)/evillib
	@$(MKDIR) $(binDir)
	@$(CP) $(buildPath)/evillib $@











