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

/** Programming infos
@ingroup gr_standards
@~english
@defgroup gr_versioning Versioning of the evillib

The Versioning of the evillib gives you information about what you must do when somethings changed. \n
The version is splittet into three parts: \n
AAA.BBB-CCC\n

*- AAA(NUMERIC): This is for Major versioning. \n
If to much changes are done, a major release will be provided. \n

*- BBB(NUMERIC): This change improve the evillib with new functions \n
If some present defines/enumerations/functions are changed, its possible to recompile and review your code. \n
If new functions are added, this part of the version will not be changed, because its not disturbes the actual Program. \n

*- CCC(NUMERIC): If this change, you dont need to do something with the Program that depends on the evillib. \n
That means present defines/enumerations/functions are not changed. \n
If some fixes inside the Functions are made, this part of the versioning is incremented. But therefore you dont need to recompile your code \n


*/

