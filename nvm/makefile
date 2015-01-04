#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
# 
TOP= ..

include $(TOP)/config.mak

NVMINFDIR=inf

# NVMCVTDIR can be specified in the environment for caching
# while building multiple packages.
NVMCVTDIR:=$(shell echo $$NVMCVTDIR)
ifeq ($(NVMCVTDIR),)
NVMCVTDIR=cvt
CLEANCVTDIR=true
endif

NVMDIRS_HCFPCI= v90 v90lan v92 v92smart riptide
NVMDIRS_HCFUSB= cadmus2
NVMDIRS_HCFUSB_BIGENDIAN= cadmus2-bigendian
NVMDIRS_HSF= pcibasic2 pcibasic2smart pcibasic2hsfi pcibasic2bry pcibasic3 mc97 mc97ali mc97ati mc97ich mc97sis mc97via cadmus2 cadmus2smart hda

ifeq ($(IMPORTED_TARGET),TARGET_HCF_PCI_LINUX)
NVMDIRS= $(NVMDIRS_HCFPCI)
endif
ifeq ($(IMPORTED_TARGET),TARGET_HCF_USB_LINUX)
ifeq ($(IMPORTED_ARCH),powerpc)
NVMDIRS= $(NVMDIRS_HCFUSB_BIGENDIAN)
else
NVMDIRS= $(NVMDIRS_HCFUSB)
endif
NVMDIRS_ALL= $(NVMDIRS_HCFUSB) $(NVMDIRS_HCFUSB_BIGENDIAN)
endif
ifeq ($(NVMDIRS),) # HSF
NVMDIRS= $(NVMDIRS_HSF)
CTYLNK= pcibasic2
else
CTYLNK= v90
endif
ifeq ($(NVMDIRS_ALL),)
NVMDIRS_ALL= $(NVMDIRS)
endif

NVMDIRS_CVT=$(patsubst %, $(NVMCVTDIR)/$(CNXTTARGET)%, $(NVMDIRS))

ALL= $(NVMCVTDIR) $(NVMDIRS_CVT) $(NVMCVTDIR)/.linksame

all: $(ALL)

$(NVMCVTDIR)/.linksame: $(NVMDIRS_CVT)
	cd $(NVMCVTDIR) && find . -type f ! -empty -exec md5sum {} ';' | sort | \
	while read sum file ; do \
		if [ "$$sum" = "$$prevsum" ] && cmp -s "$$file" "$$prevfile"; then \
			rm -f "$$file"; \
			if ! ln "$$prevfile" "$$file"; then \
				echo 2>&1 "$$0: ln FAILED - recreate $$file based on $$prevfile"; \
				exit 1; \
			fi; \
		else \
			prevsum="$$sum"; \
			prevfile="$$file"; \
		fi; \
	done
	touch $(NVMCVTDIR)/.linksame

install: all $(CNXTNVMDIR)
	cd $(NVMCVTDIR) && (find $(patsubst %, $(CNXTTARGET)%, $(NVMDIRS)) | cpio -pdmu $(CNXTNVMDIR))

$(NVMCVTDIR) $(CNXTNVMDIR):
	$(MKDIR) -p $@

uninstall:
	rm -rf $(patsubst %, "$(CNXTNVMDIR)/$(CNXTTARGET)%", $(NVMDIRS))

clean:
ifneq ($(CLEANCVTDIR),)
	rm -rf $(NVMCVTDIR)
endif
ifeq ($(CNXTDRIVER),hcf)
	rm -rf $(NVMINFDIR)/hcfpci.cty $(NVMINFDIR)/hcfpcismart.cty $(NVMINFDIR)/hcfusb.cty
endif

define inftonvm
	sed \
		-e 's!@CNXTLINUXVERSION@!$(CNXTLINUXVERSION)!g' \
		-e 's!@CNXTTARGET@!$(CNXTTARGET)!g' \
		-e 's!@CNXTDRVDSC@!$(CNXTDRVDSC)!g' \
		< $< | ./cvtinf.pl $@; \
	if [ -n "$(word 2,$+)" ]; then ./cvtinf.pl $@ < "$(word 2,$+)"; else true; fi
endef

# Non-approved countries are excluded from COUNTRY_CODE_LIST
define mkctycodelist
	(cd $@/Region && grep -v -l '\*' *NAME | sed -e 's/^0//' -e 's/^0//' -e 's/_NAME$$//' | tr '\012' ',' | sed -e 's/,$$//' -e 's/^/"/' -e 's/$$/"/' ; echo "") > $@/COUNTRY_CODE_LIST
endef

$(NVMCVTDIR)/$(CNXTTARGET)%-bigendian: $(NVMINFDIR)/$(CNXTTARGET)%.inf $(NVMINFDIR)/$(CNXTTARGET).cty
	CVTINF_SWAP=1; export CVTINF_SWAP; $(inftonvm)
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK): $(NVMINFDIR)/$(CNXTTARGET)$(CTYLNK).inf $(NVMINFDIR)/$(CNXTTARGET).cty
	$(inftonvm)
	$(mkctycodelist)
