/*
 * Copyright (c) 2004 Linuxant inc.
 * Copyright (c) 2001-2003 Conexant Systems, Inc.
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

#include "osusb.h"

/* table of devices that work with this driver */
static struct usb_device_id cnxthwusb_tbl [] = {
	{ USB_DEVICE(0x572, 0x1300) }, // USB SoftV92 Data Fax Modem with SmartCP
	{ USB_DEVICE(0x572, 0x1301) }, // USB SoftV92 Voice Modem with SmartCP (XXX 3VoltIA ?)
	{ USB_DEVICE(0x572, 0x1302) }, // USB SoftV92 Spkerphone Modem with SmartSP
	{ USB_DEVICE(0x572, 0x1303) }, // USB SoftV92 Data Fax Modem
	{ USB_DEVICE(0x8E3, 0x0111) }, // Olitec Speed'Com USB V92 Ready
	{ USB_DEVICE(0x803, 0x1300) }, // Zoom Telephonics USB V.92
	{ USB_DEVICE(0x803, 0x1301) }, // Hayes USB V92 model 15355
	{ USB_DEVICE(0x145F, 0x0106) }, // Trust MD-1250
	{ USB_DEVICE(0x148D, 0x1671) }, // Creative Modem Blaster V.92 USB DE5671-1
	{ USB_DEVICE(0x148D, 0x1672) }, // Creative Modem Blaster V.92 USB DE5673-1
	{ }
};

#define CNXTHWUSB_TYPE HW_TYPE_CADMUS2
#define CNXTHWUSB_NAME "hsfusbcd2"

#include "cnxthwusb_common.c"

