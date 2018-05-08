#!/usr/bin/make
#
# The Core Library Makefile (libcore)
#
#########################################################################

# Identity of this package.
PACKAGE_NAME			= libcore
PACKAGE_TARNAME			= libcore
PACKAGE_VERSION			= 0.1
PACKAGE_STRING			= $(PACKAGE_NAME) $(PACKAGE_VERSION)
PACKAGE_BUGREPORT		= support@lavabit.com
PACKAGE_URL				= https://lavabit.com

TOPDIR					= $(realpath .)

LIBCORE_CHECK_SRCDIR		= check
LIBCORE_CHECK_PROGRAM		= core.check$(EXEEXT)
LIBCORE_CHECK_INCLUDES		= -Icheck -Isrc/core #-Icheck

LIBCORE_SRCDIR			= src
LIBCORE_SHARED			= libcore$(DYNLIBEXT)
LIBCORE_STATIC			= libcore$(STATLIBEXT)
LIBCORE_PROGRAMS		=

LIBCORE_OBJFILES		= $(call OBJFILES, $(call SRCFILES, src check)) $(call OBJFILES, $(call CPPFILES, src check))
LIBCORE_DEPFILES		= $(call DEPFILES, $(call SRCFILES, src check)) $(call DEPFILES, $(call CPPFILES, src check))
LIBCORE_STRIPPED		= libcore-stripped$(STATLIBEXT) libcore-stripped$(DYNLIBEXT)
LIBCORE_DEPENDENCIES	=

LIBCORE_REPO				= $(shell which git &> /dev/null && git log &> /dev/null && echo 1)
ifneq ($(strip $(LIBCORE_REPO)),1)
	LIBCORE_VERSION			:= $(PACKAGE_VERSION)
	LIBCORE_COMMIT			:= "NONE"
else
	LIBCORE_VERSION			:= $(PACKAGE_VERSION).$(shell git log --format='%H' | wc -l)
	LIBCORE_COMMIT			:= $(shell git log --format="%H" -n 1 | cut -c33-40)
endif
LIBCORE_TIMESTAMP			= $(shell date +'%Y%m%d.%H%M')

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
DEFINES					+= -D_REENTRANT -DFORTIFY_SOURCE=2 -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DHAVE_NS_TYPE -DCORE_BUILD=$(LIBCORE_VERSION) -DCORE_STAMP=$(LIBCORE_TIMESTAMP)

INCLUDES				= -Ilib/local/include -I/usr/include -Isrc -Icheck/core -Isrc/core -Icheck
WARNINGS				= -Wfatal-errors -Werror -Wall -Wextra -Wformat-security -Warray-bounds  -Wformat=2 -Wno-format-nonliteral

# Compiler Parameters
CC								= gcc
CFLAGS							= -std=gnu99 -O0 -fPIC -fmessage-length=0 -ggdb3 -rdynamic -c $(CFLAGS_WARNINGS) -MMD 
CFLAGS_WARNINGS					= -Wall -Werror -Winline -Wformat-security -Warray-bounds #-Wfatal-errors
CFLAGS_PEDANTIC					= -Wextra -Wpacked -Wunreachable-code -Wformat=2

CPP								= g++
CPPFLAGS						= -std=c++0x $(CPPFLAGS_WARNINGS) -Wno-unused-parameter -pthread -g3 
CPPFLAGS_WARNINGS				= -Werror -Wall -Wextra -Wformat=2 -Wwrite-strings -Wno-format-nonliteral #-Wfatal-errors

# Linker Parameters
LD								= gcc
LDFLAGS							= -rdynamic

# Archiver Parameters
AR								= ar
ARFLAGS							= rcs

# Strip Parameters
STRIP							= strip
STRIPFLAGS						= --strip-debug

# GProf Parameters
GPROF							= -pg -finstrument-functions -fprofile-arcs -ftest-coverage

# PProf Parameters
PPROF							= -lprofiler

# Other External programs
MV						= mv --force
RM						= rm --force
RMDIR					= rmdir --parents --ignore-fail-on-non-empty
MKDIR					= mkdir --parents
RANLIB					= ranlib

# Text Coloring
RED						= $$(tput setaf 1)
BLUE					= $$(tput setaf 4)
GREEN					= $$(tput setaf 2)
WHITE					= $$(tput setaf 7)
YELLOW					= $$(tput setaf 3)

# Text Weighting
BOLD					= $$(tput bold)
NORMAL					= $$(tput sgr0)

ifeq ($(OS),Windows_NT)
    HOSTTYPE 			:= Windows
    DYNLIBEXT			:= .dll
    STATLIBEXT			:= .lib
    EXEEXT 				:= .exe
else
    HOSTTYPE			:= $(shell uname -s)
    DYNLIBEXT			:= .so
    STATLIBEXT			:= .a
    EXEEXT				:=
endif

ifeq ($(VERBOSE),yes)
RUN						=
else
RUN						= @
VERBOSE					= no
endif

# So we can tell the user what happened
ifdef MAKECMDGOALS
TARGETGOAL				+= $(MAKECMDGOALS)
else
TARGETGOAL				= $(.DEFAULT_GOAL)
endif

