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
        {0x1002, 0x434D, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ati")},
        {0x1002, 0x4378, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("mc97ati")},
        {0,}                    /* terminate list */
};

#define CNXTHWPCI_TYPE HW_TYPE_ATI
#define CNXTHWPCI_NAME "hsfmc97ati"

#include "cnxthwpci_common.c"

