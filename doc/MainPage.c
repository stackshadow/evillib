/* Documentation
	Copyright (C) 2012-2014 by Martin Langlotz

	This file is part of evillib.

	evillib is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	evillib is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with evillib.  If not, see <http://www.gnu.org/licenses/>.
*/



/**
@~english
@mainpage EvilLib - A simple/small/stable Library
@image html evillib-logo.png

@~
 @author Stackshadow ( Martin Langlotz )
 @author Brainweasel

@~english
## Idea
The evillib is a small library which is created and grown by our projects, or from the people out there. \n
Goal of the evillib is to provide an easy to use, clear, and powerfull toolkit, according to the K.I.S.S - principle

@defgroup compile How to compile
## First step

### Download
#### github
You can clone or download the evillib from github

#### evilbrain


### Compile
#### Preconditions
- a c-compiler
- c-library
- libblkid for Block-Device support
- libcryptsetup For cryptsetup support
@todo add precompiler flags to enable/disable Blockdevice / cryptsetup support

#### Compile
- to compile the library cd to source
- use \code{.cpp} make help \endcode to get the targets
- use \code{.cpp} make evillib \endcode to build the evillib and \code{.cpp}make install \endcode to install
to see how you can change the default compile settings of the evillib, see @ref compileOptions
*/
/** @defgroup compileOptions Compile options
@ingroup compile

##### ET_DEBUG_OFF
@copydoc ET_DEBUG_OFF

##### ET_SECURE_MEMORY_OFF
@copydoc ET_SECURE_MEMORY_OFF

*/



/** Core Functions
@~english


@defgroup grMemory Memory functions
@brief Functions to handle Memory

@defgroup grCore Core - Functions
@brief Core Functions
These are the core functions of the evillib



 */




// Here are definitions which are needed in the doc
#define ET_DEBUG_OFF
#define ET_SECURE_MEMORY_OFF
