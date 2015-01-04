#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
# 
TOP= ..

include $(TOP)/config.mak

SCRIPTS= $(CNXTTARGET)config $(CNXTTARGET)stop $(CNXTTARGET)modconflicts
ifeq ($(CNXTDRIVER),hsf)
SCRIPTS+= $(CNXTTARGET)dcpd
endif
ifeq ($(CNXTDRIVER),dgc)
SCRIPTS+= $(CNXTTARGET)dcpd
endif
SCRIPTS_NOINST= patcher
SCRIPTS_PARTIAL= rc$(CNXTTARGET)

all: $(SCRIPTS_NOINST) $(SCRIPTS) $(SCRIPTS_PARTIAL)

$(CNXTTARGET)config.in: cnxtconfig.in
	ln -s $< $@

$(CNXTTARGET)stop.in: cnxtstop.in
	ln -s $< $@

$(CNXTTARGET)dcpd.in: cnxtdcpd.in
	ln -s $< $@

$(CNXTTARGET)modconflicts.in: cnxtmodconflicts.in
	ln -s $< $@

rc$(CNXTTARGET).in: rccnxt.in
	ln -s $< $@

install: $(SCRIPTS_NOINST) install-scripts install-partial-scripts

install-scripts: $(SCRIPTS) $(CNXTSBINDIR) $(CNXTETCDIR)
	$(INSTALL) -m 700 $(SCRIPTS) $(CNXTSBINDIR)

install-partial-scripts: $(SCRIPTS_PARTIAL) $(CNXTLIBDIR)
	$(INSTALL) -m 700 $(SCRIPTS_PARTIAL) $(CNXTLIBDIR)

$(CNXTSBINDIR) $(CNXTLIBDIR) $(CNXTETCDIR):
	$(MKDIR) -p $@

uninstall:
	rm -f $(patsubst %, "$(CNXTSBINDIR)/%", $(SCRIPTS))
	rm -f $(patsubst %, "$(CNXTLIBDIR)/%", $(SCRIPTS_PARTIAL))

clean:
	rm -f $(patsubst %, %.in, $(SCRIPTS)) $(patsubst %, %.in, $(SCRIPTS_PARTIAL))
	rm -f $(SCRIPTS) $(SCRIPTS_PARTIAL) patcher

