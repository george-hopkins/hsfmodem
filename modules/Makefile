#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
# 
ifeq ($(KBUILD_BUILTIN)$(KBUILD_MODULES)$(KBUILD_CHECKSRC),)
TOP := ..
else
ifeq ($(obj),)
modroot := $(M)
else
modroot := $(obj)
endif
TOP := $(shell cd $(modroot)/.. && pwd)
WITHIN_KBUILD=yes
endif

include $(TOP)/config.mak

IMPORTED = $(TOP)/modules/imported
MACHINE_ARCH := $(shell if uname --hardware-platform >/dev/null 2>&1 && ! uname --hardware-platform | grep -q "unknown"; then uname --hardware-platform; else uname --machine | sed 's/^i.86$$/i386/'; fi)
ifneq ($(subst powerpc,ppc,$(IMPORTED_ARCH)),$(MACHINE_ARCH))
$(warning *** WARNING: Trying to compile kernel modules on a $(MACHINE_ARCH) system while the installed $(CNXTTARGET) driver package is for $(IMPORTED_ARCH), this is likely to fail... ***)
endif

EXTRA_CFLAGS += -I$(IMPORTED)/include -I$(TOP)/modules/include -I$(TOP)/modules/GPL
EXTRA_CFLAGS += $(IMPORTED_COMPILATION_FLAGS)
EXTRA_CFLAGS += $(IMPORTED_FRAMEWORK_DEFS) -imacros $(IMPORTED)/include/framewrk.h
EXTRA_CFLAGS += $(IMPORTED_GENERAL_DEFS)
EXTRA_CFLAGS += $(IMPORTED_DEBUG_DEFS)

EXTRA_CFLAGS += $(OTHER_INCLUDES)
EXTRA_CFLAGS += $(DISTRO_CFLAGS)

EXTRA_CFLAGS += -DOS_NOREDEFS

ifeq ($(CNXT_MODS_DIR),)
ON_BUILD_SYSTEM := no
else
ON_BUILD_SYSTEM := yes
endif

EXTRA_CFLAGS += -DCNXTLINUXVERSION_NODOT=$(subst .,,$(CNXTLINUXVERSION))

# Configure location of current 2.4 kernel source directory for compiling
CNXT_KERNELSRC := /lib/modules/$(shell uname -r)/build

KERNELSRC_EXISTS := $(shell test -r ${CNXT_KERNELSRC}/include/linux/version.h && echo yes || echo no)

DEBIAN_SYSTEM := $(shell test -f /etc/debian_version && echo yes || echo no)

ifneq ($(KERNELSRC_EXISTS),yes)
ifeq ($(DEBIAN_SYSTEM),yes)
CNXT_KERNELSRC := /usr/src/kernel-headers-$(shell uname -r)
else
CNXT_KERNELSRC := /usr/src/linux
endif
KERNELSRC_EXISTS := $(shell test -r ${CNXT_KERNELSRC}/include/linux/version.h && echo yes || echo no)
endif

UTS_FILE := $(shell for hdr in linux/utsrelease.h generated/utsrelease.h linux/version.h; do test -r "${CNXT_KERNELSRC}/include/$${hdr}" && echo "${CNXT_KERNELSRC}/include/$${hdr}" && exit 0; done; exit 1)

KERNELVER := $(shell echo UTS_RELEASE | gcc -E ${DISTRO_CFLAGS} -I${CNXT_KERNELSRC}/include -include $(UTS_FILE) - | grep '^"' | tr -d '" 	')

KMODS_DIR := /lib/modules/$(KERNELVER)
CNXT_MODS_DIR := $(KMODS_DIR)/extra
# patsubst used here since CNXT_MODS_DIR may be overridden to point to /binaries/ directory without /extra or /updates.
CNXT_SND_MODS_DIR := $(patsubst %/extra,%/updates,$(CNXT_MODS_DIR))

# On SuSE 9.0 and up, as well as Debian 2.6.29-1-* kernels and up, CNXT_KERNELSRC points to an incomplete kernel source
# directory and some include files are not available in this tree
DIFF_OBJ_TREE := $(shell if grep -q '^\(KERNELSRC\)\|\(MAKEARGS\) ' "${CNXT_KERNELSRC}/Makefile" 2>/dev/null && [ -d "${CNXT_KERNELSRC}" ]; then echo "yes"; else echo "no"; fi)

ifeq ($(DIFF_OBJ_TREE),yes)
REAL_KERNELSRC := $(shell awk '{ if ($$1 == "MAKEARGS" && $$3 == "-C") print $$4; if ($$1 == "KERNELSRC") print $$3; }' "${CNXT_KERNELSRC}/Makefile")
# Deal with a relative path
REAL_KERNELSRC := $(if $(filter ../%, $(REAL_KERNELSRC)), $(CNXT_KERNELSRC)/,)$(REAL_KERNELSRC)
else
REAL_KERNELSRC := $(CNXT_KERNELSRC)
endif

