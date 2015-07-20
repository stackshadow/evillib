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
include evillib-extra-sources.make


CFLAGS+=-g

CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3


objects+=$(sources:.c=.o)
objectsFull+=$(addprefix $(buildPath)/,$(objects))


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
	@$(RM) $(buildPath)/libevillib-extra.$(Version).so

#################################### Library ####################################
evillib-extra: $(libDir) $(libDir)/libevillib-extra.so


$(buildPath)/libevillib-extra.$(Version).c: $(buildPath)
	@echo "${CCommand}make $@ ${CNormal}"
	#cat $(buildPath)/$(CoreHeader) > $@
	#cat evillib_start.c >> $@
	cat $(buildPath)/libevillib-extra.$(Version).h > $@
	cat $(sourcesFull) >> $@
	#cat $(sourcePath)/evillib_end.c >> $@

$(buildPath)/$(ExtraLibraryShared): $(buildPath)/libevillib-extra.$(Version).c
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) \
	$(CLIBS) \
	$< -o $@


$(libDir)/libevillib-extra.so: $(buildPath)/libevillib-extra.$(Version).so
	@$(MKDIR) $(libDir)/evillib
	@$(CP) $< $(libDir)/evillib/
	@$(LN) evillib/libevillib-extra.$(Version).so $@


# Main Parts