all: config warning $(LIBCORE_SHARED) $(LIBCORE_STATIC) $(LIBCORE_PROGRAMS) $(LIBCORE_STRIPPED) finished

stripped: config warning $(LIBCORE_STRIPPED) finished

check: config warning $(LIBCORE_CHECK_PROGRAM)
	@./core.check
ifeq ($(VERBOSE),no)
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)
endif

warning:
ifeq ($(VERBOSE),no)
	@echo
	@echo 'For a more verbose output'
	@echo '  make '$(GREEN)'VERBOSE=yes' $(NORMAL)$(TARGETGOAL)
	@echo
endif

config:
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

#$(LIBCORE_DEPENDENCIES): res/scripts/build.deps.sh res/scripts/build.deps.params.sh
#ifeq ($(VERBOSE),no)
#	@echo 'Running' $(RED)$(<F)$(NORMAL)
#else
#	@echo
#endif
#	$(RUN)res/scripts/build.deps.sh all

$(LIBCORE_STRIPPED): $(LIBCORE_SHARED) $(LIBCORE_STATIC) $(LIBCORE_PROGRAMS)
ifeq ($(VERBOSE),no)
	@echo 'Creating' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(STRIP) $(STRIPFLAGS) --output-format=$(shell objdump -p "$(subst -stripped,,$@)" | grep "file format" | head -1 | \
	awk -F'file format' '{print $$2}' | tr --delete [:space:]) -o "$@" "$(subst -stripped,,$@)"

# Construct the dime check executable
#$(LIBCORE_DEPENDENCIES)
$(LIBCORE_CHECK_PROGRAM):  $(call OBJFILES, $(call CPPFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call CCFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call SRCFILES, $(LIBCORE_CHECK_SRCDIR))) $(LIBCORE_STATIC)
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS) --output='$@' $(call OBJFILES, $(call CPPFILES, $(LIBCORE_CHECK_SRCDIR))) \
	 $(call OBJFILES, $(call CCFILES, $(LIBCORE_CHECK_SRCDIR))) $(call OBJFILES, $(call SRCFILES, $(LIBCORE_CHECK_SRCDIR))) \
	-Wl,--start-group,--whole-archive $(LIBCORE_DEPENDENCIES) $(LIBCORE_STATIC) $(CORE_CHECK_GTEST) -Wl,--no-whole-archive,--end-group \
	-lresolv -lrt -ldl -lm -lstdc++ -lpthread -lcheck

# Construct the dime executable
$(CORE_PROGRAM): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(call SRCFILES, $(CORE_SRCDIR))) $(LIBCORE_STATIC)
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS) --output='$@' $(call OBJFILES, $(call SRCFILES, $(CORE_SRCDIR))) \
	-Wl,--start-group,--whole-archive $(LIBCORE_DEPENDENCIES) $(LIBCORE_STATIC) -Wl,--no-whole-archive,--end-group -lresolv -lrt -ldl -lpthread

# Create the static libcore archive
$(LIBCORE_STATIC): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(AR) $(ARFLAGS) '$@' $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))

# Create the libcore shared object
$(LIBCORE_SHARED): $(LIBCORE_DEPENDENCIES) $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR))))
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS) -o '$@' -shared $(call OBJFILES, $(filter-out $(LIBCORE_FILTERED), $(call SRCFILES, $(LIBCORE_SRCDIR)))) \
	-ggdb3 -fPIC -Wl,-Bsymbolic,--start-group,--whole-archive $(LIBCORE_DEPENDENCIES) -Wl,--no-whole-archive,--end-group -lresolv -lrt -ldl -lpthread

# Compile Source
$(OBJDIR)/src/%.o: src/%.c
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CC) $(CFLAGS) $(CFLAGS.$(<F)) $(DEFINES) $(DEFINES.$(<F)) $(INCLUDES) -MF"$(<:%.c=$(DEPDIR)/%.d)" -MT"$@" -o"$@" "$<"

$(OBJDIR)/check/%.o: check/%.c
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CC) $(CFLAGS) $(CFLAGS.$(<F)) $(DEFINES) $(DEFINES.$(<F)) $(INCLUDES) -MF"$(<:%.c=$(DEPDIR)/%.d)" -MT"$@" -o"$@" "$<"

$(OBJDIR)/%.o: %.cpp
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CPP) $(CPPFLAGS) $(CPPFLAGS.$(<F)) $(DEFINES) $(DEFINES.$(<F)) $(INCLUDES) $(LIBCORE_CHECK_INCLUDES) -MF"$(<:%.cpp=$(DEPDIR)/%.d)" -MD -MP  -MT"$@" -c -o"$@" "$<"

# If we've already generated dependency files, use them to see if a rebuild is required
-include $(LIBCORE_DEPFILES)

# Special Make Directives
.SUFFIXES: .c .cc .cpp .o
.NOTPARALLEL: warning conifg $(LIBCORE_DEPENDENCIES)
.PHONY: warning config finished all check stripped


# vim:set softtabstop=4 shiftwidth=4 tabstop=4:
