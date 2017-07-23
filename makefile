#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
#
TOP= .

include $(TOP)/config.mak
-include $(TOP)/modules/imported/makeflags.mak

ifneq ($(IMPORTED_TARGET),TARGET_DGC_LINUX)
DEBUG_TOOLS+= diag
ifeq ($(IMPORTED_DMP_SUPPORT),)
TARDIST_EXCLUDES+= *dmp*
endif
ifeq ($(IMPORTED_SCR_SUPPORT),yes)
DEBUG_TOOLS+= diag/qtmodemon
else
TARDIST_EXCLUDES+= scr.* scr_* qtmodemon
endif
#ifneq ($(IMPORTED_BLAM_SUPPORT),yes)
#ifneq ($(IMPORTED_SCR_SUPPORT),yes)
#TARDIST_EXCLUDES+= *diag*
#endif
#endif
endif

SYMLINK_SUPPORT := $(shell if ln -sf symlinktest .lntst.$$$$ > /dev/null 2>&1; then echo yes; else echo no; fi; rm -f .lntst.*)


ifeq ($(IMPORTED_TARGET),TARGET_DGC_LINUX)
SUBDIRS = scripts modules
else
SUBDIRS = nvm scripts modules $(DEBUG_TOOLS)
endif

.PHONY: default
default:
	@echo "Use:"
	@echo "    \"make install\" to install this software"
	@echo "    \"make uninstall\" to uninstall this software"
#	@echo "    \"make modules\" to recompile the kernel modules (normally done by $(CNXTTARGET)config)"
	@echo "    \"make clean\" to remove objects and other derived files"
	@echo "    \"$(CNXTTARGET)config\" (after installation) to setup your modem"
	@echo "    \"make rpmprecomp\" to build a pre-compiled RPM package for the `uname -r` kernel"
	@echo "    \"make debprecomp\" to build a pre-compiled DEB package for the `uname -r` kernel"
	@echo ""
	@false

all: scripts $(DEBUG_TOOLS)

modules/imported:
	@[ -d modules/imported ] || (echo "\"modules/imported\" directory missing!"; exit 1)

$(SUBDIRS) install:: modules/imported

.PHONY: $(SUBDIRS)
$(SUBDIRS)::
	$(MAKE) -C $@ all

.PHONY: uninstall
uninstall::
	if [ -x $(CNXTSBINDIR)/$(CNXTTARGET)config ]; then \
		$(CNXTSBINDIR)/$(CNXTTARGET)config -remove; \
	else \
		true; \
	fi

.PHONY: symlink-support
symlink-support:
ifeq ($(SYMLINK_SUPPORT),no)
	@echo "ERROR: The partition holding the '`pwd`' directory doesn't have a file system"
	@echo "which supports symbolic links. Please move the source tree to a partition with a"
	@echo "native Linux file system and try again to install the software."
	@false
endif

.PHONY: clean install
clean install uninstall:: symlink-support
	@for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir $@ || exit $$?; \
	done

install:: $(CNXTLIBDIR) $(CNXTETCDIR) LICENSE $(CNXTETCDIR)/log
	$(INSTALL) -m 444 LICENSE $(CNXTLIBDIR)
	@(echo -n "TAR " ; pwd ) > "$(CNXTETCDIR)/package"
	@echo ""
	@echo "To complete the installation and configuration of your modem,"
	@echo "please run \"$(CNXTTARGET)config\" (or \"$(CNXTSBINDIR)/$(CNXTTARGET)config\")"

$(CNXTLIBDIR) $(CNXTETCDIR):
	$(MKDIR) -p $@

uninstall::
	rm -f $(CNXTLIBDIR)/LICENSE
	rm -f $(CNXTETCDIR)/package

# Only install docs if present
PDFDOC= $(shell f=100498D_RM_HxF_Released.pdf; test -f $$f && echo $$f)

