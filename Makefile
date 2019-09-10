#!/usr/bin/make
#
# The Core Library Makefile (libcore)
#
#########################################################################

# Identity of this package.
PACKAGE_NAME			= libcore
PACKAGE_TARNAME			= libcore
PACKAGE_VERSION			= 0.2
PACKAGE_STRING			= $(PACKAGE_NAME) $(PACKAGE_VERSION)
PACKAGE_BUGREPORT		= support@lavabit.com
PACKAGE_URL				= https://lavabit.com

TOPDIR					= $(realpath .)

#  Makefile:15:  [3] (buffer) realpath:
#  This function does not protect against buffer overflows, and some
#  implementations can overflow internally (CWE-120/CWE-785!). Ensure that the
#  destination buffer is at least of size MAXPATHLEN, andto protect against
#  implementation problems, the input argument should also be checked to
#  ensure it is no larger than MAXPATHLEN.

LIBCORE_CHECK_SRCDIR	= check
LIBCORE_CHECK_PROGRAM	= core.check$(EXEEXT)
LIBCORE_CHECK_INCLUDES	= -Icheck -Isrc/core

LIBCORE_SRCDIR			= src
LIBCORE_SHARED			= libcore$(DYNLIBEXT)
LIBCORE_STATIC			= libcore$(STATLIBEXT)
LIBCORE_PROGRAMS		=

LIBCORE_OBJFILES		= $(call OBJFILES, $(call SRCFILES, src check)) $(call OBJFILES, $(call CPPFILES, src check))
LIBCORE_DEPFILES		= $(call DEPFILES, $(call SRCFILES, src check)) $(call DEPFILES, $(call CPPFILES, src check))
LIBCORE_STRIPPED		= libcore-stripped$(STATLIBEXT) libcore-stripped$(DYNLIBEXT)
LIBCORE_DEPENDENCIES	=

LIBCORE_REPO			= $(shell which git &> /dev/null && git log &> /dev/null && echo 1)
ifneq ($(strip $(LIBCORE_REPO)),1)
	LIBCORE_VERSION			:= $(PACKAGE_VERSION)
	LIBCORE_COMMIT			:= "NONE"
else
	LIBCORE_VERSION			:= $(PACKAGE_VERSION).$(shell git log --format='%H' | wc -l)
	LIBCORE_COMMIT			:= $(shell git log --format="%H" -n 1 | cut -c33-40)
endif
LIBCORE_TIMESTAMP		= $(shell date +'%Y%m%d.%H%M')

# Dependency Files
DEPDIR					= .deps
DEPFILES				= $(patsubst %.cpp, $(DEPDIR)/%.d, $(patsubst %.cc, $(DEPDIR)/%.d, $(patsubst %.c, $(DEPDIR)/%.d, $(1))))

# Object Files
OBJDIR					= .objs
OBJFILES				= $(patsubst %.cpp, $(OBJDIR)/%.o, $(patsubst %.cc, $(OBJDIR)/%.o, $(patsubst %.c, $(OBJDIR)/%.o, $(1))))

