
include evillib-version.make
include evillib-extra-sources.make

headers=$(sources:.c=.h)
headersFull=$(addprefix $(sourcePath)/,$(headers))


.DEFAULT:
	@sleep 0


help:
	@echo -n "$(CComment)"
	@echo "##################### Core Development Headers #####################"
	@echo "$(MAKE) evillib-extra-dev: Build development files ( in $(includeDir)/evillib-extra.h )"
	@echo -n "$(CNormal)"

#################################### Development / Headers ####################################
evillib-extra: $(buildPath)/libevillib-extra.$(Version).h
evillib-extra-dev: $(includeDir) $(includeDir)/evillib-extra.h
evillib-extra-clean:
	@$(RM) $(buildPath)/libevillib-extra.$(Version).h
	@$(RM) $(includeDir)/evillib/libevillib-extra.$(Version).h
	@$(RM) $(includeDir)/evillib-extra.h

$(buildPath)/libevillib-extra.$(Version).h: 
	@echo "${CCommand}make $@ ${CNormal}"
	$(MKDIR) $(buildPath)
	cat $(sourcePath)/evillib-extra_start.h > $@
	cat $(sourcePath)/evillib-extra_depends.h >> $@
	cat $(headersFull) >> $@
	cat $(sourcePath)/evillib-extra_end.h >> $@

# Output directory
$(includeDir)/evillib-extra.h: $(buildPath)/libevillib-extra.$(Version).h
	@mkdir -p $(includeDir)/evillib/
	@$(CP) $< $(includeDir)/evillib/libevillib-extra.$(Version).h
	@$(LN) evillib/libevillib-extra.$(Version).h $@