KERNELARCH := $(subst i386,x86,$(subst x86_64,x86,$(IMPORTED_ARCH)))

# On some newer SuSE kernels, the include directory doesn't have the asm symlink, this symlink is in the include2 directory of the fake kernel tree instead
# Starting 2.6.28, most include/asm headers were moved to arch/<arch>/include/asm
REAL_KERNELSRC_INCLUDE_ASM := $(shell for p in $(CNXT_KERNELSRC)/include2/asm $(REAL_KERNELSRC)/arch/$(KERNELARCH)/include/asm; do if test -e $${p}; then echo $${p}; exit 0; fi; done; for p in arch/$(KERNELARCH)/include/asm include/asm; do if test -e $(CNXT_KERNELSRC)/$${p}; then echo $(REAL_KERNELSRC)/$${p}; exit 0; fi; done;)

ifeq ($(CNXTTARGET),hsf)
REPLACE_HDA := true
else
REPLACE_HDA := false
endif

# Configure compiler (on some systems, kgcc must be used to compile kernel code)
#
#CC= kgcc
#CC= gcc
CC := $(shell $(TOP)/modules/kernelcompiler.sh $(KERNELVER) $(CNXT_KERNELSRC))

FOUND_PCI_DEV_SLOT_NAME := $(shell grep -q '[ 	]slot_name' ${REAL_KERNELSRC}/include/linux/pci.h 2>/dev/null && echo -DFOUND_PCI_DEV_SLOT_NAME)
FOUND_DEVFS := $(shell [ -e "${REAL_KERNELSRC}/include/linux/devfs_fs_kernel.h" ] && echo -DFOUND_DEVFS)

EXTRA_CFLAGS+= $(FOUND_PCI_DEV_SLOT_NAME) $(FOUND_DEVFS)

KBUILD_SUPPORTED := $(shell test -e ${CNXT_KERNELSRC}/scripts/kconfig && echo yes || echo no)

ifeq ($(KBUILD_SUPPORTED),yes)
KO= ko
KBUILD_EXTMOD_SUPPORTED := $(shell egrep -q 'KBUILD_EXTMOD|KERNEL_SOURCE|KERNELSRC' ${CNXT_KERNELSRC}/Makefile 2>/dev/null && echo yes || echo no)

