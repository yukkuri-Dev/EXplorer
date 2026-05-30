.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitpro")
endif

ifeq ($(strip $(DEVKITSH4)),)
$(error "Please set DEVKITSH4 in your environment. export DEVKITSH4=<path to>devkitSH4")
endif


ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else

include $(DEVKITSH4)/exword_rules

INCLUDES	:= -I$(CURDIR)/../../include

CFLAGS  :=	$(INCLUDES) -m4-nofpu -O2 -ffunction-sections
ASFLAGS :=	-m4-nofpu

ifeq (libdataplus,$(TARGET))

VPATH :=	$(SRCDIR)/util \
		$(SRCDIR)/util/fixedptc \
		$(SRCDIR)/syscall \
		$(SRCDIR)/libc

UTIL	   	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/util/*.c)))
FIXEDPT	   	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/util/fixedptc/*.c)))
LIBC	   	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/libc/*.c)))
SYSCALL	   	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/syscall/*.c))) _exit.o
OBJECTS :=	$(UTIL) $(FIXEDPT) $(SYSCALL) $(LIBC)

else
ifeq (libsh4a,$(TARGET))

VPATH :=	$(SRCDIR)/cpu/sh4a \
		$(SRCDIR)/cpu/sh4a/input \

SH4A	   	:= $(patsubst %.s,%.o, $(notdir $(wildcard $(SRCDIR)/cpu/sh4a/*.s)))
INPUTDEV	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/cpu/sh4a/input/*.c)))
OBJECTS :=	$(SH4A) $(INPUTDEV)

else
ifeq (libgraphics,$(TARGET))

VPATH :=	$(SRCDIR)/graphics \

GRAPHICS	:= $(patsubst %.c,%.o, $(notdir $(wildcard $(SRCDIR)/graphics/*.c)))
OBJECTS :=	$(GRAPHICS)

endif
endif
endif

$(TARGET).a: $(OBJECTS)

endif
