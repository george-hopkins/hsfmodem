#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
# 
TOP= ..

include $(TOP)/config.mak
-include $(TOP)/modules/imported/makeflags.mak

CFLAGS =	-I$(TOP)/modules/imported/include -I$(TOP)/modules/include
CFLAGS +=	$(IMPORTED_FRAMEWORK_DEFS) $(IMPORTED_GENERAL_DEFS)
CFLAGS +=	-DCNXTTARGET="\"$(CNXTTARGET)\""


#CFLAGS +=	-g

ifeq ($(IMPORTED_BLAM_SUPPORT),yes)
ALL +=		$(CNXTTARGET)diag
endif

ifneq ($(IMPORTED_DMP_SUPPORT),)
ALL +=		$(CNXTTARGET)dmp
endif

# old/deprecated:
ifeq ($(IMPORTED_SCR_SUPPORT),yes)
ALL +=		$(CNXTTARGET)scr
endif

all: $(ALL)

install: $(ALL) $(CNXTSBINDIR)
ifneq ($(ALL),)
	$(INSTALL) -m 755 $(ALL) $(CNXTSBINDIR)/
endif

$(CNXTSBINDIR):
	$(MKDIR) -p $@

$(CNXTTARGET)diag: diag.o modemexpert.o
	$(CC) -o $@ $^

$(CNXTTARGET)scr: scr.o
	$(CC) -o $@ $^

$(CNXTTARGET)dmp: dmp.o
	$(CC) -o $@ $^

clean:
	rm -f *.o $(ALL)

uninstall:
	rm -f "$(CNXTSBINDIR)/$(CNXTTARGET)diag" "$(CNXTSBINDIR)/$(CNXTTARGET)scr" "$(CNXTSBINDIR)/$(CNXTTARGET)dmp"

