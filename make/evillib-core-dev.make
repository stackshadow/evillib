
include evillib-version.make
include evillib-core-sources.make

headerVersion=$(buildPath)/evillib-core_version.h

headers=$(sources:.c=.h)
headersFull=$(addprefix $(sourcePath)/,$(headers))

.DEFAULT:
	@sleep 0

help:
	@echo -n "$(CComment)"
	@echo "##################### Core Development Headers #####################"
	@echo "$(MAKE) evillib-core-dev: Build development files ( in $(includeDir)/evillib.h )"
	@echo -n "$(CNormal)"

clean:
	@$(RM) $(headerVersion)
	@$(RM) $(buildPath)/evillib.$(Version).h

install: evillib-core-dev-install

#################################### Development / Headers ####################################
evillib-core: $(buildPath)/evillib.$(Version).h
evillib-core-dev: $(buildPath)/evillib.$(Version).h
evillib-core-dev-install: $(includeDir)/evillib.h
evillib-core-dev-uninstall:
	@$(RM) $(includeDir)/evillib/evillib.$(Version).h
	@$(RM) $(includeDir)/evillib.h


$(headerVersion): 
	@echo "${CCommand}make $@ ${CNormal}"
	$(MKDIR) $(buildPath)
	@echo "// Evillib - Version" > $@
	@echo "/**	@defgroup grCoreDefineVersion Versions" >> $@
	@echo "@ingroup grCoreDefines" >> $@
	@echo "*/" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "The major version of the evillib */" >> $@
	@echo "#define ET_VERSION_MAJOR $(VerMajor)" >> $@
	@echo "" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "If this is changed, a recompilation is needed */" >> $@
	@echo "#define ET_VERSION_MINOR $(VerMinor)" >> $@
	@echo "" >> $@

	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "If this is changed, no recompile needed */" >> $@
	@echo "#define ET_VERSION_PATCH $(VerPatch)" >> $@
	
	@echo "" >> $@
	@echo "/** @ingroup grCoreDefineVersion" >> $@
	@echo "The versionsstring */" >> $@
	@echo "#define ET_VERSIONSTRING \"$(VerMajor).$(VerMinor)-$(VerPatch)\"" >> $@
	
	@echo "" >> $@

$(buildPath)/evillib.$(Version).h: $(headerVersion)
	@echo "${CCommand}make $@ ${CNormal}"
	$(MKDIR) $(buildPath)
	cat $(sourcePath)/evillib_start.h > $@
	cat $(sourcePath)/evillib_depends.h >> $@
	cat $(headerVersion) >> $@
	cat $(sourcePath)/evillib_defines.h >> $@
	cat $(headersFull) >> $@
	cat $(sourcePath)/evillib_end.h >> $@


# Output directory
$(includeDir)/evillib.h: $(buildPath) $(buildPath)/evillib.$(Version).h
	@echo "${CCommand}make $@ ${CNormal}"
	@mkdir -p $(includeDir)/evillib/
	@$(CP) $(buildPath)/evillib.$(Version).h $(includeDir)/evillib/	
	@$(LN) evillib/evillib.$(Version).h $@
