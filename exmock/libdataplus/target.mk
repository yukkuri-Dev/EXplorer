BUILDDIR := build/

TARGETS := _libdataplus _libsh4a _libgraphics

OBJDIRS := $(addprefix $(BUILDDIR),$(TARGETS))


INSTALL_TARGET := libdataplus

# Define the rules to build in the target subdirectories.
#
MAKETARGET = $(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile \
		SRCDIR=$(CURDIR)/src

all: $(OBJDIRS)

.PHONY: $(OBJDIRS)
$(OBJDIRS):
	@echo "Building $@..."
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKETARGET) TARGET=$(patsubst _%,%,$(notdir $@))

# These rules keep make from trying to use the match-anything rule below to
# rebuild the makefiles--ouch!  Obviously, if you don't follow my convention
# of using a `.mk' suffix on all non-standard makefiles you'll need to change
# the pattern rule.
#
Makefile : ;
%.mk :: ;


# Anything we don't know how to build will use this rule.  The command is a
# do-nothing command, but the prerequisites ensure that the appropriate
# recursive invocations of make will occur.
#
% :: $(OBJDIRS) ;


# The clean rule is best handled from the source directory: since we're
# rigorous about keeping the target directories containing only target files
# and the source directory containing only source files, `clean' is as trivial
# as removing the target directories!
#
.PHONY: clean
clean:
	@echo clean ...
	@rm -rf $(BUILDDIR)

.PHONY: install
install: $(OBJDIRS)
	@mkdir -p $(DEVKITPRO)/$(INSTALL_TARGET)/lib
	@cp -frv include $(DEVKITPRO)/$(INSTALL_TARGET)
	@cp -frv $(shell find $(BUILDDIR) -name '*.a') $(DEVKITPRO)/$(INSTALL_TARGET)/lib
