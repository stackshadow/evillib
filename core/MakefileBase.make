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

# Export all variables
export

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
sourcePath=$(PWD)
buildPath=/tmp/build/evillib
prefix=$(buildPath)/target

includeDir=$(prefix)/include
binDir=$(prefix)/bin
libDir=$(prefix)/lib
shareDir=$(prefix)/share
docDir=$(prefix)/share/doc/evillib/${Version}


# Version of evillib
VerMajor=00
VerMinor=14
VerPatch=04
Version=$(VerMajor).$(VerMinor)-$(VerPatch)