#ifeq ($(CNXTDRIVER),hsf)
#	[ -e $(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK)/Region/0042 ] && (rm -rf $(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK)/Region/0042*; ln -sf 0004 $(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK)/Region/0042 && ln -sf 0004_NAME $(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK)/Region/0042_NAME)
#endif

$(NVMCVTDIR)/$(CNXTTARGET)v92smart: $(NVMINFDIR)/$(CNXTTARGET)v92smart.inf $(NVMINFDIR)/$(CNXTTARGET)smart.cty
	$(inftonvm)
	$(mkctycodelist)

ifeq ($(CNXTDRIVER),hcf)
$(NVMCVTDIR)/$(CNXTTARGET)cadmus2: $(NVMINFDIR)/$(CNXTTARGET)cadmus2.inf $(NVMINFDIR)/$(CNXTTARGET).cty
	$(inftonvm)
	$(mkctycodelist)
else
$(NVMCVTDIR)/$(CNXTTARGET)cadmus2: $(NVMINFDIR)/$(CNXTTARGET)cadmus2.inf
	$(inftonvm)
	rm -f cadmus2/HW_ADAPTER_TYPE
	ln -sf ../$(CNXTTARGET)pcibasic2/Profile $@/.
	ln -sf ../$(CNXTTARGET)pcibasic2/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)cadmus2smart: $(NVMINFDIR)/$(CNXTTARGET)cadmus2smart.inf
	$(inftonvm)
	rm -f cadmus2smart/HW_ADAPTER_TYPE
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Profile $@/.
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Region $@/.
	$(mkctycodelist)
endif

$(NVMCVTDIR)/$(CNXTTARGET)mc97: $(NVMINFDIR)/$(CNXTTARGET)mc97ich.inf
	$(inftonvm)
	rm -f mc97/HW_ADAPTER_TYPE
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Profile $@/.
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)mc97ali $(NVMCVTDIR)/$(CNXTTARGET)mc97ati $(NVMCVTDIR)/$(CNXTTARGET)mc97ich $(NVMCVTDIR)/$(CNXTTARGET)mc97sis $(NVMCVTDIR)/$(CNXTTARGET)mc97via: $(NVMCVTDIR)/$(CNXTTARGET)mc97
	ln -sf `basename $<` $@

$(NVMCVTDIR)/$(CNXTTARGET)hda: $(NVMINFDIR)/$(CNXTTARGET)hda.inf
	$(inftonvm)
	rm -f hda/HW_ADAPTER_TYPE
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Profile $@/.
	ln -sf ../$(CNXTTARGET)pcibasic2smart/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)%: $(NVMINFDIR)/$(CNXTTARGET)%.inf
	$(inftonvm)
	if [ -d $(NVMCVTDIR)/$(CNXTTARGET)$(CTYLNK)/Profile ]; then ln -sf ../$(CNXTTARGET)$(CTYLNK)/Profile $@/.; else true; fi
	ln -sf ../$(CNXTTARGET)$(CTYLNK)/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)pcibasic2hsfi: $(NVMINFDIR)/$(CNXTTARGET)pcibasic2hsfi.inf $(NVMINFDIR)/$(CNXTTARGET).cty
	$(inftonvm)
	rm -rf $@/Region
	ln -sf ../$(CNXTTARGET)$(CTYLNK)/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)pcibasic2bry: $(NVMINFDIR)/$(CNXTTARGET)pcibasic2bry.inf $(NVMINFDIR)/$(CNXTTARGET).cty
	$(inftonvm)
	rm -rf $@/Region
	ln -sf ../$(CNXTTARGET)$(CTYLNK)/Region $@/.
	$(mkctycodelist)

$(NVMCVTDIR)/$(CNXTTARGET)pcibasic3: $(NVMINFDIR)/$(CNXTTARGET)pcibasic3.inf $(NVMINFDIR)/$(CNXTTARGET).cty
	$(inftonvm)
	rm -rf $@/Region
	ln -sf ../$(CNXTTARGET)$(CTYLNK)/Region $@/.
	$(mkctycodelist)

$(NVMINFDIR)/hcfpci.cty: $(NVMINFDIR)/HCFCTY.mst
	tr -d '\015' < $< | tr '\\' '@' | cpp -DDAA_STANDARD -DINT_PCI -P | tr '@' '\\' | grep -v '^[ 	]*$$' > $@

$(NVMINFDIR)/hcfpcismart.cty: $(NVMINFDIR)/HCFCTY.mst
	tr -d '\015' < $< | tr '\\' '@' | cpp -DDAA_SMART -DINT_PCI -P | tr '@' '\\' | grep -v '^[ 	]*$$' > $@

$(NVMINFDIR)/hcfusb.cty: $(NVMINFDIR)/HCFCTY.mst
	tr -d '\015' < $< | tr '\\' '@' | cpp -DDAA_STANDARD -DINT_USB -P | tr '@' '\\' | grep -v '^[ 	]*$$' > $@
