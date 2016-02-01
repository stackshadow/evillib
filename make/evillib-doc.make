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

include evillib-version.make

.DEFAULT:
	@sleep 0

help:
	@echo -n "$(CComment)"
	@echo "\nDocumentation:"
	@echo "\t$(MAKE) evillib-doc: Build the documentation ( in $(docDir)/evillib/$(Version) )"
	@echo -n "$(CNormal)"

clean:
	$(RM) -R $(docDir)/evillib/$(Version)

evillib-doc: $(docDir)/evillib/$(Version)


$(docDir)/evillib/$(Version):
	mkdir -p $(docDir)/evillib/$(Version)
	cd documentation && SOURCEDIR=.. OUTPUTDIR="$(docDir)/evillib/$(Version)" doxygen 

.PHONY: $(docDir)/evillib/$(Version)
