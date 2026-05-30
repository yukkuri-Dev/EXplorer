CC = gcc
SDL2_FLAGS = $(shell sdl2-config --cflags --libs)

LIBDP_SRCS = $(shell find ../exmock/libdataplus/src -name "*.c")
LIBDP_EXCLUDE = lcdc.c init.c keypad.c console.c sbrk.c system.c malloc.c
LIBDP_FILTERED = $(filter-out $(addprefix %,$(LIBDP_EXCLUDE)), $(LIBDP_SRCS))

SRCS  = ../src/main.c
SRCS += mock_syscall.c
SRCS += mock_memmgr.c
SRCS += mock_main.c
SRCS += $(shell find ../src/libct -name "*.c")
SRCS += $(LIBDP_FILTERED)

CFLAGS  = -DEXWORD_MOCK
CFLAGS += -Dmain=_ew_main
CFLAGS += -include mock_syscall.h
CFLAGS += -I../src
CFLAGS += -I../exmock/libdataplus/include

ifdef ENABLE_SD2
CFLAGS += -DMOCK_SD2
endif

GEN_VERSION_H = ../src/version_generated.h
BUILD_VERSION = mock
BUILD_GIT_HASH = $(shell git rev-parse --short HEAD 2>/dev/null || echo "unknown")

all: exmock

$(GEN_VERSION_H):
	@mkdir -p ../src
	@echo "#ifndef VERSION_H" > $@
	@echo "#define VERSION_H" >> $@
	@echo "" >> $@
	@echo "#define BUILD_VERSION \"$(BUILD_VERSION)\"" >> $@
	@echo "#define BUILD_GIT_HASH \"$(BUILD_GIT_HASH)\"" >> $@
	@echo "" >> $@
	@echo "#endif" >> $@



exmock: $(GEN_VERSION_H) $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(SDL2_FLAGS) -o exmock

clean:
	rm -f exmock