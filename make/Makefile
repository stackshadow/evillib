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
	@echo "$(COK)Welcome to the evillib-build-system $(CNormal)"
	@echo ""
	@#$(MAKE) --no-print-directory -f make/evillib-core.make $@
	@#$(MAKE) --no-print-directory -f make/evillib-extra-dev.make $@
	@#$(MAKE) --no-print-directory -f make/evillib-extra.make $@
	@#$(MAKE) --no-print-directory -f make/evillib-doc.make $@
	@$(MAKE) --no-print-directory -f make/evillib-app.make $@
	@echo -n "$(CNormal)"

touch:
	find .. -exec touch {} \;

debian/libevillib-core/DEBIAN/control:
	parts="core" dpkg-buildpackage -us -uc -b
evillib-core-debian: debian/libevillib-core/DEBIAN/control

debian/libevillib-extra/DEBIAN/control:
	parts="extra" dpkg-buildpackage -us -uc -b
evillib-extra-debian: debian/libevillib-extra/DEBIAN/control
	@echo "done"

