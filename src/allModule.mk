# @file module.mk
#
# The topmost subsystem (module) make include file. Uses the MODULES
# list to determine what files to continue the make with (the
# submodules themselves).
EXECUTABLE_MODULES := main
MODULES := io util vssd vssd-solution


include $(patsubst %,$(SOURCE)/%/module.mk,$(MODULES))
include $(patsubst %,$(SOURCE)/%/executable.mk,$(EXECUTABLE_MODULES))
