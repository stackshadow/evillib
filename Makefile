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

# Colours
CNormal=\033[00m
CCommand=\033[1;35m
CComment=\033[36m
COK=\033[32m

# Compiler
CC = ${CROSSCOMPILE}gcc
CXX = ${CROSSCOMPILE}g++
AR = ${CROSSCOMPILE}ar
RANLIB = ${CROSSCOMPILE}ranlib
RM = rm -v -f
CP = cp -v -f
LN = ln -v -f -s
MKDIR = mkdir -p -v
RMDIR = rmdir -v

# Directorys
prefix=/usr
export sourceDir=$(PWD)
export buildDir=$(PWD)/.build
buildDir=/tmp/.build
targetDir=$(prefix)
includeDir=$(targetDir)/include
binDir=$(targetDir)/bin
libDir=$(targetDir)/lib
docDir=$(targetDir)/share/doc/evillib/${Version}
outputDir=$(buildDir)/deploy

# Version of evillib
VerMajor=00
VerMinor=14
VerPatch=04
Version=$(VerMajor).$(VerMinor)-$(VerPatch)

.EXPORT_ALL_VARIABLES:


.DEFAULT:
	@echo "$(COK)Welcome to the evillib-build-system $(CNormal)"
	@echo ""
	$(MAKE) --directory=$(sourceDir)/core $@
	$(MAKE) --directory=$(sourceDir)/doc $@

.PHONY:



