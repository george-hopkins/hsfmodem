IMPORTED_ARCH ?= $(shell uname -i)

include $(TOP)/modules/imported/makeflags-$(IMPORTED_ARCH).mak