FOUND_CLASS_SIMPLE := $(shell grep -q 'class_simple_device_add' ${REAL_KERNELSRC}/include/linux/device.h 2>/dev/null && echo -DFOUND_CLASS_SIMPLE)
FOUND_UART_REGISTER_PORT := $(shell grep -q 'uart_register_port' ${REAL_KERNELSRC}/include/linux/serial_core.h 2>/dev/null && echo -DFOUND_UART_REGISTER_PORT)
FOUND_TTY_START_STOP := $(shell grep -q 'stop_tx.*tty_stop' ${REAL_KERNELSRC}/include/linux/serial_core.h 2>/dev/null && echo -DFOUND_TTY_START_STOP)
FOUND_CLASS_DEVICE_PARENT := $(shell grep -q 'struct[ \t]*class_device[ \t]*\*[ \t]*parent' ${REAL_KERNELSRC}/include/linux/device.h 2>/dev/null && echo -DFOUND_CLASS_DEVICE_PARENT)
FOUND_MODULE_PARAM :=  $(shell grep -q 'module_param' ${REAL_KERNELSRC}/include/linux/moduleparam.h 2>/dev/null && echo -DFOUND_MODULE_PARAM)
FOUND_PM_MESSAGE_T := $(shell grep -q 'pm_message_t' ${REAL_KERNELSRC}/include/linux/pci.h 2>/dev/null && echo -DFOUND_PM_MESSAGE_T)
FOUND_PM_MESSAGE_STRUCT := $(shell grep -q '^typedef struct pm_message' ${REAL_KERNELSRC}/include/linux/pm.h 2>/dev/null && echo -DFOUND_PM_MESSAGE_STRUCT)
FOUND_PCI_CHOOSE_STATE := $(shell grep -q 'pci_choose_state' ${REAL_KERNELSRC}/include/linux/pci.h 2>/dev/null && echo -DFOUND_PCI_CHOOSE_STATE)
FOUND_LINUX_SUSPEND := $(shell test -e ${REAL_KERNELSRC}/include/linux/suspend.h && echo -DFOUND_LINUX_SUSPEND)
FOUND_USB_DRIVER_OWNER := $(shell grep -q 'struct module \*owner' ${REAL_KERNELSRC}/include/linux/usb.h 2>/dev/null && echo -DFOUND_USB_DRIVER_OWNER)
FOUND_PCI_REGISTER_DRIVER := $(shell grep -q 'pci_register_driver' ${REAL_KERNELSRC}/include/linux/pci.h 2>/dev/null && echo -DFOUND_PCI_REGISTER_DRIVER)
FOUND_TTY_NEW_API := $(shell grep -q 'tty_buffer_request_room' ${REAL_KERNELSRC}/include/linux/tty_flip.h 2>/dev/null && echo -DFOUND_TTY_NEW_API)
FOUND_TOUCH_ATIME := $(shell grep -q 'touch_atime' ${REAL_KERNELSRC}/include/linux/fs.h 2>/dev/null && echo -DFOUND_TOUCH_ATIME)
FOUND_SEEK_MODES := $(shell grep -q 'SEEK_SET' ${REAL_KERNELSRC}/include/linux/fs.h 2>/dev/null && echo -DFOUND_SEEK_MODES)
FOUND_LINUX_CONFIG := $(shell test -e ${REAL_KERNELSRC}/include/linux/config.h && echo -DFOUND_LINUX_CONFIG)
FOUND_KERNEL_EXECVE := $(shell grep -q 'kernel_execve' ${REAL_KERNELSRC}/include/linux/syscalls.h 2>/dev/null && echo -DFOUND_KERNEL_EXECVE)
FOUND_IRQ_HANDLER_T := $(shell grep -q 'irq_handler_t' ${REAL_KERNELSRC}/include/linux/interrupt.h 2>/dev/null && echo -DFOUND_IRQ_HANDLER_T)
FOUND_USB_COMPLETE_PT_REGS := $(shell grep -q 'usb_complete_t.*pt_regs' ${REAL_KERNELSRC}/include/linux/usb.h 2>/dev/null && echo -DFOUND_USB_COMPLETE_PT_REGS)
FOUND_KTERMIOS := $(shell grep -q 'ktermios' ${REAL_KERNELSRC}/include/linux/serial_core.h 2>/dev/null && echo -DFOUND_KTERMIOS)
FOUND_PCI_GET_DEVICE := $(shell grep -q 'pci_get_device' ${REAL_KERNELSRC}/include/linux/pci.h 2>/dev/null && echo -DFOUND_PCI_GET_DEVICE)
FOUND_LINUX_SEMAPHORE := $(shell test -e  ${REAL_KERNELSRC}/include/linux/semaphore.h && echo -DFOUND_LINUX_SEMAPHORE)
FOUND_CLASS_DEVICE := $(shell grep -q 'class_device_create' ${REAL_KERNELSRC}/include/linux/device.h 2>/dev/null && echo -DFOUND_CLASS_DEVICE)
FOUND_THREAD_XSTATE := $(shell grep -q 'thread_xstate' ${REAL_KERNELSRC_INCLUDE_ASM}/processor.h 2>/dev/null && echo -DFOUND_THREAD_XSTATE)
FOUND_DEVICE_CREATE_DRVDATA := $(shell grep -q 'device_create_drvdata' ${REAL_KERNELSRC}/include/linux/device.h 2>/dev/null && echo -DFOUND_DEVICE_CREATE_DRVDATA)
PAREN := (
FOUND_DEVICE_CREATE_WITH_DRVDATA := $(shell grep -A3 'device_create$(PAREN)' ${REAL_KERNELSRC}/include/linux/device.h | grep -q drvdata && echo -DFOUND_DEVICE_CREATE_WITH_DRVDATA)
FOUND_STRUCT_TTY_PORT := $(shell grep -q 'tty_port' ${REAL_KERNELSRC}/include/linux/serial_core.h 2>/dev/null && echo -DFOUND_STRUCT_TTY_PORT)
FOUND_NO_STRUCT_UART_INFO := $(shell [ -r ${REAL_KERNELSRC}/include/linux/serial_core.h ] && ! grep -q 'uart_info' ${REAL_KERNELSRC}/include/linux/serial_core.h 2>/dev/null && echo -DFOUND_NO_STRUCT_UART_INFO)
FOUND_CURRENT_CRED := $(shell grep -q 'struct cred' ${REAL_KERNELSRC}/include/linux/sched.h 2>/dev/null && echo -DFOUND_CURRENT_CRED)
FOUND_LINUX_BYTEORDER_SWAB := $(shell test -e ${REAL_KERNELSRC}/include/linux/byteorder/swab.h 2>/dev/null && echo -DFOUND_LINUX_BYTEORDER_SWAB)
FOUND_DEV_NAME := $(shell grep -q 'dev_name' ${REAL_KERNELSRC}/include/linux/device.h 2>/dev/null && echo -DFOUND_DEV_NAME)

EXTRA_CFLAGS+= $(FOUND_CLASS_SIMPLE) $(FOUND_UART_REGISTER_PORT) $(FOUND_TTY_START_STOP) $(FOUND_CLASS_DEVICE_PARENT) $(FOUND_MODULE_PARAM) $(FOUND_PM_MESSAGE_T) $(FOUND_PM_MESSAGE_STRUCT) $(FOUND_PCI_CHOOSE_STATE) $(FOUND_LINUX_SUSPEND) $(FOUND_USB_DRIVER_OWNER) $(FOUND_PCI_REGISTER_DRIVER) $(FOUND_TTY_NEW_API) $(FOUND_TOUCH_ATIME) $(FOUND_SEEK_MODES) $(FOUND_LINUX_CONFIG) $(FOUND_KERNEL_EXECVE) $(FOUND_IRQ_HANDLER_T) $(FOUND_USB_COMPLETE_PT_REGS) $(FOUND_KTERMIOS) $(FOUND_PCI_GET_DEVICE) $(FOUND_LINUX_SEMAPHORE) $(FOUND_CLASS_DEVICE) $(FOUND_THREAD_XSTATE) $(FOUND_DEVICE_CREATE_DRVDATA) $(FOUND_DEVICE_CREATE_WITH_DRVDATA) $(FOUND_STRUCT_TTY_PORT) $(FOUND_CURRENT_CRED) $(FOUND_LINUX_BYTEORDER_SWAB) $(FOUND_DEV_NAME) $(FOUND_NO_STRUCT_UART_INFO)

FOUND_KZALLOC := $(shell grep -q 'kzalloc' ${REAL_KERNELSRC}/include/linux/slab.h 2>/dev/null && echo -DFOUND_KZALLOC)
FOUND_OPEN_SUBSTREAM_NOFILE := $(shell grep -q 'snd_pcm_open_substream.*struct[ \t]*file' ${REAL_KERNELSRC}/include/sound/pcm.h 2>/dev/null || echo -DFOUND_OPEN_SUBSTREAM_NOFILE)
FOUND_TLV := $(shell test -e ${REAL_KERNELSRC}/include/sound/tlv.h 2>/dev/null && echo -DFOUND_TLV)
FOUND_SND_PCM_HW_PARAM_SET := $(shell grep -q '_snd_pcm_hw_param_set' ${REAL_KERNELSRC}/include/sound/pcm_params.h 2>/dev/null && echo -DFOUND_SND_PCM_HW_PARAM_SET)
FOUND_READ_SIZE := $(shell grep -q 'long read_size' ${REAL_KERNELSRC}/include/sound/info.h 2>/dev/null && echo -DFOUND_READ_SIZE)
FOUND_DELAYED_WORK := $(shell grep -q 'INIT_DELAYED_WORK' ${REAL_KERNELSRC}/include/linux/workqueue.h 2>/dev/null && echo -DFOUND_DELAYED_WORK)
FOUND_SOUND_TYPEDEFS := $(shell test -s ${REAL_KERNELSRC}/include/sound/typedefs.h && echo -DFOUND_SOUND_TYPEDEFS)
FOUND_NO_CTL_ELEM_RW := $(shell test -r ${REAL_KERNELSRC}/Module.symvers && grep -q snd_ctl_new ${REAL_KERNELSRC}/Module.symvers && ! grep -q snd_ctl_elem_read ${REAL_KERNELSRC}/Module.symvers && echo -DFOUND_NO_CTL_ELEM_RW)

HDA_CFLAGS := $(FOUND_KZALLOC) $(FOUND_OPEN_SUBSTREAM_NOFILE) $(FOUND_TLV) $(FOUND_SND_PCM_HW_PARAM_SET) $(FOUND_READ_SIZE) $(FOUND_IRQ_HANDLER_T) $(FOUND_DELAYED_WORK) $(FOUND_SOUND_TYPEDEFS) $(FOUND_NO_CTL_ELEM_RW)

ifeq ($(ON_BUILD_SYSTEM)-$(CONFIG_SND_HDA_INTEL),no-)
# On Ubuntu Gutsy (7.10), the HDA modules are not included in linux-image but in a seprarate package
# and CONFIG_SND_HDA_INTEL is not enabled in the kernel .config file
CONFIG_SND_HDA_INTEL=$(shell modprobe -n snd-hda-intel > /dev/null 2>&1 && echo y || echo n)
endif

MODULAR_HDA := $(shell test -e  ${REAL_KERNELSRC}/include/sound/hda_codec.h && echo yes || echo no)

else
KO= o

# Configure various machine specific CFLAGS for compiling kernel modules
#EXTRA_CFLAGS+= -mpreferred-stack-boundary=2
#EXTRA_CFLAGS+= -fomit-frame-pointer
EXTRA_CFLAGS += -Wall -Wstrict-prototypes -Werror-implicit-function-declaration -O2 -I$(CNXT_KERNELSRC)/include
EXTRA_CFLAGS += -D__KERNEL__ -DMODULE

CONFIG_MODVERSIONS= $(shell echo CONFIG_MODVERSIONS | gcc ${DISTRO_CFLAGS} -E -I${CNXT_KERNELSRC}/include -include ${CNXT_KERNELSRC}/include/linux/config.h - | grep -q 'CONFIG_MODVERSIONS' && echo n || echo y)
ifeq ($(CONFIG_MODVERSIONS),y)
MODVERFLAGS= -DMODVERSIONS -include $(CNXT_KERNELSRC)/include/linux/modversions.h
endif
CONFIG_USB= $(shell echo 'CONFIG_USB CONFIG_USB_MODULE' | gcc ${DISTRO_CFLAGS} -E -I${CNXT_KERNELSRC}/include -include ${CNXT_KERNELSRC}/include/linux/config.h - | grep -q 'CONFIG_USB CONFIG_USB_MODULE' && echo n || echo y)
CONFIG_SND_HDA_INTEL=$(shell echo 'CONFIG_SND_HDA_INTEL CONFIG_SND_HDA_INTEL_MODULE' | gcc ${DISTRO_CFLAGS} -E -I${CNXT_KERNELSRC}/include -include ${CNXT_KERNELSRC}/include/linux/config.h - | grep -q 'CONFIG_SND_HDA_INTEL CONFIG_SND_HDA_INTEL_MODULE' && echo n || echo y)

EXTRA_CFLAGS+= -include $(CNXT_KERNELSRC)/include/linux/config.h
EXTRA_CFLAGS+= $(MODVERFLAGS)
EXTRA_CFLAGS+= -DFOUND_TTY_START_STOP -DFOUND_LINUX_BYTEORDER_SWAB
endif # KBUILD_SUPPORTED

paren := (
FOUND_KILL_PROC := $(shell grep -q 'kill_proc$(paren)' ${REAL_KERNELSRC}/include/linux/sched.h 2>/dev/null && echo -DFOUND_KILL_PROC)
EXTRA_CFLAGS+= $(FOUND_KILL_PROC)

## END OF CONFIGURATION SECTION
## no changes should be necessary beyond this point
#####################################################################

EXTRA_CFLAGS+= -DCNXTTARGET="\"$(CNXTTARGET)\""
EXTRA_CFLAGS+= -DCNXTDRIVER="\"$(CNXTDRIVER)\""
EXTRA_CFLAGS+= -DCNXTDRVDSC="\"$(CNXTDRVDSC)\""

ifneq ($(WITHIN_KBUILD),yes)
.PHONY: default
default: all
endif

CC_SHOW := $(shell echo $(CC) | tr '[a-z]' '[A-Z]')
LD_SHOW := $(shell echo $(LD) | tr '[a-z]' '[A-Z]')

ifneq ($(WITHIN_KBUILD),yes)
.c.o:
ifneq ($(KERNELSRC_EXISTS),yes)
ifeq ($(DEBIAN_SYSTEM),yes)
	$(error CNXT_KERNELSRC does not point to a proper directory ($(CNXT_KERNELSRC)); You may need to install kernel-headers matching your kernel, perhaps by running "apt-get install kernel-headers-$(shell uname -r)")
else
	$(error CNXT_KERNELSRC does not point to a proper directory ($(CNXT_KERNELSRC)); you may need to install kernel-source or kernel-headers)
endif
endif
ifeq ($(V),1)
	$(COMPILE.c) $(CFLAGS_$(@F)) $(OUTPUT_OPTION) $<
else
	@echo ""; echo $(CC_SHOW) $(CFLAGS_$(@F)) $(OUTPUT_OPTION) $<
	@$(COMPILE.c) $(EXTRA_CFLAGS) $(CFLAGS_$(@F)) $(OUTPUT_OPTION) $<
endif

CLEAN= *.o GPL/*.o *.ko GPL/*.ko *.mod.c GPL/*.mod.c .*.cmd GPL/.*.cmd .tmp_versions
ifeq ($(KBUILD_SUPPORTED),yes)
CLEAN+= .tmp_versions $(patsubst %, ${CNXT_KERNELSRC}/.tmp_versions/%, $(ALL_MODS:.$(KO)=.mod)) Modules.symvers GPL/hda/Modules.symvers Module.symvers GPL/hda/Module.symvers modules.order GPL/hda/modules.order Module.markers GPL/hda/Module.markers
endif

.PHONY: clean
clean::
ifeq ($(KBUILD_SUPPORTED),yes)
ifeq ($(KBUILD_EXTMOD_SUPPORTED),yes)
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "M=$(MODULES_SRC_DIR)" "CC=$(CC)" clean)
ifeq ($(CNXTDRIVER),hsf)
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "M=$(MODULES_SRC_DIR)/GPL/hda" "CC=$(CC)" "HDA_CFLAGS=$(HDA_CFLAGS)" clean)
endif
else
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "SUBDIRS+=$(MODULES_SRC_DIR)" "CC=$(CC)" clean)
ifeq ($(CNXTDRIVER),hsf)
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "SUBDIRS+=$(MODULES_SRC_DIR)/GPL/hda" "CC=$(CC)" "HDA_CFLAGS=$(HDA_CFLAGS)" clean)
endif
endif
endif
	rm -rf $(CLEAN)
endif



ifeq ($(CNXTDRIVER),dgc)
ALL_MODS= $(CNXTTARGET)usbdcp.$(KO)
else
PRP_MODS= $(CNXTTARGET)engine.$(KO)
ALL_MODS= $(CNXTTARGET)osspec.$(KO) $(CNXTTARGET)serial.$(KO) $(PRP_MODS)
endif

ifeq ($(CNXTDRIVER),hcf)
ALL_MODS+= $(CNXTTARGET)hw.$(KO)
endif
ifeq ($(CNXTDRIVER),hsf)
PRP_MODS+= $(CNXTTARGET)pcibasic2.$(KO)
PRP_MODS+= $(CNXTTARGET)pcibasic3.$(KO)
ifneq ($(CONFIG_USB),)
ifneq ($(CONFIG_USB),n)
PRP_MODS+= $(CNXTTARGET)usbcd2.$(KO)
endif
ifeq ($(MODULAR_HDA),yes)
PRP_MODS+= snd-hda-codec-hsfmodem.$(KO)
endif
endif
ifneq ($(CONFIG_SND_HDA_INTEL),)
ifneq ($(CONFIG_SND_HDA_INTEL),n)
PRP_MODS+= $(CNXTTARGET)hda.$(KO)
endif
endif
PRP_MODS+= $(CNXTTARGET)mc97ich.$(KO) $(CNXTTARGET)mc97via.$(KO) $(CNXTTARGET)mc97ali.$(KO) $(CNXTTARGET)mc97ati.$(KO) $(CNXTTARGET)mc97sis.$(KO)
PRP_MODS+= $(CNXTTARGET)soar.$(KO)
endif

OSSPEC_OBJS = osservices.o osstdio.o osnvm.o osresour.o osstring.o osmemory.o osdiag.o
ifeq ($(CNXTTARGET),hcfusb)
OSSPEC_OBJS+= osusb.o
endif
ifeq ($(CNXTTARGET),hsf)
OSSPEC_OBJS+= osusb.o osfloat.o osdcp.o
endif
ifeq ($(IMPORTED_SCR_SUPPORT),yes)
OSSPEC_OBJS+= osscr.o cdbgscr.o imported/$(CNXTDRIVER)scr-$(IMPORTED_ARCH).O
endif
ifeq ($(IMPORTED_BLAM_SUPPORT),yes)
OSSPEC_OBJS+= imported/$(CNXTDRIVER)blam-$(IMPORTED_ARCH).O
endif

CFLAGS_osnvm.o += -DCNXTSBINDIR=\"$(CNXTSBINDIR)\" -DCNXTNVMDIR=\"$(CNXTNVMDIR)\"
CFLAGS_osdcp.o += -DCNXTSBINDIR=\"$(CNXTSBINDIR)\" -DCNXTDCPMAJOR=$(CNXTDCPMAJOR)
CFLAGS_osdiag.o += -DCNXTDIAGMAJOR=$(CNXTDIAGMAJOR) -DCNXTDIAGDMPMINOR=$(CNXTDIAGDMPMINOR)
CFLAGS_osscr.o += -DCNXTSCRMAJOR=$(CNXTSCRMAJOR)

ifneq ($(WITHIN_KBUILD),yes)
ifeq ($(KBUILD_SUPPORTED),yes)
MODULES_SRC_DIR=$(shell pwd)

all:
ifeq ($(KBUILD_EXTMOD_SUPPORTED),yes)
	(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "M=$(MODULES_SRC_DIR)" "CC=$(CC)" modules)
ifeq ($(REPLACE_HDA)|$(MODULAR_HDA),true|no)
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "M=$(MODULES_SRC_DIR)/GPL/hda" "CC=$(CC)" "HDA_CFLAGS=$(HDA_CFLAGS)" modules)
endif
else
	(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "SUBDIRS+=$(MODULES_SRC_DIR)" "CC=$(CC)" modules)
ifeq ($(REPLACE_HDA)|$(MODULAR_HDA),true|no)
	-(cd ${CNXT_KERNELSRC} && make "CNXT_KERNELSRC=${CNXT_KERNELSRC}" "SUBDIRS+=$(MODULES_SRC_DIR)/GPL/hda" "CC=$(CC)" "HDA_CFLAGS=$(HDA_CFLAGS)" modules)
endif
endif
else
all: $(ALL_MODS)
endif
else

obj-m := $(ALL_MODS:.$(KO)=.o)

$(CNXTTARGET)serial-objs := GPL/serial_cnxt.o

$(CNXTTARGET)engine-objs := mod_engine.o imported/$(CNXTDRIVER)engine-$(IMPORTED_ARCH).O
$(CNXTTARGET)pcibasic2-objs := mod_pcibasic2.o imported/$(CNXTDRIVER)pcibasic2-$(IMPORTED_ARCH).O
$(CNXTTARGET)pcibasic3-objs := mod_pcibasic3.o imported/$(CNXTDRIVER)pcibasic3-$(IMPORTED_ARCH).O
$(CNXTTARGET)mc97ich-objs := mod_mc97ich.o imported/$(CNXTDRIVER)mc97ich-$(IMPORTED_ARCH).O
$(CNXTTARGET)mc97via-objs := mod_mc97via.o imported/$(CNXTDRIVER)mc97via-$(IMPORTED_ARCH).O
$(CNXTTARGET)mc97ali-objs := mod_mc97ali.o imported/$(CNXTDRIVER)mc97ali-$(IMPORTED_ARCH).O
$(CNXTTARGET)mc97ati-objs := mod_mc97ati.o imported/$(CNXTDRIVER)mc97ati-$(IMPORTED_ARCH).O
$(CNXTTARGET)mc97sis-objs := mod_mc97sis.o imported/$(CNXTDRIVER)mc97sis-$(IMPORTED_ARCH).O
$(CNXTTARGET)usbcd2-objs := mod_usbcd2.o imported/$(CNXTDRIVER)usbcd2-$(IMPORTED_ARCH).O
$(CNXTTARGET)hda-objs := mod_hda.o imported/$(CNXTDRIVER)hda-$(IMPORTED_ARCH).O
$(CNXTTARGET)soar-objs := mod_soar.o imported/$(CNXTDRIVER)soar-$(IMPORTED_ARCH).O

$(CNXTTARGET)hw-objs := mod_$(CNXTTARGET).o

$(CNXTTARGET)osspec-objs := mod_osspec.o $(OSSPEC_OBJS)

$(CNXTTARGET)usbdcp-objs := mod_$(CNXTTARGET)usbdcp.o osdcp.o
endif

CFLAGS_mod_osspec.o = -DEXPORT_SYMTAB
CFLAGS_mod_engine.o = -DEXPORT_SYMTAB
ifeq ($(CNXTDRIVER),hcf)
CFLAGS_mod_$(CNXTTARGET).o = -DEXPORT_SYMTAB
endif
ifeq ($(CNXTDRIVER),hsf)
CFLAGS_mod_pcibasic2.o = -DEXPORT_SYMTAB
CFLAGS_mod_pcibasic3.o = -DEXPORT_SYMTAB
CFLAGS_mod_mc97ich.o = -DEXPORT_SYMTAB
CFLAGS_mod_mc97via.o = -DEXPORT_SYMTAB
CFLAGS_mod_mc97ali.o = -DEXPORT_SYMTAB
CFLAGS_mod_mc97ati.o = -DEXPORT_SYMTAB
CFLAGS_mod_mc97sis.o = -DEXPORT_SYMTAB
CFLAGS_mod_usbcd2.o = -DEXPORT_SYMTAB
CFLAGS_mod_hda.o = -DEXPORT_SYMTAB
CFLAGS_mod_soar.o = -DEXPORT_SYMTAB
endif
ifeq ($(CNXTDRIVER),dgc)
CFLAGS_mod_dgcusbdcp.o = -DEXPORT_SYMTAB
endif


NON_ATOMIC_TTY_STRUCT_COUNT := $(shell grep -A 15 'struct tty_struct {' ${CNXT_KERNELSRC}/include/linux/tty.h 2>/dev/null | grep -q 'int count;' && echo -DNON_ATOMIC_TTY_STRUCT_COUNT)

CFLAGS_serial_cnxt.o = -DEXPORT_SYMTAB
CFLAGS_serial_cnxt.o+= -DCNXTSERIALMAJOR=$(CNXTSERIALMAJOR) -DCNXTCALOUTMAJOR=$(CNXTCALOUTMAJOR) -DCNXTSERIALMINOR=$(CNXTSERIALMINOR)
CFLAGS_serial_cnxt.o+= -DCNXTSERDEV="\"$(CNXTSERDEV)\""
CFLAGS_serial_cnxt.o+= -DCNXTMAXMDM="$(CNXTMAXMDM)"
CFLAGS_serial_cnxt.o+= $(NON_ATOMIC_TTY_STRUCT_COUNT)

ifneq ($(WITHIN_KBUILD),yes)
CFLAGS_serial_cnxt.o+= -DCNXTSERIAL_INCLUDE_CORE -DFOUND_UART_REGISTER_PORT

GPL/serial_cnxt.o: GPL/serial_core.c GPL/serial_core.h

ifeq ($(V),1)
define kmodlink
	$(LD) -r -o $@ $^
endef
else
define kmodlink
	@echo ""; echo $(LD_SHOW) -r -o $@ $^
	@$(LD) -r -o $@ $^
endef
endif

$(CNXTTARGET)serial.$(KO): GPL/serial_cnxt.o
	$(kmodlink)

ifneq ($(PRP_MODS),)
$(PRP_MODS): $(CNXTTARGET)%.$(KO): mod_%.o $(IMPORTED)/$(CNXTDRIVER)%-$(IMPORTED_ARCH).O
	$(kmodlink)
endif

ifeq ($(CNXTDRIVER),hcf)
$(CNXTTARGET)hw.$(KO): mod_$(CNXTTARGET).o
	$(kmodlink)
endif

$(CNXTTARGET)osspec.$(KO): mod_osspec.o $(OSSPEC_OBJS)
	$(kmodlink)

ifeq ($(CNXTDRIVER),dgc)
$(CNXTTARGET)usbdcp.$(KO): mod_dgcusbdcp.o osdcp.o
	$(kmodlink)
endif

.PHONY: check_kernelver modules_install minstall mi
check_kernelver:
	@if [ -z "$(KERNELVER)" ]; then \
		echo 1>&2 "Unable to determine version of kernel source directory $(CNXT_KERNELSRC)"; false; \
	else \
		true; \
	fi

ifeq ($(KO),ko)
KERNCOMPVER := $(shell sed -n -e '/gcc version /s/gcc version //p' ${CNXT_KERNELSRC}/kernelcompiler 2>/dev/null)
endif

ifeq ($(CNXTDRIVER),hsf)
modules_install minstall mi: check_kernelver all $(CNXT_MODS_DIR) $(CNXT_SND_MODS_DIR)
else
modules_install minstall mi: check_kernelver all $(CNXT_MODS_DIR)
endif
ifneq ($(KERNCOMPVER),)
	for m in "$(CNXTTARGET)"*.ko GPL/hda/*.ko; do \
		[ -r $$m ] || continue; \
		sed 's@\(vermagic=.* \)gcc-[0-9]\.[0-9]@\1gcc-$(KERNCOMPVER)@g' < $$m > $$m.tmp && mv $$m.tmp $$m; \
		strip --strip-debug $$m; \
	done
endif
	for m in "$(CNXTTARGET)"*.ko GPL/hda/*.ko; do \
		[ -r $$m ] || continue; \
		objcopy --rename-section .ctors=.ctors_not_used $$m; \
	done
	rm -f "$(CNXT_MODS_DIR)/$(CNXTTARGET)"*.$(KO)
ifeq ($(KBUILD_SUPPORTED),yes)
	$(INSTALL) -m 644 "$(CNXTTARGET)"*.ko $(CNXT_MODS_DIR)
ifeq ($(REPLACE_HDA)|$(MODULAR_HDA),true|no)
	-$(INSTALL) -m 644 GPL/hda/*.ko $(CNXT_SND_MODS_DIR)
endif
ifeq ($(MODULAR_HDA),yes)
	$(INSTALL) -m 644 "snd-hda-codec-$(CNXTTARGET)modem.ko" $(CNXT_MODS_DIR)
endif
else
	$(INSTALL) -m 644 $(ALL_MODS) $(CNXT_MODS_DIR)
endif

.PHONY: install uninstall

install: uninstall $(CNXTMODDIR)
	(cd $(TOP) && find config.mak modules/imported -depth -print | cpio -pdmu $(CNXTLIBDIR))
	find . \( -name COPYING -o -name '*.sh' -o -name '*.[ch]' -o -name '*.mak' -o -name '[Mm]akefile' \) -print | cpio -pdmu $(CNXTMODDIR)
	find binaries -depth -print | cpio -pdmu $(CNXTMODDIR)

$(CNXT_MODS_DIR) $(CNXT_SND_MODS_DIR) $(CNXTMODDIR):
	$(MKDIR) -p $@

uninstall:
	rm -rf "$(CNXTLIBDIR)/config.mak" "$(CNXTLIBDIR)/modules/imported" "$(CNXTMODDIR)"
endif

ifeq ($(CNXTTARGET),hcfpci)
mod_$(CNXTTARGET).o: cnxthw_common.c cnxthwpci_common.c
endif
ifeq ($(CNXTTARGET),hcfusb)
mod_$(CNXTTARGET).o: cnxthw_common.c cnxthwusb_common.c
endif

ifeq ($(CNXTDRIVER),hsf)
mod_mc97sis.o mod_mc97ati.o mod_mc97ali.o mod_mc97ich.o mod_mc97via.o mod_pcibasic2.o mod_pcibasic3.o: cnxthw_common.c cnxthwpci_common.c

mod_usbcd2.o: cnxthw_common.c cnxthwusb_common.c

mod_hda.o: cnxthw_common.c cnxthwhda_common.c
endif

