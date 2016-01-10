
ifneq ($(VARS),1)
	include make/vars.make
endif

include evillib-version.make

sourcePath=$(sourceBasePath)/extra
buildPath=$(buildBasePath)/extra

sources = $(shell cd $(sourcePath) && find . -name "*.c" ! -wholename "*specialheaders/*" ! -wholename "*develop/*" ! -wholename "*tests/*" -printf "%p " )
sourcesFull=$(addprefix $(sourcePath)/,$(sources))

headers=$(sources:.c=.h)
headersFull=$(addprefix $(sourcePath)/,$(headers))

.DEFAULT:
	@sleep 0


help:
	@echo -n "$(CComment)"
	@echo "##################### Core Development Headers #####################"
	@echo "$(MAKE) evillib-extra-dev: Build development files ( in $(includeDir)/evillib-extra.h )"
	@echo -n "$(CNormal)"

clean:
	@$(RM) $(buildPath)/libevillib-extra.$(Version).h
	@$(RM) $(includeDir)/evillib/libevillib-extra.$(Version).h
	@$(RM) $(includeDir)/evillib-extra.h

install: evillib-extra-install

#################################### Development / Headers ####################################
evillib-extra: evillib-extra-dev
evillib-extra-dev: $(includeDir) $(buildPath)/libevillib-extra.$(Version).h
evillib-extra-dev-install: $(includeDir)/evillib-extra.h

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

