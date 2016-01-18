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



sourcePath=$(sourceBasePath)/extra
buildPath=$(buildBasePath)/extra

sources = $(shell cd $(sourcePath) && find . -name "*.c" ! -wholename "*specialheaders/*" ! -wholename "*develop/*" ! -wholename "*tests/*" -printf "%p " )
sourcesFull=$(addprefix $(sourcePath)/,$(sources))

headers=$(sources:.c=.h)
headersFull=$(addprefix $(sourcePath)/,$(headers))



CFLAGS+=-g
CFLAGS+=-rdynamic
CFLAGS+=-I $(sourcePath)
CFLAGS+=-I $(buildPath)
CFLAGS+=-I $(sourceBasePath)/core

CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3
CLIBS+=-lssl
CLIBS+=-lcrypto 



ExtraLibraryShared=libevillib-extra.$(Version).so
ExtraLibraryStatic=libevillib-extra.$(Version).a

.DEFAULT:
	@sleep 0

help:
	@echo -n "$(CComment)"
	@echo "##################### Extras #####################"
	@echo "$(MAKE) evillib-extra: Build evillib-extra library ( in $(buildPath)/libevillib-extra.so )"
	@echo -n "$(CNormal)"

clean:
	@$(RM) $(buildPath)/$(ExtraLibraryShared)
	@$(RM) $(buildPath)/libevillib-extra.$(Version).c

install: evillib-extra-install

#################################### Library ####################################
evillib-extra: $(libDir) $(buildPath)/$(ExtraLibraryShared)
evillib-extra-install: $(libDir)/libevillib-extra.so




$(buildPath)/$(ExtraLibraryShared): $(buildPath)/libevillib-extra.o
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -shared -Wl,-soname,libevillib-extra.so \
	$< \
	$(CLIBS) \
	-o $@

$(buildPath)/libevillib-extra.o: $(buildPath)/evillib-extra.$(Version).c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) $< -o $@

$(buildPath)/evillib-extra.$(Version).c: $(buildPath)/evillib-extra.$(Version).prep.c
	@echo "${CCommand}make $@ ${CNormal}"
	@cat $(sourcePath)/evillib_depends.h > $@
	@cat $< >> $@

$(buildPath)/evillib-extra.$(Version).prep.c: $(buildPath)/evillib-extra.$(Version).concat.c
	@echo "${CCommand}make $@ ${CNormal}"
	@$(CC) -E \
	-D_H_etDepends \
	$(CFLAGS) \
	$< > $@

$(buildPath)/evillib-extra.$(Version).concat.c:
	@echo "${CCommand}make $@ ${CNormal}"
	@$(MKDIR) $(buildPath)
	@cat $(sourcePath)/specialheaders/evillib_start.c >> $@
	@cat $(sourcesFull) >> $@
	@cat $(sourcePath)/specialheaders/evillib_end.c >> $@





objects=$(sources:.c=.o)
objectsFull=$(addprefix $(buildPath)/,$(objects))

evillib-app: $(buildBasePath)/obj/$(ExtraLibraryShared)
evillib-app-clean:
	@$(RM) $(objectsFull)

$(buildBasePath)/obj/$(ExtraLibraryShared): $(objectsFull)
	@echo "${CCommand}make $@ ${CNormal}"
	@mkdir -v -p $$(dirname $@)
	$(CC) -shared -Wl,-soname,libevillib.so \
	$(CFLAGS) -g \
	$(CLIBS) \
	$(objectsFull) \
	-o $@
	ln -fs $(buildBasePath)/obj/$(ExtraLibraryShared) $(buildBasePath)/obj/libevillib-extra.so


$(buildPath)/%.o: $(sourcePath)/%.c
	@mkdir -v -p $$(dirname $@)
	$(CC) -fPIC -I. -Wall -DET_SINGLEMODULE -g -c $(CFLAGS) $< -o $@




# Main Parts
