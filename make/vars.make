#!/bin/make
#The evillib build-system
#	Copyright (C) 2015 by Martin Langlotz
#
#	This file is part of evillib.
#
#	evillib is free software: you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#	evillib is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public License
#	along with evillib.  If not, see <http://www.gnu.org/licenses/>.

# Export all variables
export

# Colours
export VARS=1
export CNormal=\033[00m
export CCommand=\033[1;35m
export CComment=\033[36m
export COK=\033[32m

# Compiler
export CC						= ${CROSSCOMPILE}gcc
export CXX						= ${CROSSCOMPILE}g++
export AR                 		= ${CROSSCOMPILE}ar
export RANLIB             		= ${CROSSCOMPILE}ranlib
export RM                 		= rm -v -f
export CP                  		= cp -v -f
export LN                  		= ln -v -f -s
export MKDIR               		= mkdir -p -v
export RMDIR					= rmdir -v

# Directorys
export buildPath	        	?= /tmp/evillib/build
export tempPath					?= /tmp/evillib/tmp
export prefix	            	?= /tmp/evillib/target

export includeDir				= $(prefix)/include
export binDir					= $(prefix)/bin
export libDir					= $(prefix)/lib
export shareDir					= $(prefix)/share
export docDir					= $(prefix)/share/doc


# Version of evillib
VerMajor=00
VerMinor=15
VerPatch=00
Version=$(VerMajor).$(VerMinor)-$(VerPatch)



#################################### Additional ( Folders ) ####################################

$(buildPath):
	$(MKDIR) $@
$(includeDir):
	$(MKDIR) $@
$(binDir):
	$(MKDIR) $@
$(libDir):
	$(MKDIR) $@
$(shareDir):
	$(MKDIR) $@
$(docDir):
	MKDIR) $@

