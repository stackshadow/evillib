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

.DEFAULT:
	@echo "$@: This target dont exist in docu Makefile"

.PHONY: doc


help:
	@echo -n "$(CComment)"
	@echo "##################### Documentation #####################"
	@echo "$(MAKE) doc: create doxygen documentation to $(outputDir)$(docDir)"
	@echo "$(MAKE) doc-clean: clean up $(outputDir)$(docDir)"
	@echo -n "$(CNormal)"

all: doc

$(docDir):
	@$(MKDIR) $(docDir)

doc: $(docDir)
	cd documentation && \
	SOURCEDIR=../ \
	SECTION="DEV" \
	OUTPUTDIR=$(docDir) \
	doxygen 

doc-clean:
	@$(RM) -fR $(outputDir)$(docDir)/*
	@$(RMDIR) -p $(outputDir)$(docDir)

evillib-doc: doc