TARDIST_EXCLUDES+= OLD *,v $(CNXTTARGET)modem*.tar.gz binaries/* GPL/hda-* $(PDFDOC)
ifneq ($(IMPORTED_TARGET),TARGET_HCF_USB_LINUX)
ifneq ($(IMPORTED_TARGET),TARGET_HSF_LINUX)
ifneq ($(IMPORTED_TARGET),TARGET_DGC_LINUX)
TARDIST_EXCLUDES+= *usb*
endif
endif
endif
ifneq ($(IMPORTED_TARGET),TARGET_HCF_PCI_LINUX)
ifneq ($(IMPORTED_TARGET),TARGET_HSF_LINUX)
TARDIST_EXCLUDES+= *pci*
endif
endif

TARPKG=$(CNXTTARGET)modem-$(CNXTLINUXVERSION).tar.gz

.PHONY: tardist
tardist: clean
	$(MAKE) $(TARPKG)

CNXTTEMPDIST:=/tmp/$(CNXTTARGET)dist-$(shell echo $$$$)/$(CNXTTARGET)modem-$(CNXTLINUXVERSION)

$(TARPKG): $(CNXTTARGET)modem.spec
	rm -rf $(CNXTTEMPDIST)
	$(MKDIR) -p $(CNXTTEMPDIST)
	[ -d modules/binaries ] || $(MKDIR) modules/binaries
	find . -depth -print | grep -v '^\.\/packages\/' | cpio -pdmu $(CNXTTEMPDIST)
	(cd $(CNXTTEMPDIST)/.. && tar $(patsubst %, --exclude '%', $(TARDIST_EXCLUDES)) -cf - $(CNXTTARGET)modem-$(CNXTLINUXVERSION)) | gzip > $@
	rm -rf $(dir $(CNXTTEMPDIST))

# Test if our rpm supports --define and --eval options. Early versions didn't
RPMOPTDEFINE=$(shell rpm --define 'test test' >/dev/null 2>&1 && echo yes)
RPMOPTEVAL=$(shell rpm --eval 'test' >/dev/null 2>&1 && echo yes)

ifeq ($(RPMOPTDEFINE),yes)
# We're fine
RPMTOPDIR=$(CURDIR)/packages
else
ifeq ($(RPMOPTEVAL),yes)
RPMTOPDIR=$(shell rpm --eval '%_topdir')
else
RPMTOPDIR=
endif
endif
RPMTOPDIRDEFINED=$(shell grep -q '%_topdir' $$HOME/.rpmmacros 2>/dev/null && echo yes)

ifeq ($(RPMOPTEVAL),yes)
RPMTARGETCPU=$(shell rpm --eval '%_target_cpu')
else
RPMTARGETCPU=$(shell uname -i)
endif

ifneq ($(RPMOPTDEFINE),yes)
UID=$(shell id -u 2>/dev/null)
ifneq ($(UID),0)
ifneq ($(RPMTOPDIRDEFINED),yes)
$(warning If you cannot run make as root and the rpm creation fails with)
$(warning a Permission denied error, try adding the line:)
$(warning %_topdir $(CURDIR)/packages)
$(warning to your ~/.rpmmacros file, and creating under packages)
$(warning the BUILD/ RPMS/ SPECS/ SRPMS/ subdirectories.)
endif
endif
endif

ifneq ($(RPMTOPDIR),)
RPMDIRS=$(patsubst %, $(RPMTOPDIR)/%, BUILD RPMS SPECS SRPMS)
endif

DEBDIRS=$(patsubst %, packages/DEBS/%, i386 amd64 all)

$(RPMDIRS) $(DEBDIRS) $(CNXTETCDIR)/log:
	mkdir -p $@

DISTROKERNHDRSDIR= /home/cnxt/distrokernhdrs

RPMBUILD= $(shell test -x /usr/bin/rpmbuild && echo rpmbuild || echo rpm)

RPMBUILDARGS=
ifeq ($(RPMOPTDEFINE),yes)
RPMBUILDARGS+=	--define='_topdir $(RPMTOPDIR)'
RPMBUILDARGS+=	--define='_distro_kernels $(DISTROKERNHDRSDIR)'
RPMBUILDARGS+=	--define='_unpackaged_files_terminate_build 0'
endif

IS_DEBIAN=$(shell test -e /etc/debian_version && echo yes || echo no)

ifeq ($(IS_DEBIAN):$(IMPORTED_ARCH),yes:x86_64)
IMPORTED_ARCH_DEBIAN=amd64
else
IMPORTED_ARCH_DEBIAN=$(IMPORTED_ARCH)
endif

CNXTLINUXVERSION_DEBIAN=$(subst _,-,$(CNXTLINUXVERSION))

DEB_DISTROS= generic
RPM_DISTROS= generic
TARGETS_generic= $(subst powerpc,ppc,$(IMPORTED_ARCH_DEBIAN))
KERNELS_generic= generic

ifneq ($(IMPORTED_ARCH),powerpc)
-include $(DISTROKERNHDRSDIR)/incavail.mak
endif

DEBFILES= $(patsubst %, debian/%, rules control changelog copyright preinst postinst prerm postrm)
DEBCLEAN= $(patsubst %, debian/%, tmp files $(CNXTTARGET)modem-doc* README.tmp target.mak) build

debname = $(2)/$(1)_$(CNXTLINUXVERSION_DEBIAN)$(subst _kgeneric,,_k$(subst -,_,$(3)))$(subst _generic,,_$(d))_$(2).deb

BINARYDEBS = $(foreach d, $(DEB_DISTROS), $(foreach t, $(TARGETS_$(d)), $(foreach k,$(KERNELS_$(d)), packages/DEBS/$(call debname,$(CNXTTARGET)modem,$t,$k))))

getdebverdist=$(word 2,$(subst @, ,$(subst _$(*D),,$(subst _k2,@2,$(*F)))))
getdebwords=$(subst _, ,$(call getdebverdist))
getdebdist=$(if $(call getdebwords),$(word $(words $(call getdebwords)),$(call getdebwords)),generic)
getdebver=$(subst _,-,$(subst _$(call getdebdist),,$(call getdebverdist)))
getdebrequires=$(foreach t, $(REQUIRES),$(if $(filter $(call getdebver),$(REQUIRES_$(t))),$(REQUIRES_VALUE_$(t))))
$(BINARYDEBS): packages/DEBS/%.deb:
	@echo ""
	@echo "================================================================================"
	@echo "DPKG-BUILDPACKAGE $(@F)"
	@echo "================================================================================"
	rm -fr modules/binaries/linux-*
	@k="$(call getdebver)"; d="$(call getdebdist)"; \
	sed -e 's/^Depends:.*$$/Depends: $(call getdebrequires)/g' < debian/control > debian/control.tmp; \
	chmod --reference=debian/control debian/control.tmp; \
	rm -f debian/control; \
	mv debian/control.tmp debian/control; \
	if [ -z "$$k" ]; then \
		sed -e 's/$(CNXTTARGET)modem (.*)/$(CNXTTARGET)modem ($(CNXTLINUXVERSION_DEBIAN))/' < debian/changelog > debian/changelog.tmp; \
		chmod --reference=debian/changelog debian/changelog.tmp; \
		rm -f debian/changelog; \
		mv debian/changelog.tmp debian/changelog; \
		( echo CNXTDRIVER=$(CNXTDRIVER); echo "PACKAGE_NAME=$(@F)"; echo "PACKAGE_ARCH=$(*D)" ) > debian/target.mak; \
	else \
		sed -e 's/$(CNXTTARGET)modem (.*)/$(CNXTTARGET)modem ($(CNXTLINUXVERSION_DEBIAN)-k$(call getdebver)-$(CNXTLINUXDEB_REL).$(call getdebdist))/' < debian/changelog > debian/changelog.tmp; \
		chmod --reference=debian/changelog debian/changelog.tmp; \
		rm -f debian/changelog; \
		mv debian/changelog.tmp debian/changelog; \
		( echo CNXTDRIVER=$(CNXTDRIVER); echo "TARGET_DISTRO=$$d"; echo "TARGET_KERNEL=$$k"; echo "DISTROKERNHDRSDIR=$(DISTROKERNHDRSDIR)"; echo "PACKAGE_NAME=$(@F)"; echo "PACKAGE_ARCH=$(*D)" ) > debian/target.mak; \
	fi
	unset LANG; unset LOCALE; unset LC_TIME; unset LC_ALL; dpkg-buildpackage -a$(IMPORTED_ARCH_DEBIAN) -us -uc || true
	rm -f debian/target.mak
	mv ../$(CNXTTARGET)modem_*.deb packages/DEBS/$(*D)/$(@F)
	rm -f ../$(CNXTTARGET)modem_* ../$(CNXTTARGET)modem-doc_*

UNAMER := $(shell uname -r)
CUSTOMDEB := packages/DEBS/$(IMPORTED_ARCH_DEBIAN)/$(CNXTTARGET)modem_$(CNXTLINUXVERSION_DEBIAN)-k$(subst -,_,$(UNAMER))_custom_$(IMPORTED_ARCH_DEBIAN).deb

$(CUSTOMDEB): $(TARPKG)
	@sed -e 's/$(CNXTTARGET)modem (.*)/$(CNXTTARGET)modem ($(CNXTLINUXVERSION_DEBIAN)-k$(subst _,-,$(UNAMER))-$(CNXTLINUXDEB_REL).custom)/' < debian/changelog > debian/changelog.tmp; \
	chmod --reference=debian/changelog debian/changelog.tmp; \
	rm -f debian/changelog; \
	mv debian/changelog.tmp debian/changelog; \
	( echo CNXTDRIVER=$(CNXTDRIVER); echo "TARGET_KERNEL=$(UNAMER)"; echo "TARGET_DISTRO=custom"; echo "PACKAGE_NAME=$(@F)"; echo "PACKAGE_ARCH=$(*D)" ) > debian/target.mak
	unset LANG; unset LOCALE; unset LC_TIME; unset LC_ALL; dpkg-buildpackage -a$(IMPORTED_ARCH_DEBIAN) -us -uc || true
	rm -f debian/target.mak
	mv ../$(CNXTTARGET)modem_*.deb packages/DEBS/$(IMPORTED_ARCH_DEBIAN)/$(@F)
	rm -f ../$(CNXTTARGET)modem_* ../$(CNXTTARGET)modem-doc_*

.PHONY:
debprecomp: clean $(DEBFILES) packages/DEBS/$(IMPORTED_ARCH_DEBIAN) packages/DEBS/all $(CUSTOMDEB)

.PHONY: debdist
debdist: clean $(DEBFILES) packages/DEBS/$(IMPORTED_ARCH_DEBIAN) packages/DEBS/all $(BINARYDEBS)
	rm -f ../$(CNXTTARGET)modem_$(CNXTLINUXVERSION)*
	rm -f ../$(CNXTTARGET)modem-doc_$(CNXTLINUXVERSION)*

debian/README.tmp: README
	expand < README | sed -e 's/^/ /' -e 's/^ $$/ ./' > debian/README.tmp

debian/%: debian/%.in debian/README.tmp
	( \
	  sed -n \
		-e "s!@DATE@!`LC_TIME=C date \"+%a, %d %b %Y %T %z\"`!g" \
		-e 's!@CNXTDRIVER@!$(CNXTDRIVER)!g' \
		-e 's!@CNXTDRVDSC@!$(CNXTDRVDSC)!g' \
		-e 's!@CNXTTARGET@!$(CNXTTARGET)!g' \
		-e 's!@CNXTETCDIR@!$(CNXTETCDIR)!g' \
		-e 's!@CNXTLIBDIR@!$(CNXTLIBDIR)!g' \
		-e 's!@CNXTSBINDIR@!$(CNXTSBINDIR)!g' \
		-e 's!@CNXTLINUXVERSION@!$(CNXTLINUXVERSION)!g' \
		-e 's!@CNXTLINUX_REL@!$(CNXTLINUX_REL)!g' \
		-e 's!@CNXTLINUXRPM_REL@!$(CNXTLINUXRPM_REL)!g' \
		-e 's!@CNXTLINUXDEB_REL@!$(CNXTLINUXDEB_REL)!g' \
		-e "s!@BLAM_SUPPORT@!`test ! \"$(IMPORTED_BLAM_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@SCR_SUPPORT@!`test ! \"$(IMPORTED_SCR_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@DMP_SUPPORT@!`test -z \"$(IMPORTED_DMP_SUPPORT)\"; echo $$?`!g" \
		-e '/@README_FILE@/ {' \
		-e 'r debian/README.tmp' \
		-e 'd' \
		-e '}' \
		-e '/@LICENSE_FILE@/ {' \
		-e 'r LICENSE' \
		-e 'd' \
		-e '}' \
		-e 'p' \
		< $< ; \
	  true sed -e 's/^\([A-Z]\)/* \1/' -e 's/^	\* /	- /' < CHANGES \
	) > $@
	chmod --reference=$< $@

rpmname = $(2)/$(1)-$(CNXTLINUXVERSION)$(subst _kgeneric,,_k$(subst -,_,$(3)))-$(CNXTLINUXRPM_REL)$(subst _$(2),,$(subst generic,,$(d))).$(2).rpm

BINARYRPMS = $(foreach d, $(RPM_DISTROS), $(foreach t, $(TARGETS_$(d)), $(foreach k,$(KERNELS_$(d)), packages/RPMS/$(call rpmname,$(CNXTTARGET)modem,$t,$k))))
SOURCERPM  = $(subst /src/,/SRPMS/,packages/$(call rpmname,$(CNXTTARGET)modem,src,generic))

NVMCVTDIR:=/tmp/$(CNXTTARGET)modem-$(CNXTLINUXVERSION)-dist-nvmcvtcache

getkdistro=$(subst _,-,$(subst -$(CNXTLINUXRPM_REL), ,$(word 2,$(subst @, ,$(patsubst %.$(*D),%,$(subst _k,@,$(*F)))))))
getrequires=$(foreach t, $(REQUIRES),$(if $(filter $(call getkdistro),$(REQUIRES_$(t))),$(REQUIRES_VALUE_$(t))))
$(BINARYRPMS): packages/RPMS/%.rpm:
	@echo ""
	@echo "================================================================================"
	@echo "RPMBUILD $(@F)"
	@echo "================================================================================"
	@NVMCVTDIR=$(NVMCVTDIR); export NVMCVTDIR; k="$(word 1,$(call getkdistro))"; d="$(word 2,$(call getkdistro))"; \
		if [ -z "$$k" ]; then \
			$(RPMBUILD) -tb $(RPMBUILDARGS) --target $(*D) $(TARPKG); \
		else \
			$(RPMBUILD) -tb --define="_target_distro $$d" --define="_target_kernel $$k" --define="_requires $(call getrequires)" $(RPMBUILDARGS) --target $(*D) $(TARPKG); \
		fi

CUSTOMRPM := packages/RPMS/$(RPMTARGETCPU)/$(CNXTTARGET)modem-$(CNXTLINUXVERSION)_k$(subst -,_,$(UNAMER))-1custom.$(RPMTARGETCPU).rpm

$(CUSTOMRPM): $(TARPKG)
	@NVMCVTDIR=$(NVMCVTDIR); export NVMCVTDIR; \
	$(RPMBUILD) -tb --define="_target_distro custom" $(RPMBUILDARGS) $(TARPKG)

.PHONY: rpmprecomp
rpmprecomp: $(RPMDIRS) $(CUSTOMRPM)

ifneq ($(PDFDOC),)
DOCRPM  = packages/RPMS/$(call rpmname,$(CNXTTARGET)modem-doc,noarch,generic)

$(DOCRPM): packages/RPMS/%.rpm:
	@echo ""
	@echo "================================================================================"
	@echo "RPMBUILD $(@F)"
	@echo "================================================================================"
	$(RPMBUILD) -tb --define="_build_doc 1" $(RPMBUILDARGS) $(TARPKG)
	rm -f packages/RPMS/$(call rpmname,$(CNXTTARGET)modem,noarch,generic)
endif

ALLRPMS = $(BINARYRPMS)
ALLRPMS = $(BINARYRPMS)
ifneq ($(DOCRPM),)
ALLRPMS+= $(DOCRPM)
endif

.PHONY: rpmdist
rpmdist: $(RPMDIRS) $(TARPKG) $(BINARYRPMS) $(DOCRPM)

$(CNXTTARGET)modem.spec: cnxtmodem.spec.in CHANGES
	( \
	  sed -n \
		-e 's!@CNXTDRIVER@!$(CNXTDRIVER)!g' \
		-e 's!@CNXTDRVDSC@!$(CNXTDRVDSC)!g' \
		-e 's!@CNXTTARGET@!$(CNXTTARGET)!g' \
		-e 's!@CNXTETCDIR@!$(CNXTETCDIR)!g' \
		-e 's!@CNXTLIBDIR@!$(CNXTLIBDIR)!g' \
		-e 's!@CNXTLINUXVERSION@!$(CNXTLINUXVERSION)!g' \
		-e 's!@CNXTLINUX_REL@!$(CNXTLINUX_REL)!g' \
		-e 's!@CNXTLINUXRPM_REL@!$(CNXTLINUXRPM_REL)!g' \
		-e 's!@CNXTLINUXDEB_REL@!$(CNXTLINUXDEB_REL)!g' \
		-e "s!@BLAM_SUPPORT@!`test ! \"$(IMPORTED_BLAM_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@SCR_SUPPORT@!`test ! \"$(IMPORTED_SCR_SUPPORT)\" = yes; echo $$?`!g" \
		-e "s!@DMP_SUPPORT@!`test -z \"$(IMPORTED_DMP_SUPPORT)\"; echo $$?`!g" \
		-e '/@LICENSE_FILE@/ {' \
		-e 'r LICENSE' \
		-e 'd' \
		-e '}' \
		-e 'p' \
		< $< ; \
	  sed -e 's/^\([A-Z]\)/* \1/' -e 's/^	\* /	- /' < CHANGES \
	) > $@
	chmod 644 $@

clean::
	rm -f $(CNXTTARGET)modem.spec packages/SPECS/$(CNXTTARGET)modem.spec
	rm -f $(filter-out ./debian/%,$(ALL_IN_GEN))

pkgclean: clean
	rm -f $(filter ./debian/%,$(ALL_IN_GEN))
	rm -f $(CNXTTARGET)modem*.tar.gz
	rm -f $(RPMTOPDIR)/RPMS/*/$(CNXTTARGET)modem* $(RPMTOPDIR)/SRPMS/$(CNXTTARGET)modem*
	rm -rf $(RPMTOPDIR)/BUILD/$(CNXTTARGET)modem*
	rm -rf modules/binaries/*
	rm -rf $(DEBFILES) $(DEBCLEAN)
	rm -rf $(NVMCVTDIR)
	rm -fr packages/DEBS
	rm -fr installed_patches

ALLRPMSZIP= $(patsubst %.rpm,%.rpm.zip,$(ALLRPMS))
ALLDEBSZIP= $(patsubst %.deb,%.deb.zip,$(BINARYDEBS))

%.zip: %
	rm -f $@
	zip -j $@ $^ LICENSE

WWWDIR=/var/www

ifeq ($(CNXTDRIVER),dgc)
RELDIR=$(WWWDIR)/linuxant.com/drivers/$(CNXTDRIVER)/archive/$(CNXTTARGET)modem-$(CNXTLINUXVERSION)
else
RELDIR=$(WWWDIR)/linuxant.com/drivers/$(CNXTDRIVER)/$(IMPORTED_BUILD_TYPE)/archive/$(CNXTTARGET)modem-$(CNXTLINUXVERSION)
endif

EXTDIR=$(WWWDIR)/build_drivers/extracted

$(RELDIR):
	mkdir -p $@

.PHONY: resignrpms
resignrpms:
	rpm --resign $(ALLRPMS)

.PHONY: ziprpms zipdebs
ziprpms: $(ALLRPMSZIP)
zipdebs: $(ALLDEBSZIP)

ifeq ($(CNXTDRIVER),hcf)
PPCPKG=$(wildcard ../hcfusbmodem*powerpc*.tar.gz)
endif

RELTAR = $(shell if [ -f "$(CNXTTARGET)modem-$(CNXTLINUXVERSION).tar.gz" ]; then echo yes; else echo no; fi)
RELRPMS = $(shell if [ -d "packages/RPMS" ]; then echo yes; else echo no; fi)
RELDEBS = $(shell if [ -d "packages/DEBS" ]; then echo yes; else echo no; fi)

RELTARGETS = $(RELDIR)
ifeq ($(RELRPMS),yes)
RELTARGETS += resignrpms ziprpms
endif
ifeq ($(RELDEBS),yes)
RELTARGETS += zipdebs
endif

rel: $(RELTARGETS)
ifeq ($(RELTAR),yes)
	cp -p $(TARPKG) $(RELDIR)
endif
ifeq ($(RELRPMS),yes)
	cp -p $(ALLRPMSZIP) $(RELDIR)
endif
ifeq ($(RELDEBS),yes)
	cp -p $(ALLDEBSZIP) $(RELDIR)
endif
ifneq ($(PDFDOC),)
	cp -p $(PDFDOC) $(RELDIR)
endif
ifneq ($(PPCPKG),)
	cp -p $(PPCPKG) $(RELDIR)/..
endif
ifeq ($(IMPORTED_ARCH),i386)
	@if [ -e "$(EXTDIR)" ]; then cd $(EXTDIR) && rm -rf $(CNXTTARGET)modem-$(CNXTLINUXVERSION) && tar xzf $(RELDIR)/$(TARPKG); fi
endif

