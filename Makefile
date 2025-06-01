#
# Makefile for tonclib.
#

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif
include $(DEVKITARM)/gba_rules

BUILD		:=	build
SRCDIRS		:=	asm src src/font src/tte src/pre1.3
INCDIRS		:=	include
DATADIRS	:=	data

DATESTRING	:=	$(shell date +%Y)$(shell date +%m)$(shell date +%d)

ARCH		:=	-mthumb -mthumb-interwork
RARCH		:= -mthumb-interwork -mthumb
IARCH		:= -mthumb-interwork -marm

bTEMPS		:= 0	# Save g++ temporaries (.i and .s files)
bDEBUG2		:= 0	# Generate debug info (bDEBUG2? Not a full DEBUG flag. Yet)

VERSION		:=	1.4.3

#---------------------------------------------------------------------------------
# Options for code generation
#---------------------------------------------------------------------------------

CXXBASE   := $(INCLUDE) -Wall -fno-strict-aliasing #-fno-tree-loop-optimize
CXXBASE	+= -Ofast

RCFLAGS := $(CXXBASE) $(RARCH) -Wno-pointer-arith
ICFLAGS := $(CXXBASE) $(IARCH) -mlong-calls -fno-gcse -Wno-pointer-arith
CXXFLAGS  := $(RCFLAGS)

ASFLAGS := $(INCLUDE) -Wa,--warn $(ARCH)

# --- Save temporary files ? ---
ifeq ($(strip $(bTEMPS)), 1)
	RCFLAGS  += -save-temps
	ICFLAGS  += -save-temps
	CFLAGS	 += -save-temps
	CXXFLAGS += -save-temps
endif

# --- Debug info ? ---

ifeq ($(strip $(bDEBUG2)), 1)
	CFLAGS	+= -g
	LDFLAGS	+= -g
endif

#---------------------------------------------------------------------------------
# Path to tools - this can be deleted if you set the path in windows
#---------------------------------------------------------------------------------

export PATH		:=	$(DEVKITARM)/bin:$(PATH)

#---------------------------------------------------------------------------------

ifneq ($(BUILD),$(notdir $(CURDIR)))

export TARGET	:=	$(CURDIR)/lib/libtonc.a

export VPATH	:=	$(foreach dir,$(DATADIRS),$(CURDIR)/$(dir)) $(foreach dir,$(SRCDIRS),$(CURDIR)/$(dir))

ICFILES		:=	$(foreach dir,$(SRCDIRS),$(notdir $(wildcard $(dir)/*.iwram.cpp)))
RCFILES		:=	$(foreach dir,$(SRCDIRS),$(notdir $(wildcard $(dir)/*.cpp)))
CXXFILES		:=  $(ICFILES) $(RCFILES)

SFILES		:=	$(foreach dir,$(SRCDIRS),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATADIRS),$(notdir $(wildcard $(dir)/*.*)))

export OFILES	:=	$(addsuffix .o,$(BINFILES)) $(CXXFILES:.cpp=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCDIRS),-I$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/build

.PHONY: $(BUILD) clean docs re

$(BUILD):
	@[ -d lib ] || mkdir -p lib
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

docs:
	doxygen libtonc.dox

clean:
	@echo clean ...
	@rm -fr $(BUILD)

install:
	@mkdir -p $(DESTDIR)$(DEVKITPRO)/libtonc/lib
	@cp -rv include $(DESTDIR)$(DEVKITPRO)/libtonc/include
	@cp -v lib/libtonc.a $(DESTDIR)$(DEVKITPRO)/libtonc/lib/

re: clean $(BUILD)

#-------------------------------------------------------------------------------
dist:
#-------------------------------------------------------------------------------
	@tar -cvjf libtonc-src-$(VERSION).tar.bz2 asm src include \
		Makefile todo.txt libtonc.dox base.cpp base.h

#---------------------------------------------------------------------------------

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------

%.a :

$(TARGET): $(OFILES)

%.a : $(OFILES)
	@echo Building $@
	@rm -f $@
	@$(AR) -crs $@ $^
	$(PREFIX)nm -Sn $@ > $(basename $(notdir $@)).map

%.iwram.o : %.iwram.c
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$(@:.o=.d) $(ICFLAGS) -c $< -o $@

%.o : %.c
	@echo $(notdir $<)
	$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(RCFLAGS) -c $< -o $@

-include $(DEPENDS)

endif

#---------------------------------------------------------------------------------
