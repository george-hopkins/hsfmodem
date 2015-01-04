/*
 * Copyright (c) 2003-2004 Linuxant inc.
 *
 * NOTE: The use and distribution of this software is governed by the terms in
 * the file LICENSE, which is included in the package. You must read this and
 * agree to these terms before using or distributing this software.
 *
 */
#include "oscompat.h"
#include "osservices.h"
#include "osresour_ex.h"
#include "oslinux.h"

#include <linux/pci.h>

static struct pci_device_id cnxthwpci_tbl[] =
{
        {0x1025, 0x5453, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ali")},
        {0x1025, 0x5457, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ali")}, /* plus */
        {0x10B9, 0x5453, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ali")},
        {0x10B9, 0x5457, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ali")}, /* plus */
        {0,}                    /* terminate list */
};

#define CNXTHWPCI_TYPE HW_TYPE_ALI
#define CNXTHWPCI_NAME "hsfmc97ali"

#ifndef CNXTHWPCI_NO_ALI_WORKAROUNDS
#define CNXTHWPCI_ENABLE_ALI_WORKAROUNDS
#endif

#include "cnxthwpci_common.c"

