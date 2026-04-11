.SUFFIXES:

ifeq ($(strip $(DEVKITSH4)),)
$(error "Please set DEVKITSH4 in your environment. export DEVKITSH4=<path to sdk>")
endif
include $(DEVKITSH4)/exword_rules

TARGET       := filem
MODNAME      := filemgr
APPTITLE     := File Manager Beta
APPID        := FILEB
APPMOD       := $(TARGET).d01
BUILD_VERSION := 2.0.2
BUILD_GIT_HASH := $(shell git describe --tags --always)
GEN_VERSION_H := src/version_generated.h

SOURCEDIR    := src
HTMLDIR      := html
INSTALLDIR   := $(HOME)/.local/share/exword
BUILDS       := ja cn
EXCLUDE      :=
CFILES       := $(filter-out $(EXCLUDE),$(wildcard $(SOURCEDIR)/*.c)) $(wildcard $(SOURCEDIR)/libc/*.c) $(wildcard $(SOURCEDIR)/libct/*.c) $(wildcard $(SOURCEDIR)/libct/fsc/*.c) $(SOURCEDIR)/libct/ui/dialog/user_input_dialog.c $(SOURCEDIR)/libct/ui/dialog/user_select_dialog.c $(SOURCEDIR)/libct/fonts/jpn-font.c $(SOURCEDIR)/libct/ui/dialog/system_storage_info.c $(SOURCEDIR)/libct/ui/dialog/file_copy.c $(SOURCEDIR)/libct/ui/fake_UI/Binary_view.c
SFILES       := $(wildcard $(SOURCEDIR)/*.s) $(wildcard $(SOURCEDIR)/libc/*.s)
OBJECTS      := $(CFILES:.c=.o) $(SFILES:.s=.o)

CC_OPTS      :=
LDFLAGS      := -Wall -std=gnu17 -nostdlib -L$(DEVKITPRO)/libdataplus/lib -ldataplus -lgraphics -lsh4a -lgcc
CFLAGS       := -Wall -std=gnu17 -fno-builtin -I$(DEVKITPRO)/libdataplus/include -I$(SOURCEDIR) -I$(SOURCEDIR)/libc/include -O3 $(CC_OPTS)
ASFLAGS      := -Wall -std=gnu17 -m4-nofpu

app: $(GEN_VERSION_H) $(addprefix build/,$(addsuffix /$(APPID),$(BUILDS)))

# バージョン情報ヘッダ生成
$(GEN_VERSION_H): FORCE
	@mkdir -p src
	@echo "#ifndef VERSION_H" > $@
	@echo "#define VERSION_H" >> $@
	@echo "" >> $@
	@echo "#define BUILD_VERSION \"$(BUILD_VERSION)\"" >> $@
	@echo "#define BUILD_GIT_HASH \"$(BUILD_GIT_HASH)\"" >> $@
	@echo "" >> $@
	@echo "#endif" >> $@

.SECONDEXPANSION:
build/%/$(APPID): $(TARGET).d01 $$(wildcard $(HTMLDIR)/$$*/*.htm)
	@echo building $* version in $@...
	@mkdir -p $@
	@cp $(TARGET).d01 $@
	@for f in $(HTMLDIR)/$*/*.htm; do \
		sed -e 's/@APPTITLE/$(APPTITLE)/g' -e 's/@APPID/$(APPID)/g' -e 's/@APPMOD/$(APPMOD)/g' $$f > $@/$$(basename $$f); \
	done
	@touch $@/fileinfo.cji

$(TARGET).elf: $(OBJECTS)

install: app
	@echo installing to $(INSTALLDIR)...
	@mkdir -p $(INSTALLDIR)
	@cp -r build/* $(INSTALLDIR)/
	@echo 'You can now install this app to EX-word by `dict install $(APPID)` in libexword.'

clean:
	@echo clean $(OBJECTS) $(TARGET).elf $(TARGET).elf.map $(TARGET).d01 $(GEN_VERSION_H)
	@rm -fr build $(OBJECTS) $(TARGET).elf $(TARGET).elf.map $(TARGET).d01 $(GEN_VERSION_H)

FORCE:

.PHONY: app install clean FORCE