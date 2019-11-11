#
#       Please modify this file to reflect the supported processor names
#       and the output file
#

# The output file name without extension
# Usually it is the processor family name
PROC=arm

# The processor description string
# Its format is
#       family_name:processor1:processor2:...
# The processor names in the string should be synchronized
# with the short processor names in reg.cpp
DESCRIPTION=ARM processors:ARM

# If you have additional files to prepare, then use
#ADDITIONAL_GOALS=desired_additional_file_names
# For example:
#ADDITIONAL_GOALS=$(R)$(PROC).cfg
# and add rules to prepare the additional files after the !include directive

!include ..\module.mak

# MAKEDEP dependency list ------------------
$(F)ana$(O)   : $(I)area.hpp $(I)auto.hpp $(I)bytes.hpp $(I)funcs.hpp       \
	        $(I)help.h $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp          \
	        $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp     \
	        $(I)name.hpp $(I)netnode.hpp $(I)offset.hpp $(I)pro.h       \
	        $(I)queue.hpp $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp       \
	        ..\idaidp.hpp ana.cpp arm.hpp ins.hpp
$(F)emu$(O)   : $(I)area.hpp $(I)auto.hpp $(I)bytes.hpp $(I)funcs.hpp       \
	        $(I)help.h $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp          \
	        $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp     \
	        $(I)name.hpp $(I)netnode.hpp $(I)offset.hpp $(I)pro.h       \
	        $(I)queue.hpp $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp       \
	        ..\idaidp.hpp arm.hpp emu.cpp ins.hpp
$(F)ins$(O)   : $(I)area.hpp $(I)auto.hpp $(I)bytes.hpp $(I)funcs.hpp       \
	        $(I)help.h $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp          \
	        $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp     \
	        $(I)name.hpp $(I)netnode.hpp $(I)offset.hpp $(I)pro.h       \
	        $(I)queue.hpp $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp       \
	        ..\idaidp.hpp arm.hpp ins.cpp ins.hpp
$(F)out$(O)   : $(I)area.hpp $(I)auto.hpp $(I)bytes.hpp $(I)funcs.hpp       \
	        $(I)help.h $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp          \
	        $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp     \
	        $(I)name.hpp $(I)netnode.hpp $(I)offset.hpp $(I)pro.h       \
	        $(I)queue.hpp $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp       \
	        ..\idaidp.hpp arm.hpp ins.hpp out.cpp
$(F)reg$(O)   : $(I)area.hpp $(I)auto.hpp $(I)bytes.hpp $(I)funcs.hpp       \
	        $(I)help.h $(I)ida.hpp $(I)idp.hpp $(I)kernwin.hpp          \
	        $(I)lines.hpp $(I)llong.hpp $(I)loader.hpp $(I)nalt.hpp     \
	        $(I)name.hpp $(I)netnode.hpp $(I)offset.hpp $(I)pro.h       \
	        $(I)queue.hpp $(I)segment.hpp $(I)ua.hpp $(I)xref.hpp       \
	        ..\idaidp.hpp arm.hpp ins.hpp reg.cpp
