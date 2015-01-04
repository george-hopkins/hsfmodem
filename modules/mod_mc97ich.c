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
        {0x8086, 0x2416, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},
        {0x8086, 0x2446, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},
        {0x8086, 0x2486, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},
        {0x8086, 0x24C6, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},
        {0x8086, 0x24D6, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},
        {0x8086, 0x266D, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")},

	/* the following entries need verification & more testing: */
        {0x8086, 0x2426, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")}, /* ICH */
        {0x8086, 0x7186, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")}, /* ICH */
        {0x8086, 0x7196, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")}, /* ICH */
        {0x10DE, 0x00D9, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")}, /* Nvidia */
        {0x10DE, 0x01C1, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ich")}, /* Nvidia */
        {0,}                    /* terminate list */
};

#define CNXTHWPCI_TYPE HW_TYPE_ICH
#define CNXTHWPCI_NAME "hsfich"

#include "cnxthwpci_common.c"

