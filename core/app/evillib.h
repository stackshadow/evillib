/* Example functions
	Copyright (C) 2015  by Martin Langlotz alias stackshadow

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


int etBENCHMARK_MULTIPLIKATOR;
int etBENCHMARK_MALLOC_SIZE;
int etBENCHMARK_REALLOC_SIZE;

void etApicheckYES( int Return );

void etApicheckCheckMemoryLeak( struct mallinfo MallinfoOld );

void etApicheckPrintMemory( struct mallinfo MallinfoOld );

void etApicheckPrintMalloced( struct mallinfo MallinfoOld );



etID_STATE		APICHECK_etMemory();
etID_STATE		APICHECK_etList();
etID_STATE		APICHECK_etString();
void			APICHECK_etBlkDevice();