# Source Files
SRCDIRS					= $(shell find $(1) -type d -print)
CCFILES					= $(foreach dir, $(call SRCDIRS, $(1)), $(wildcard $(dir)/*.cc))
CPPFILES				= $(foreach dir, $(call SRCDIRS, $(1)), $(wildcard $(dir)/*.cpp))
SRCFILES				= $(foreach dir, $(call SRCDIRS, $(1)), $(wildcard $(dir)/*.c))

# Setup the Defines
DEFINES					+= -D_REENTRANT -DFORTIFY_SOURCE=2 -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DHAVE_NS_TYPE -DMAGMA_PEDANTIC -DCORE_VERSION=\"$(LIBCORE_VERSION)\" -DCORE_COMMIT=\"$(LIBCORE_COMMIT)\" -DCORE_TIMESTAMP=\"$(LIBCORE_TIMESTAMP)\"

INCLUDES				= -Ilib/local/include -I/usr/include -Isrc -Icheck/core -Isrc/core -Icheck
WARNINGS				= -Wfatal-errors -Werror -Wall -Wextra -Wformat-security -Warray-bounds  -Wformat=2 -Wno-format-nonliteral

# C Compiler
ifeq ($(patsubst undefined,default,$(origin CC)),default)
CC  = gcc
endif

# C++ Preprocessor
ifeq ($(patsubst undefined,default,$(origin CPP)),default)
CPP  = gcc
endif

# C++ Compiler
ifeq ($(patsubst undefined,default,$(origin CXX)),default)
CXX  = gcc
endif

# Linker
ifeq ($(patsubst undefined,default,$(origin LD)),default)
LD  = gcc
endif

# Archiver
ifeq ($(patsubst undefined,default,$(origin AR)),default)
AR  = ar
endif

# Symbol Stripper
ifeq ($(patsubst undefined,default,$(origin STRIP)),default)
STRIP  = strip
endif

# File Installer
ifeq ($(patsubst undefined,default,$(origin INSTALL)),default)
INSTALL  = install
endif

# Archive Symbol Updater
ifeq ($(patsubst undefined,default,$(origin RANLIB)),default)
RANLIB  = ranlib
endif

# Verbosity Control
ifeq ($(patsubst undefined,default,$(origin VERBOSE)),default)
VERBOSE  = no
endif

# Quick Dependency Builds
ifeq ($(patsubst undefined,default,$(origin QUICK)),default)
QUICK  = yes
endif

# Compiler Parameters
CFLAGS							?=
CFLAGS_WARNINGS			= -Wall -Werror -Winline -Wformat-security -Warray-bounds
CFLAGS_PEDANTIC			= -Wextra -Wpacked -Wunreachable-code -Wformat=2
CFLAGS_COMBINED			= -std=gnu99 -O0 -fPIC -fmessage-length=0 -ggdb3 -c $(CFLAGS_WARNINGS) -MMD $(CFLAGS)

CPPFLAGS						?=
CPPFLAGS_WARNINGS		= -Werror -Wall -Wextra -Wformat=2 -Wwrite-strings -Wno-format-nonliteral
CPPFLAGS_COMBINED		= -std=c++0x $(CPPFLAGS_WARNINGS) -Wno-unused-parameter -pthread -g3 $(CPPFLAGS)

# Linker Options
LDFLAGS                      ?=
LDFLAGS_COMBINED              = -rdynamic $(LDFLAGS)

# Archiver Options
ARFLAGS                      ?= rcs

# Strip Options
STRIPFLAGS                   ?= --strip-debug

# GProf Options
GPROF                        ?= -pg -finstrument-functions -fprofile-arcs -ftest-coverage

# PProf Options
PPROF                        ?= -lprofiler

# Miscellaneous External programs
MV                            = mv --force
RM                            = rm --force
RMDIR                         = rmdir --parents --ignore-fail-on-non-empty
MKDIR                         = mkdir --parents

# Control the Text Color/Weight if the TERM supports it. If no TERM is available, then
# default to using vt100 as the terminal type.
ifdef TERM
  RED                           = $$(tput setaf 1 || true)
  BLUE                          = $$(tput setaf 4 || true)
  GREEN                         = $$(tput setaf 2 || true)
  WHITE                         = $$(tput setaf 7 || true)
  YELLOW                        = $$(tput setaf 3 || true)
  BOLD                          = $$(tput bold || true)
  NORMAL                        = $$(tput sgr0 || true)
else
  RED                           = $$([[ -t 0 ]] && tput -Tvt100 setaf 1 || true)
  BLUE                          = $$([[ -t 0 ]] && tput -Tvt100 setaf 4 || true)
  GREEN                         = $$([[ -t 0 ]] && tput -Tvt100 setaf 2 || true)
  WHITE                         = $$([[ -t 0 ]] && tput -Tvt100 setaf 7 || true)
  YELLOW                        = $$([[ -t 0 ]] && tput -Tvt100 setaf 3 || true)
  BOLD                          = $$([[ -t 0 ]] && tput -Tvt100 bold || true)
  NORMAL                        = $$([[ -t 0 ]] && tput -Tvt100 sgr0 || true)
endif

ifeq ($(VERBOSE),yes)
RUN                           =
else
RUN                           = @
endif

# So we can tell the user what happened
ifdef MAKECMDGOALS
TARGETGOAL                   += $(MAKECMDGOALS)
else
TARGETGOAL                   = $(.DEFAULT_GOAL)
endif

ifeq ($(OS),Windows_NT)
  HOSTTYPE                   := "Windows"
  LIBPREFIX                  :=
  DYNLIBEXT                  := ".dll"
  STATLIBEXT                 := ".lib"
  EXEEXT                     := ".exe"

  # Alias the target names on Windows to the equivalent without the exe extension.
  $(basename $(DIME_PROGRAM)) := $(DIME_PROGRAM)
  
else
  HOSTTYPE                 := $(shell uname -s)
  LIBPREFIX                := lib
  DYNLIBEXT                := .so
  STATLIBEXT               := .a
  EXEEXT                   :=
endif

# We only build the unit tests as part of 'all' if libcheck is installed.
ifeq ($(shell pkg-config --silence-errors --atleast-version=0.9.8 check && echo 1),1)
ALL = $(LIBCORE_SHARED) $(LIBCORE_STATIC) $(LIBCORE_PROGRAMS) $(LIBCORE_STRIPPED) $(LIBCORE_CHECK_PROGRAM)
else
ALL = $(LIBCORE_SHARED) $(LIBCORE_STATIC) $(LIBCORE_PROGRAMS) $(LIBCORE_STRIPPED)
endif

all: config warning $(ALL) finished

stripped: config warning $(LIBCORE_STRIPPED) finished

check: config warning $(LIBCORE_CHECK_PROGRAM)
	@./$(LIBCORE_CHECK_PROGRAM)
ifeq ($(VERBOSE),no)
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)
endif

warning: config
ifeq ($(VERBOSE),no)
	@echo
	@echo 'For a more verbose output'
	@echo '  make '$(GREEN)'VERBOSE=yes' $(NORMAL)$(TARGETGOAL)
	@echo
endif

config:
ifeq ($(strip $(LIBCORE_REPO)),1)
	@git update-index --assume-unchanged .project
	@git update-index --assume-unchanged .cproject
endif
	@echo
	@echo 'TARGET' $(TARGETGOAL)
	@echo 'VERBOSE' $(VERBOSE)
	@echo
	@echo 'VERSION ' $(LIBCORE_VERSION)
	@echo 'COMMIT '$(LIBCORE_COMMIT)
	@echo 'DATE ' $(LIBCORE_TIMESTAMP)
	@echo 'HOST ' $(HOSTTYPE)

finished:
ifeq ($(VERBOSE),no)
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)
endif

# Alias the target names on Windows to the equivalent target without the exe extension.
ifeq ($(HOSTTYPE),Windows)

$(basename %): $(LIBCORE_PROGRAMS)

endif

# Delete the compiled program along with the generated object and dependency files
clean:
	$(RUN)$(RM) $(LIBCORE_PROGRAMS) $(LIBCORE_STRIPPED) $(LIBCORE_CHECK_PROGRAM)
	$(RUN)$(RM) $(LIBCORE_SHARED) $(LIBCORE_STATIC)
	$(RUN)$(RM) $(LIBCORE_OBJFILES) $(LIBCORE_DEPFILES)
	@for d in $(sort $(dir $(LIBCORE_OBJFILES))); do if test -d "$$d"; then $(RMDIR) "$$d"; fi; done
	@for d in $(sort $(dir $(LIBCORE_DEPFILES))); do if test -d "$$d"; then $(RMDIR) "$$d"; fi; done
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)

distclean:
	$(RUN)$(RM) $(LIBCORE_PROGRAMS) $(LIBCORE_STRIPPED) $(LIBCORE_CHECK_PROGRAM)
	$(RUN)$(RM) $(LIBCORE_SHARED) $(LIBCORE_STATIC)
	$(RUN)$(RM) $(LIBCORE_OBJFILES) $(LIBCORE_DEPFILES)
	@$(RM) --recursive --force $(DEPDIR) $(OBJDIR)
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)

$(LIBCORE_DEPENDENCIES): 

$(LIBCORE_STRIPPED): $(LIBCORE_SHARED) $(LIBCORE_STATIC) $(LIBCORE_PROGRAMS)
ifeq ($(VERBOSE),no)
	@echo 'Creating' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(STRIP) $(STRIPFLAGS) --output-format=$(shell objdump -p "$(subst -stripped,,$@)" | grep "file format" | head -1 | \
	awk -F'file format' '{print $$2}' | tr --delete [:space:]) -o "$@" "$(subst -stripped,,$@)"

# Construct the libcore check executable.
$(LIBCORE_CHECK_PROGRAM): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(call CPPFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call CCFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call SRCFILES, $(LIBCORE_CHECK_SRCDIR))) $(LIBCORE_STATIC)
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS_COMBINED) --output='$@' $(call OBJFILES, $(call CPPFILES, $(LIBCORE_CHECK_SRCDIR))) \
	 $(call OBJFILES, $(call CCFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call SRCFILES, $(LIBCORE_CHECK_SRCDIR))) \
	-Wl,--start-group,--whole-archive $(LIBCORE_DEPENDENCIES) $(LIBCORE_STATIC) $(CORE_CHECK_GTEST) -Wl,--no-whole-archive,--end-group \
	-lresolv -ldl -lstdc++ -lpthread -lcheck -lm -lrt 

# Create the static libcore archive.
$(LIBCORE_STATIC): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
	$(RUN)$(AR) $(ARFLAGS) '$@' $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR)))) &> /dev/null
else
	@echo
	$(RUN)$(AR) $(ARFLAGS) '$@' $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))
endif

# Create the libcore shared object.
$(LIBCORE_SHARED): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS_COMBINED) -o '$@' -shared $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR)))) \
	-ggdb3 -fPIC -Wl,-Bsymbolic,--start-group,--whole-archive $(LIBCORE_DEPENDENCIES) -Wl,--no-whole-archive,--end-group -lresolv -lrt -ldl -lpthread

# Compile Source
$(OBJDIR)/src/%.o: src/%.c
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CC) $(DEFINES) $(CFLAGS_COMBINED) $(CFLAGS.$(<F)) $(DEFINES.$(<F)) $(INCLUDES) -MF"$(<:%.c=$(DEPDIR)/%.d)" -MT"$@" -o"$@" "$<"

$(OBJDIR)/check/%.o: check/%.c
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CC) $(DEFINES) $(CFLAGS_COMBINED) $(CFLAGS.$(<F)) $(DEFINES.$(<F)) $(INCLUDES) -MF"$(<:%.c=$(DEPDIR)/%.d)" -MT"$@" -o"$@" "$<"

$(OBJDIR)/%.o: %.cpp
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CPP) $(DEFINES) $(CPPFLAGS_COMBINED) $(CPPFLAGS.$(<F)) $(DEFINES.$(<F)) $(INCLUDES) $(LIBCORE_CHECK_INCLUDES) -MF"$(<:%.cpp=$(DEPDIR)/%.d)" -MD -MP  -MT"$@" -c -o"$@" "$<"

# If we've already generated dependency files, use them to see if a rebuild is required
-include $(LIBCORE_DEPFILES)

# Special Make Directives
.SUFFIXES: .c .cc .cpp .o
.PHONY: warning config finished all check stripped
