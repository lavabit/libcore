#!/usr/bin/make
#
# The libcore Makefile
#
#########################################################################

TOPDIR							= $(realpath .)
MFLAGS							=
MAKEFLAGS						= --output-sync=target

# Identity of this package.
PACKAGE_NAME					= Libcore
PACKAGE_TARNAME					= libcore
PACKAGE_VERSION					= 6.4
PACKAGE_STRING					= $(PACKAGE_NAME) $(PACKAGE_VERSION)
PACKAGE_BUGREPORT				= support@lavabit.com
PACKAGE_URL						= https://lavabit.com

MAGMA_PROGRAM					= $(addsuffix $(EXEEXT), libcore)
MAGMA_CHECK_PROGRAM				= $(addsuffix $(EXEEXT), libcore.check)
MAGMA_SHARED_LIBRARY			= $(addsuffix $(DYNLIBEXT), libcore)

# Source Files
BUILD_SRCFILES					= src/engine/status/build.c

MAGMA_STATIC					= 
MAGMA_DYNAMIC					= -lrt -ldl -lpthread -lresolv
MAGMA_SRCDIRS					= $(shell find src -type d -print)
MAGMA_SRCFILES					= $(filter-out $(FILTERED_SRCFILES), $(foreach dir, $(MAGMA_SRCDIRS), $(wildcard $(dir)/*.c)))

MAGMA_CHECK_STATIC				= $(MAGMA_STATIC) $(TOPDIR)/lib/local/lib/libcheck.a
MAGMA_CHECK_DYNAMIC				= $(MAGMA_DYNAMIC) -lm
MAGMA_CHECK_SRCDIRS				= $(shell find check -type d -print)
MAGMA_CHECK_SRCFILES			= $(foreach dir, $(MAGMA_CHECK_SRCDIRS), $(wildcard $(dir)/*.c))

# Bundled Dependency Include Paths
INCDIR							= $(TOPDIR)/lib/local/include

MAGMA_CHECK_INCDIRS				= -Icheck

MAGMA_CINCLUDES					= -Isrc -Icheck #-Isrc/providers -I$(INCDIR) $(addprefix -I,$(MAGMA_INCLUDE_ABSPATHS))
CDEFINES						= -D_REENTRANT -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DHAVE_NS_TYPE -DFORTIFY_SOURCE=2 -DMAGMA_PEDANTIC 
CDEFINES.build.c 				= -DMAGMA_VERSION=\"$(MAGMA_VERSION)\" -DMAGMA_COMMIT=\"$(MAGMA_COMMIT)\" -DMAGMA_TIMESTAMP=\"$(MAGMA_TIMESTAMP)\"

CPPDEFINES						= $(CDEFINES) -DGTEST_TAP_PRINT_TO_STDOUT -DGTEST_HAS_PTHREAD=1


# Hidden Directory for Dependency Files
DEPDIR							= .deps
MAGMA_DEPFILES					= $(patsubst %.c,$(DEPDIR)/%.d,$(MAGMA_SRCFILES))
MAGMA_CHECK_DEPFILES			= $(patsubst %.c,$(DEPDIR)/%.d,$(MAGMA_CHECK_SRCFILES))

# Hidden Directory for Object Files
OBJDIR							= .objs
MAGMA_OBJFILES					= $(patsubst %.c,$(OBJDIR)/%.o,$(MAGMA_SRCFILES))
MAGMA_CHECK_OBJFILES			= $(patsubst %.c,$(OBJDIR)/%.o,$(MAGMA_CHECK_SRCFILES))

MAGMA_PROF_OBJFILES				= $(patsubst %.c,$(OBJDIR)/%.pg.o,$(MAGMA_SRCFILES))
MAGMA_CHECK_PROF_OBJFILES		= $(patsubst %.c,$(OBJDIR)/%.pg.o,$(MAGMA_CHECK_SRCFILES))


# Resolve the External Include Directory Paths
INCLUDE_DIR_VPATH				= $(INCDIR) /usr/include /usr/local/include
INCLUDE_DIR_SEARCH 				= $(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(INCLUDE_DIR_VPATH)))))

# Generate the Absolute Directory Paths for Includes
MAGMA_INCLUDE_ABSPATHS			+= $(foreach target,$(MAGMA_INCDIRS), $(call INCLUDE_DIR_SEARCH,$(target)))
MAGMA_CHECK_INCLUDE_ABSPATHS	+= $(foreach target,$(MAGMA_CHECK_INCDIRS), $(call INCLUDE_DIR_SEARCH,$(target)))

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
LDFLAGS							= -rdynamic -shared

# Archiver Parameters
AR								= ar
ARFLAGS							= rcs

# Strip Parameters
STRIP							= strip
STRIPFLAGS						= --strip-debug

# Other External programs
MV								= mv --force
RM								= rm --force
RMDIR							= rmdir --parents --ignore-fail-on-non-empty
MKDIR							= mkdir --parents
RANLIB							= ranlib
INSTALL							= install

# Text Coloring
RED								= $$(tput setaf 1)
BLUE							= $$(tput setaf 4)
GREEN							= $$(tput setaf 2)
WHITE							= $$(tput setaf 7)
YELLOW							= $$(tput setaf 3)

# Text Weighting
BOLD							= $$(tput bold)
NORMAL							= $$(tput sgr0)

# Calculate the version, commit and timestamp strings.
MAGMA_REPO						= $(shell which git &> /dev/null && git log &> /dev/null && echo 1) 
ifneq ($(strip $(MAGMA_REPO)),1)
	MAGMA_VERSION				:= $(PACKAGE_VERSION)
	MAGMA_COMMIT				:= "NONE"
else
	# Add the --since='YYYY/MM/DD' or --since='TAG' to the git log command below to reset the patch version to 0.
	MAGMA_VERSION				:= $(PACKAGE_VERSION).$(shell git log --format='%H' | wc -l)
	MAGMA_COMMIT				:= $(shell git log --format="%H" -n 1 | cut -c33-40)
endif

MAGMA_TIMESTAMP					= $(shell date +'%Y%m%d.%H%M')

ifeq ($(VERBOSE),yes)
RUN								=
else
RUN								= @
VERBOSE							= no
endif

# So we can tell the user what happened
ifdef MAKECMDGOALS
TARGETGOAL						+= $(MAKECMDGOALS)
else
TARGETGOAL						= $(.DEFAULT_GOAL)
endif

ifeq ($(OS),Windows_NT)
    HOSTTYPE 					:= "Windows"
    LIBPREFIX					:= 
    DYNLIBEXT					:= ".dll"
    STATLIBEXT					:= ".lib"
    EXEEXT 						:= ".exe"
    
    # Alias the target names on Windows to the equivalent without the exe extension.
	$(basename $(MAGMA_PROGRAM)) := $(MAGMA_PROGRAM)
	$(basename $(MAGMA_CHECK_PROGRAM)) := $(MAGMA_CHECK_PROGRAM)
else
    HOSTTYPE					:= $(shell uname -s)
    LIBPREFIX					:= lib
    DYNLIBEXT					:= .so
    STATLIBEXT					:= .a
    EXEEXT						:= 
endif

all: config warning $(MAGMA_PROGRAM) 

strip: config warning stripped-$(MAGMA_PROGRAM) stripped-$(MAGMA_SHARED_LIBRARY) finished
	
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
	@echo 'VERSION ' $(MAGMA_VERSION)
	@echo 'COMMIT '$(MAGMA_COMMIT)
	@echo 'DATE ' $(MAGMA_TIMESTAMP)
	@echo 'HOST ' $(HOSTTYPE)

setup: $(PACKAGE_DEPENDENCIES)
ifeq ($(VERBOSE),no)
	@echo 'Running the '$(YELLOW)'setup'$(NORMAL)' scripts.'
endif
	$(RUN)dev/scripts/linkup.sh
ifeq ($(VERBOSE),no)
	@echo 'Generating new '$(YELLOW)'key'$(NORMAL)' files.'
endif

check: config warning $(MAGMA_CHECK_PROGRAM) $(DIME_CHECK_PROGRAM) finished
	$(RUN)$(TOPDIR)/$(MAGMA_CHECK_PROGRAM) sandbox/etc/magma.sandbox.config
	$(RUN)$(TOPDIR)/$(DIME_CHECK_PROGRAM)

gprof: $(MAGMA_PROGRAM_GPROF) $(MAGMA_CHECK_PROGRAM_GPROF)

pprof: $(MAGMA_PROGRAM_PPROF) $(MAGMA_CHECK_PROGRAM_PPROF)
  
# If verbose mode is disabled, we only output this finished message.
finished:
ifeq ($(VERBOSE),no)
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)
endif

#incremental: $(MAGMA_INCREMENTAL_BUILD)

# Delete the compiled program along with the generated object and dependency files
clean:
	@$(RM) $(MAGMA_PROGRAM) $(DIME_PROGRAM) $(SIGNET_PROGRAM) $(GENREC_PROGRAM) $(MAGMA_CHECK_PROGRAM) $(DIME_CHECK_PROGRAM) 
	@$(RM) $(MAGMA_PROGRAM_PPROF) $(MAGMA_CHECK_PROGRAM_PPROF) $(MAGMA_PROGRAM_GPROF) $(MAGMA_CHECK_PROGRAM_GPROF)
	@$(RM) $(MAGMA_OBJFILES) $(DIME_OBJFILES) $(SIGNET_OBJFILES) $(GENREC_OBJFILES) $(MAGMA_CHECK_OBJFILES) $(DIME_CHECK_OBJFILES) $(MAGMA_PROF_OBJFILES) $(MAGMA_CHECK_PROF_OBJFILES) 
	@$(RM) $(MAGMA_DEPFILES) $(DIME_DEPFILES) $(SIGNET_DEPFILES) $(GENREC_DEPFILES) $(MAGMA_CHECK_DEPFILES) $(DIME_CHECK_DEPFILES)
	@for d in $(sort $(dir $(MAGMA_OBJFILES)) $(dir $(MAGMA_CHECK_OBJFILES)) $(dir $(DIME_OBJFILES)) $(dir $(SIGNET_OBJFILES)) $(dir $(GENREC_OBJFILES))); \
		do if test -d "$$d"; then $(RMDIR) "$$d"; fi; done
	@for d in $(sort $(dir $(MAGMA_DEPFILES)) $(dir $(MAGMA_CHECK_DEPFILES)) $(dir $(DIME_DEPFILES)) $(dir $(SIGNET_DEPFILES)) $(dir $(GENREC_DEPFILES))); \
		do if test -d "$$d"; then $(RMDIR) "$$d"; fi; done
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)

distclean: 
	@$(RM) $(MAGMA_PROGRAM) $(DIME_PROGRAM) $(SIGNET_PROGRAM) $(GENREC_PROGRAM) $(MAGMA_CHECK_PROGRAM) $(DIME_CHECK_PROGRAM) 
	@$(RM) $(MAGMA_PROGRAM_PPROF) $(MAGMA_CHECK_PROGRAM_PPROF) $(MAGMA_PROGRAM_GPROF) $(MAGMA_CHECK_PROGRAM_GPROF) $(MAGMA_SHARED_LIBRARY)
	@$(RM) $(MAGMA_OBJFILES) $(DIME_OBJFILES) $(SIGNET_OBJFILES) $(GENREC_OBJFILES) $(MAGMA_CHECK_OBJFILES) $(DIME_CHECK_OBJFILES) $(MAGMA_PROF_OBJFILES) $(MAGMA_CHECK_PROF_OBJFILES)  
	@$(RM) $(MAGMA_DEPFILES) $(DIME_DEPFILES) $(SIGNET_DEPFILES) $(GENREC_DEPFILES) $(MAGMA_CHECK_DEPFILES) $(DIME_CHECK_DEPFILES)
	@$(RM) --recursive --force $(DEPDIR) $(OBJDIR) lib/local lib/logs lib/objects lib/sources
	@echo 'Finished' $(BOLD)$(GREEN)$(TARGETGOAL)$(NORMAL)

stripped-%: $(MAGMA_PROGRAM) $(MAGMA_SHARED_LIBRARY)
ifeq ($(VERBOSE),no)
	@echo 'Creating' $(RED)$@$(NORMAL)
else
	@echo 
endif
	$(RUN)$(STRIP) $(STRIPFLAGS) --output-format=$(shell objdump -p "$(subst stripped-,,$@)" | grep "file format" | head -1 | \
	awk -F'file format' '{print $$2}' | tr --delete [:space:]) -o "$@" "$(subst stripped-,,$@)"

install: $(MAGMA_PROGRAM) $(MAGMA_SHARED_LIBRARY)
ifeq ($(VERBOSE),no)
	@echo 'Installing' $(GREEN)$(MAGMA_PROGRAM)$(NORMAL)
endif
	$(RUN)$(INSTALL) --mode=0755 --owner=root --group=root --context=system_u:object_r:bin_t:s0 --no-target-directory \
		$(MAGMA_PROGRAM) /usr/libexec/$(MAGMA_PROGRAM)
	$(RUN)$(INSTALL) --mode=0755 --owner=root --group=root --context=system_u:object_r:bin_t:s0 --no-target-directory \
		$(MAGMA_SHARED_LIBRARY) /usr/libexec/$(MAGMA_SHARED_LIBRARY)
		
# Construct the magma daemon executable.
$(MAGMA_PROGRAM): $(PACKAGE_DEPENDENCIES) $(MAGMA_OBJFILES)
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
else
	@echo
endif
	$(RUN)$(LD) $(LDFLAGS) -o '$@' $(MAGMA_OBJFILES) -Wl,--start-group $(MAGMA_DYNAMIC) $(MAGMA_STATIC) -Wl,--end-group

# Construct the magma unit test executable.
$(MAGMA_CHECK_PROGRAM): $(PACKAGE_DEPENDENCIES) $(MAGMA_CHECK_OBJFILES) $(filter-out .objs/src/magma.o, $(MAGMA_OBJFILES))
ifeq ($(VERBOSE),no)
	@echo 'Constructing' $(RED)$@$(NORMAL)
endif
	$(RUN)$(LD) $(LDFLAGS) -o '$@' $(MAGMA_CHECK_OBJFILES) $(filter-out .objs/src/magma.o, $(MAGMA_OBJFILES)) -Wl,--start-group,--whole-archive $(MAGMA_CHECK_STATIC) -Wl,--no-whole-archive,--end-group $(MAGMA_CHECK_DYNAMIC) 
		
# The Magma Daemon Object Files
$(OBJDIR)/%.o: %.c 
ifeq ($(VERBOSE),no)
	@echo 'Building' $(YELLOW)$<$(NORMAL)
endif
	@test -d $(DEPDIR)/$(dir $<) || $(MKDIR) $(DEPDIR)/$(dir $<)
	@test -d $(OBJDIR)/$(dir $<) || $(MKDIR) $(OBJDIR)/$(dir $<)
	$(RUN)$(CC) -o '$@' $(CFLAGS) $(CDEFINES) $(CFLAGS.$(<F)) $(CDEFINES.$(<F)) $(MAGMA_CINCLUDES) -MF"$(<:%.c=$(DEPDIR)/%.d)" -MT"$@" "$<"

$(PACKAGE_DEPENDENCIES): 
ifeq ($(VERBOSE),no)
	@echo 'Building the '$(YELLOW)'bundled'$(NORMAL)' dependencies.'
endif
	$(RUN)dev/scripts/builders/build.lib.sh all

# If we've already generated dependency files, use them to see if a rebuild is required
-include $(MAGMA_DEPFILES) $(MAGMA_CHECK_DEPFILES)

# Special Make Directives
.SUFFIXES: .c .cc .cpp .o 
#.NOTPARALLEL: warning conifg $(PACKAGE_DEPENDENCIES)
.PHONY: all strip warning config finished check setup clean distclean install pprof gprof

# vim:set softtabstop=4 shiftwidth=4 tabstop=4:
