This package contains Linux drivers for the Conexant (formerly Rockwell)
HSF Softmodem family. The latest version and related information
are available at http://www.linuxant.com on the web.

The drivers should work on most current Linux distributions, with
the 2.4 or 2.6 kernels. A Pentium processor with the MMX enhancements
is required. Some systems may require kernel recompilation with
special ACPI patches, especially with 2.4 kernels. Using a 2.6 kernel
is recommended.

The following modem devices are recognized by the driver:

HSF/HSFi (Standard and SmartDAA)

	PCI ID {127A,14F1}:{2013,2014,2015,2016}
	PCI ID {127A,14F1}:{1023,1024,1026}
	PCI ID {127A,14F1}:4311 (RIPTIDE)
	PCI ID {127A,14F1}:{1025,1085,2005} (if it doesn't work, try HCF driver)
	PCI ID {127A,14F1}:{2003,2004,2006}
	PCI ID 127A:2114

	PCI ID 14F1:{2043,2044,2045,2046}
	PCI ID 14F1:{2063,2064,2065,2066}
	PCI ID 14F1:2093
	PCI ID 14F1:{201A,201B}
	PCI ID 14F1:{204A,204B}
	PCI ID 14F1:{2143,2144,2145,2146}
	PCI ID 14F1:{2163,2164,2165,2166}
	PCI ID 14F1:{2343,2344,2345,2346}
	PCI ID 14F1:{2363,2364,2365,2366}
	PCI ID 14F1:{2443,2444,2445,2446}
	PCI ID 14F1:{2463,2464,2465,2466}

	PCI ID 14F1:{2F00,2F01,2F02,2F03,2F04}
	PCI ID 14F1:{2F10,2F11,2F12,2F13,2F14}
	PCI ID 14F1:{2702,2703,2704,2705}
	PCI ID 14F1:{2F20,2F30}
	PCI ID 14F1:{2F40,2F50}
	PCI ID 14F1:{2F80,2F81,2F82,2F83}

	PCI ID 158B:0001 (Allied Data Technologies)
	PCI ID 158B:0005 (Allied Data Technologies)

	PCI ID 16EC:2F00 (U.S. Robotics USR5660A (265660A) 56K PCI Faxmodem)

	USB ID 0572:1300
	USB ID 0572:1301
	USB ID 0572:1302
	USB ID 0572:1303
	USB ID 08E3:0111 (Olitec Speed'Com USB V92 Ready)
	USB ID 0803:1300 (Zoom Telephonics USB V.92)
	USB ID 0803:1301 (Hayes USB V92 model 15355)
	USB ID 145F:0106 (Trust MD-1250)
	USB ID 148D:1671 (Creative Modem Blaster V.92 USB DE5671-1)
	USB ID 148D:1672 (Creative Modem Blaster V.92 USB DE5673-1)

HDA (High Definition Audio) Modems

	Only under 2.6.16 or newer kernels

	HDA VENDOR ID 14F12BFA
	HDA VENDOR ID 14F12C06

	We recommend the installation of alsa-driver-linuxant available
	at http://www.linuxant.com/alsa-driver before installing this
	driver if you have a HDA modem.

INTEL AC-Link Controller (ICH)

	PCI ID 8086:7186
	PCI ID 8086:7196
	PCI ID 8086:2416
	PCI ID 8086:2446
	PCI ID 8086:2486
	PCI ID 8086:24C6
	PCI ID 8086:24D6
	PCI ID 8086:266D
	PCI ID 8086:2426

	PCI ID 10DE:00D9 (NVIDIA nForce3)
	PCI ID 10DE:01C1 (NVIDIA)

VIA AC-Link Controller

	PCI ID 1106:3068

ALI AC-Link Controller

	PCI ID {1025,10B9}:5453
	PCI ID {1025,10B9}:5457

	The internal modem of HP omnibook xe4500 series machines should work
	with this driver.

ATI AC-Link Controller

	PCI ID 1002:434D
	PCI ID 1002:4378

SIS AC-Link Controller

	PCI ID 1039:7013

MC97 modems from other manufacturers may appear under the AC-Link Controller
PCI IDs above, but this driver only supports Conexant devices.

Many vendors (such as Dell, HP, IBM, U.S. Robotics/3Com, etc..) have
shipped modems based on Conexant modem chipsets. If you have a modem based
on the HSF chipset but with different PCI vendor or device IDs, please
contact modem.support@linuxant.com. If supported, your device could be added
to the list and automatically recognized in future versions of this driver.

See the INSTALL file for installation instructions.

The modem AT command set is described in the file 100498D_RM_HxF_Released.pdf.

Bug reports are welcome. (see section "REPORTING PROBLEMS" in INSTALL file)

Most files in this package are released under terms described in
the LICENSE file. Some distinct components, located in the
modules/GPL directory however are covered by the GNU General Public License.
See the files LICENSE and modules/GPL/COPYING for details.

