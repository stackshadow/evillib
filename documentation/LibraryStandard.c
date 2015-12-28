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

/** Library standard
@~english
@defgroup gr_standards Evillib standards
@{

\n
\n

@section standard_description Descriptions



@subsection standardFunctions Standard Functions


Every Object ( means etList, etString, etc. ) have standard functions.
These functions are describet here

*- <Object-Name>_alloc: This will allocate an new Object inside the memory and init it. \n

*- <Object-Name>_init: This initialisize an STATIC object \n
Be careful to use this function twice, some function will allocate memory internal. So if you call this function again,\n
the pointer to this memory get lost. This will cause a memory leak. \n
If you need to reuse the same object, you can clean it out.
So this functions are mainly for static objects. Means if you use temporary objects in your functions, its not needed to allocate
memory global.

*- <Object-Name>_clean: This will clean out everything inside the object, but the object itselfe is not freed. \n
This will not free any memory, only set it to initial state.

*- <Object-Name>_destroy: Acts like <Object-Name>_clean but frees the internal memory

*- <Object-Name>_free: This will free an allocated Object and set it to NULL! \n
Dont use this function on a static object, this will crash your application and burn out your brain ;)


Every function return a @ref etID_STATE


@defgroup gr_internal_function Internal Functions
@{
    @anchor internal_function Internal Functions
    @warning Internal functions should not used in your program
    This functions are called inside the evillib
@}


@}


*/

