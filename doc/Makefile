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

.DEFAULT:
	@echo "$@: This target dont exist in docu Makefile"

help:
	@echo -n "$(CComment)"
	@echo "##################### Documentation #####################"
	@echo "$(MAKE) docu: create doxygen documentation to $(outputDir)$(docDir)"
	@echo "$(MAKE) docu-clean: clean up $(outputDir)$(docDir)"

all: docu

docu:
	mkdir -p ./output
	SOURCEDIR=../ \
	OUTPUTDIR=./output \
	SECTION="DEV" \
	doxygen 

docu-clean:
	@$(RM) -fR $(outputDir)$(docDir)/*
	@$(RMDIR) -p $(outputDir)$(docDir)
