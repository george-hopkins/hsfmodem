/*
 * Copyright (c) 2003-2007 Linuxant inc.
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
        {0x127A, 0x1023, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x1023, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x1024, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x1024, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

		/* 1025 could also be HCF */
        {0x127A, 0x1025, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x1025, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x1026, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x1026, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

		/* 1085 could also be HCF */
        {0x127A, 0x1085, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x1085, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x2003, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2003, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x2004, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2004, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

		/* 2005 could also be HCF */
        {0x127A, 0x2005, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2005, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x2006, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2006, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x2013, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x127A, 0x2014, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x127A, 0x2015, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x127A, 0x2016, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x127A, 0x2114, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2013, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2014, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2015, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2016, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2043, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2044, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2045, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2046, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},

        {0x14F1, 0x2063, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2064, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2065, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2066, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2093, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* Legend? */

        {0x14F1, 0x2143, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2144, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2145, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2146, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2163, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2164, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2165, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2166, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2343, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2344, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2345, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2346, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2363, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2364, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2365, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2366, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2443, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2444, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2445, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2446, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},

        {0x14F1, 0x2463, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2464, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2465, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2466, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x2F00, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")},
        {0x14F1, 0x2F01, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")},
        {0x14F1, 0x2F02, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")},
        {0x14F1, 0x2F03, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")},
        {0x14F1, 0x2F04, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")},

        {0x14F1, 0x2F10, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2F11, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2F12, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2F13, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x2F14, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x14F1, 0x4311, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* riptide */
        {0x127A, 0x4311, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* riptide */

	/* the following entries need verification & more testing: */
        {0x14F1, 0x2702, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2703, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2704, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2705, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")},
        {0x14F1, 0x2F20, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2smart")}, /* Riesling */
        {0x14F1, 0x2F30, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")}, /* Wellington */
        {0x14F1, 0x2F40, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2bry")}, /* Bryce */
        {0x14F1, 0x2F50, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2hsfi")}, /* Lassen */
        {0x14F1, 0x201A, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x201B, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x204A, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},
        {0x14F1, 0x204B, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")},

        {0x158B, 0x0001, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* Allied Data Technologies */
        {0x158B, 0x0005, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* Allied Data Technologies */

        {0x16EC, 0x2F00, 0x16EC,     0x010B,     0, 0, CNXTHWCFG("pcibasic2hsfi")}, /* U.S. Robotics V.92 Fax Host Int (USR3090) */
        {0x16EC, 0x2F00, PCI_ANY_ID, PCI_ANY_ID, 0, 0, CNXTHWCFG("pcibasic2")}, /* U.S. Robotics USR5660A (265660A) 56K PCI Faxmodem */

        {0,}                    /* terminate list */
};

#define CNXTHWPCI_TYPE HW_TYPE_BASIC2
#define CNXTHWPCI_NAME "hsfpcibasic2"

#include "cnxthwpci_common.c"

