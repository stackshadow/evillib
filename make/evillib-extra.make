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


sourcePathExtra=$(sourcePath)/extra
buildPathExtra=$(buildPath)/extra

CFLAGS+=-g
CFLAGS+=-rdynamic
CFLAGS+=-I $(sourcePathExtra)
CFLAGS+=-I $(buildPathExtra)
CFLAGS+=-I $(sourcePath)/core

CLIBS+=-ldl
CLIBS+=-ljansson
CLIBS+=-lsqlite3
CLIBS+=-lssl
CLIBS+=-lcrypto 




sources  = $(shell find $(sourcePathExtra) -name "*.c" ! -wholename "*specialheaders/*" ! -wholename "*develop/*" ! -wholename "*tests/*" -printf "%p " )
headers  = $(sources:.c=.h)
headers += evillib-extra_depends.h


.DEFAULT:
	@sleep 0

help:
	@echo -n "$(CComment)"
	@echo "##################### Extras #####################"
	@echo "$(MAKE) evillib-extra: Build evillib-extra library ( in $(buildPathExtra)/libevillib-extra.so )"
	@echo -n "$(CNormal)"


#################################### Headers ####################################

headersRel = $(subst $(sourcePathExtra)/,,$(headers))
headersTarget = $(addprefix $(includeDir)/evillib-$(Version)/,$(headersRel))


# this copy all source headers to target
$(includeDir)/evillib-$(Version)/%.h: $(sourcePathExtra)/%.h
	@$(MKDIR) $(shell dirname $@)
	@$(CP) $< $@

dev-install: $(headersTarget)


#################################### Library ####################################
ExtraLibraryShared=libevillib-extra.$(Version).so

library: $(buildPathExtra)/$(ExtraLibraryShared)

$(buildPathExtra)/$(ExtraLibraryShared): $(buildPathExtra)/libevillib-extra.o
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -shared -Wl,-soname,libevillib-extra.so \
	$< \
	$(CLIBS) \
	-o $@

$(buildPathExtra)/libevillib-extra.o: $(buildPathExtra)/evillib-extra.$(Version).c
	@echo "${CCommand}make $@ ${CNormal}"
	$(CC) -fPIC -c -Wall \
	$(CFLAGS) $< -o $@

$(buildPathExtra)/evillib-extra.$(Version).c: $(buildPathExtra)/evillib-extra.$(Version).prep.c
	@echo "${CCommand}make $@ ${CNormal}"
	@cat $(sourcePathExtra)/evillib-extra_depends.h > $@
	@cat $< >> $@

$(buildPathExtra)/evillib-extra.$(Version).prep.c: $(buildPathExtra)/evillib-extra.$(Version).concat.c
	@echo "${CCommand}make $@ ${CNormal}"
	@$(CC) -E \
	-D_H_etDepends \
	-D_H_evillibExtra_depends \
	$(CFLAGS) \
	$< > $@

$(buildPathExtra)/evillib-extra.$(Version).concat.c:
	@echo "${CCommand}make $@ ${CNormal}"
	@$(MKDIR) $(buildPathExtra)
	@echo "#ifdef __cplusplus" > $@
	@echo "extern \"C\" {" >> $@
	@echo "#endif" >> $@
	
	@cat $(sources) >> $@
	
	@echo "#ifdef __cplusplus" >> $@
	@echo "}" >> $@
	@echo "#endif" >> $@



library-install: $(libDir)/libevillib-extra.so
$(libDir)/libevillib-extra.so: $(buildPathExtra)/$(ExtraLibraryShared)
	@$(MKDIR) $(libDir)/evillib
	@$(CP) $(buildPathExtra)/$(ExtraLibraryShared) $(libDir)/evillib/
	@$(LN) evillib/$(ExtraLibraryShared) $(libDir)/libevillib-extra.$(VerMajor).$(VerMinor).so
	@$(LN) evillib/$(ExtraLibraryShared) $(libDir)/libevillib-extra.so

library-clean:
	@$(RM) $(buildPathExtra)/libevillib-extra.o
	@$(RM) $(buildPathExtra)/evillib-extra.$(Version).c
	@$(RM) $(buildPathExtra)/evillib-extra.$(Version).prep.c
	@$(RM) $(buildPathExtra)/evillib-extra.$(Version).concat.c
    
    