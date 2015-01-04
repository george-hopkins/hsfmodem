/*
 * Copyright (c) 2007 Linuxant inc.
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
        {0x14F1, 0x2F80, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic3")}, /* Rainier */
        {0x14F1, 0x2F81, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic3")}, /* Rainier B0 OEM */
        {0x14F1, 0x2F82, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic3")}, /* Rainier A0 OEM */
        {0x14F1, 0x2F83, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic3")}, /* Rainier Voice */

        {0,}                    /* terminate list */
};

#define CNXTHWPCI_TYPE HW_TYPE_BASIC3
#define CNXTHWPCI_NAME "hsfpcibasic3"

#include "cnxthwpci_common.c"

