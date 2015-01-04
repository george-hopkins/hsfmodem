#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
#
-include $(TOP)/modules/imported/makeflags.mak

# The version string specified here is propagated everywhere
CNXTLINUXVERSION=$(IMPORTED_CNXTLINUXVERSION)
CNXTLINUX_REL=1

CNXTLINUXRPM_REL=$(CNXTLINUX_REL)
CNXTLINUXDEB_REL=$(CNXTLINUX_REL)

ifeq ($(IMPORTED_TARGET),TARGET_HSF_LINUX)
CNXTDRVDSC=Conexant HSF softmodem
CNXTDRIVER=hsf
CNXTTARGET=hsf
CNXTSERDEV=HSF
endif
ifeq ($(IMPORTED_TARGET),TARGET_HCF_USB_LINUX)
CNXTDRVDSC=Conexant HCF controllerless USB modem
CNXTDRIVER=hcf
CNXTTARGET=hcfusb
CNXTSERDEV=HCFUSB
endif
ifeq ($(IMPORTED_TARGET),TARGET_HCF_PCI_LINUX)
CNXTDRVDSC=Conexant HCF controllerless PCI modem
CNXTDRIVER=hcf
CNXTTARGET=hcfpci
CNXTSERDEV=HCF
endif
ifeq ($(IMPORTED_TARGET),TARGET_DGC_LINUX)
CNXTDRVDSC=Conexant DGC USB modem
CNXTDRIVER=dgc
CNXTTARGET=dgc
CNXTSERDEV=DGC
endif

ifeq ($(CNXTDRIVER),hsf)
CNXTMODS= $(CNXTTARGET)pcibasic2 $(CNXTTARGET)pcibasic3 $(CNXTTARGET)mc97ich $(CNXTTARGET)mc97via $(CNXTTARGET)mc97ali $(CNXTTARGET)mc97ati $(CNXTTARGET)mc97sis $(CNXTTARGET)usbcd2 $(CNXTTARGET)hda $(CNXTTARGET)soar
endif
ifeq ($(CNXTDRIVER),hcf)
CNXTMODS= $(CNXTTARGET)hw
endif
ifeq ($(CNXTDRIVER),dgc)
CNXTMODS= $(CNXTTARGET)usbdcp
else
CNXTMODS+= $(CNXTTARGET)serial $(CNXTTARGET)engine
CNXTMODS+= $(CNXTTARGET)osspec
endif

# maximum number of modem units
CNXTMAXMDM=8

ROOT=
PREFIX=$(ROOT)/usr

ETCDIR=$(ROOT)/etc
SBINDIR=$(PREFIX)/sbin
BINDIR=$(PREFIX)/bin
LIBDIR=$(PREFIX)/lib

CNXTETCDIR=$(ETCDIR)/$(CNXTTARGET)modem
CNXTNVMDIR=$(CNXTETCDIR)/nvm

CNXTSBINDIR=$(SBINDIR)
CNXTBINDIR=$(BINDIR)

CNXTLIBDIR=$(LIBDIR)/$(CNXTTARGET)modem
CNXTMODDIR=$(CNXTLIBDIR)/modules

# On some Debian variants, there is no cc symlink to gcc
ifeq ($(CC),cc)
CC := gcc
endif

INSTALL= install
MKDIR=   mkdir -m 755

PATCHERURL=http://www.linuxant.com/drivers/$(CNXTDRIVER)/full/archive/patches
PATCHERURLUSER=http://www.linuxant.com/drivers/$(CNXTDRIVER)/downloads-patches.php

# Default values; these are usually overriden by module options in
# /etc/modules.conf so they shouldn't need to be changed here
CNXTSERIALMAJOR=240
CNXTCALOUTMAJOR=241
CNXTSERIALMINOR=64

CNXTDCPMAJOR=242

CNXTDIAGMAJOR=243
CNXTDIAGDMPMINOR=255

CNXTSCRMAJOR=244


ALL_IN = $(shell find . -name "*.in")
ALL_IN_GEN = $(patsubst %.in, %, $(ALL_IN))

%: %.in
	sed \
		-e "s!@DATE@!`date \"+%a, %d %b %Y %T %z\"`!g" \
		-e 's!@CNXTDRIVER@!$(CNXTDRIVER)!g' \
		-e 's!@CNXTDRVDSC@!$(CNXTDRVDSC)!g' \
		-e 's!@CNXTTARGET@!$(CNXTTARGET)!g' \
		-e 's!@CNXTARCH@!$(IMPORTED_ARCH)!g' \
		-e 's!@CNXTSERDEV@!$(CNXTSERDEV)!g' \
		-e 's!@CNXTMAXMDM@!$(CNXTMAXMDM)!g' \
		-e 's!@CNXTETCDIR@!$(CNXTETCDIR)!g' \
		-e 's!@CNXTLIBDIR@!$(CNXTLIBDIR)!g' \
		-e 's!@CNXTSERIALMAJOR@!$(CNXTSERIALMAJOR)!g' \
		-e 's!@CNXTCALOUTMAJOR@!$(CNXTCALOUTMAJOR)!g' \
		-e 's!@CNXTSERIALMINOR@!$(CNXTSERIALMINOR)!g' \
		-e 's!@CNXTDCPMAJOR@!$(CNXTDCPMAJOR)!g' \
		-e 's!@CNXTDIAGMAJOR@!$(CNXTDIAGMAJOR)!g' \
		-e 's!@CNXTDIAGDMPMINOR@!$(CNXTDIAGDMPMINOR)!g' \
		-e 's!@CNXTSCRMAJOR@!$(CNXTSCRMAJOR)!g' \
		-e 's!@CNXTSBINDIR@!$(CNXTSBINDIR)!g' \
		-e 's!@CNXTNVMDIR@!$(CNXTNVMDIR)!g' \
		-e 's!@CNXTLINUXVERSION@!$(CNXTLINUXVERSION)!g' \
		-e 's!@CNXTLINUX_REL@!$(CNXTLINUX_REL)!g' \
		-e 's!@CNXTLINUXRPM_REL@!$(CNXTLINUXRPM_REL)!g' \
		-e 's!@CNXTLINUXDEB_REL@!$(CNXTLINUXDEB_REL)!g' \
		-e 's!@CNXTMODS@!$(CNXTMODS)!g' \
		-e 's!@PATCHERURL@!$(PATCHERURL)!g' \
		-e 's!@PATCHERURLUSER@!$(PATCHERURLUSER)!g' \
		-e "s!@BLAM_SUPPORT@!`test ! \"$(IMPORTED_BLAM_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@SCR_SUPPORT@!`test ! \"$(IMPORTED_SCR_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@DMP_SUPPORT@!`test -z \"$(IMPORTED_DMP_SUPPORT)\"; echo $$?`!g" \
		< $< > $@
	chmod --reference=$< $@
