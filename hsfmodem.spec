#
# Copyright (c) 2003-2010 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
#

%if "%{_target_distro}" == "custom"
%define _target_kernel %(eval uname -r)
%endif

# set _target_kernel to generic if it wasn't defined on the command-line
%if %{expand:%{?_target_kernel:0}%{!?_target_kernel:1}}
%define _target_kernel generic
%endif

%if %{expand:%{?_build_doc:0}%{!?_build_doc:1}}
%define _build_doc 0
%endif

%if %{expand:%{?_requires:0}%{!?_requires:1}}
%define _requires none
%endif

%if "%{_target_kernel}" == "generic"
%define ver      7.80.02.06x86_64full
%define rel      1
%define automrecomp 1
%else
%define ver      7.80.02.06x86_64full_k%(eval echo %{_target_kernel} | sed 's/-/_/g')
%define rel      1%{_target_distro}
%define automrecomp 0
%endif

%define cnxtdriver  hsf
%define cnxtdrvdsc  Conexant HSF softmodem driver
%define cnxttarget  hsf

%define scr_support 0
%define dmp_support 0
%define blam_support 0

# Note that this is NOT a relocatable package

%define cnxtetcdir /etc/hsfmodem
%define cnxtlibdir /usr/lib/hsfmodem

# Even though newer versions of RPM provide definitions for these,
# we must still accomodate the older ones
%define prefix   /usr
%define etcdir   /etc
%define libdir   %{prefix}/lib
%define sbindir  %{prefix}/sbin
%define bindir   %{prefix}/bin

Summary:   %{cnxtdrvdsc}
Name:      %{cnxttarget}modem
ExclusiveOS: Linux
%if "%{_target_kernel}" == "generic"
%if "%{cnxtdriver}" == "hsf"
ExclusiveArch: athlon i686 i586 i386 x86_64
%endif
%if "%{cnxtdriver}" == "hcf"
%if "%{_target_cpu}" == "ppc"
ExclusiveArch: ppc
%else
ExclusiveArch: athlon i686 i586 i486 i386
%endif
%endif
%else
%if "%{_target_distro}" != "custom"
ExclusiveArch: %{_target_cpu}
%endif
%endif
Version:   %ver
Release:   %rel
Vendor:	   Linuxant
License: Copyright (c) 2003-2010 Linuxant inc. All rights reserved.
Group:     System Environment/Kernel
Source:    %{cnxttarget}modem-7.80.02.06x86_64full.tar.gz
%if %{_build_doc}
Source2:   100498D_RM_HxF_Released.pdf
%endif
URL:       http://www.linuxant.com/drivers/%{cnxtdriver}
BuildRoot: %{_tmppath}/%{cnxttarget}modem-%{PACKAGE_VERSION}-root
Packager:  Linuxant
Requires:  pciutils
%if %{automrecomp}
#Requires:  kernel-source
Requires:  gcc
%endif
%if "%{cnxtdriver}" == "hsf"
Requires:  perl
%endif
%if "%{cnxtdriver}" == "hsf" || "%{cnxtdriver}" == "hcf"
Conflicts: %{cnxtdriver}linmodem
%endif
%if "%{_requires}" != "none"
Requires: %{_requires}
%endif
Autoreq:   0

%if %{_build_doc}
%package doc
Group:     Documentation
Summary:   Documentation for %{cnxtdrvdsc}
BuildArch: noarch
ExclusiveArch: noarch
%endif

%description
%{cnxtdrvdsc}

Copyright (c) 2003-2010 Linuxant inc.
Copyright (c) 2001-2010 Conexant Systems, Inc.

1.   Permitted use. Redistribution and use in source and binary forms,
without modification, are only permitted under the terms set forth herein.

2.   Disclaimer of Warranties. LINUXANT, ITS SUPPLIERS, AND OTHER CONTRIBUTORS
MAKE NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOFTWARE FOR ANY PURPOSE.
IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTIES OF ANY KIND.
LINUXANT AND OTHER CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD
TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE, GOOD TITLE AND AGAINST INFRINGEMENT.

This software has not been formally tested, and there is no guarantee that
it is free of errors including, but not limited to, bugs, defects,
interrupted operation, or unexpected results. Any use of this software is
at user's own risk.

3.   No Liability.

(a) Linuxant, its suppliers, or contributors shall not be responsible for
any loss or damage to users, customers, or any third parties for any reason
whatsoever, and LINUXANT, ITS SUPPLIERS OR CONTRIBUTORS SHALL NOT BE LIABLE
FOR ANY ACTUAL, DIRECT, INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL, OR
CONSEQUENTIAL (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED, WHETHER IN CONTRACT, STRICT OR OTHER LEGAL THEORY OF
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

(b) User agrees to hold Linuxant, its suppliers, and contributors harmless
from any liability, loss, cost, damage or expense, including attorney's fees,
as a result of any claims which may be made by any person, including
but not limited to User, its agents and employees, its customers, or
any third parties that arise out of or result from the manufacture,
delivery, actual or alleged ownership, performance, use, operation
or possession of the software furnished hereunder, whether such claims
are based on negligence, breach of contract, absolute liability or any
other legal theory.

4.   Notices. User hereby agrees not to remove, alter or destroy any
copyright, trademark, credits, other proprietary notices or confidential
legends placed upon, contained within or associated with the Software,
and shall include all such unaltered copyright, trademark, credits,
other proprietary notices or confidential legends on or in every copy of
the Software.

5.   Reverse-engineering. User hereby agrees not to reverse engineer,
decompile, or disassemble the portions of this software provided solely
in object form, nor attempt in any manner to obtain their source-code.

6.   Redistribution. Redistribution of this software is only permitted
for exact copies (without modification) of versions explicitly marked
and officially released by Linuxant with the word "free" in their name.
Redistribution or disclosure of other versions, derivatives or license key
information is expressly prohibited without explicit written approval signed
by an authorized Linuxant officer.

7.   Performance. V.92 modems are designed to be capable of receiving data at
up to 56Kbps with compatible phone line and server equipment, and transmitting
data at up to 31.2Kbps. V.90 modems are designed to be capable of receiving
data at up to 56 Kbps from a compatible service provider and transmitting data
at up to about 28.8 Kbps. Public networks currently limit download speeds to
about 53Kbps. The free version of the drivers is limited to 14.4Kbps.
Actual speeds vary and are often less than the maximum possible.


%if %{_build_doc}
%description doc
This package contains the documentation for the %{cnxtdrvdsc}.
%endif

%prep
%setup -q -n %{cnxttarget}modem-7.80.02.06x86_64full

%build

%if %{_build_doc}
if [ -f %{SOURCE2} ]; then cp %{SOURCE2} .; else true; fi
%else
make --quiet --no-print-directory all
%endif

%if "%{_target_kernel}" == "generic"

%else

MODS_DIR=binaries/linux-%{_target_kernel}
UNAMER=`uname -r`

# Figure out if we should add -SMP at the end of CNXT_MODS_DIR. We should only add it if the
# kernel was compiled with SMP and the word SMP doesn't appear in the kernel version. This
# is expected by dldrconfig.
case "%{_target_kernel}" in
*[Ss][Mm][Pp]*)
	SMPSUFFIX=""
	;;
*)
	SMPSUFFIX="-SMP"
	;;
esac

case "%{_target_distro}" in
rh | fdr | fs)
	CONFIGFILE="%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel}.%{_target_cpu}/.config"
	;;
custom)
	CONFIGFILE="/lib/modules/${UNAMER}/build/.config"
	;;
*)
	CONFIGFILE="%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel}/.config"
	;;
esac

if [ -e "${CONFIGFILE}" ] && grep -q '^CONFIG_SMP=y$' "${CONFIGFILE}"; then
	MODS_DIR="${MODS_DIR}${SMPSUFFIX}"
fi

%if "%{_target_distro}" == "rh"
(
	if cd modules; then
		case "%{_target_kernel}" in
		*.[Ee][Ll]*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel}.%{_target_cpu} DISTRO_CFLAGS="-D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="-D__BOOT_KERNEL_H_ -D__BOOT_KERNEL_ENTERPRISE=0 -D__BOOT_KERNEL_SMP=0 -D__BOOT_KERNEL_UP=1 -D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		esac
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "fdr"
(
	if cd modules; then
		case "%{_target_kernel}" in
		2.4*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="-D__BOOT_KERNEL_H_ -D__BOOT_KERNEL_ENTERPRISE=0 -D__BOOT_KERNEL_SMP=0 -D__BOOT_KERNEL_UP=1 -D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel}.%{_target_cpu} DISTRO_CFLAGS="" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		esac
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "fs"
(
	if cd modules; then
		case "%{_target_kernel}" in
		2.4*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="-D__BOOT_KERNEL_H_ -D__BOOT_KERNEL_ENTERPRISE=0 -D__BOOT_KERNEL_SMP=0 -D__BOOT_KERNEL_UP=1 -D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		*)
			make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel}.%{_target_cpu} DISTRO_CFLAGS="" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
			;;
		esac
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "mdk"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="-D__BOOT_KERNEL_H_ -D__BOOT_KERNEL_ENTERPRISE=0 -D__BOOT_KERNEL_SMP=0 -D__BOOT_KERNEL_UP=1 -D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "mdv"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "suse"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "jds"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "turbo"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="-D__BOOT_KERNEL_H_ -D__BOOT_KERNEL_SMP=0 -D__BOOT_KERNEL_SMP64G=0 -D__BOOT_KERNEL_UP=1 -D__BOOT_KERNEL_BOOT=0 -D__MODULE_KERNEL_%{_target_cpu}=1" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "redflag"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=%{_distro_kernels}/%{_target_distro}/linux-%{_target_kernel} DISTRO_CFLAGS="" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%if "%{_target_distro}" == "custom"
(
	if cd modules; then
		make --quiet --no-print-directory CNXT_KERNELSRC=/lib/modules/${UNAMER}/build DISTRO_CFLAGS="" CNXT_MODS_DIR="${MODS_DIR}" clean all modules_install || exit $?
	fi
) || exit $?
%endif

%endif

%install
rm -rf $RPM_BUILD_ROOT

make --quiet --no-print-directory all
make --quiet --no-print-directory ROOT=$RPM_BUILD_ROOT install

if [ -d $RPM_BUILD_ROOT%{cnxtetcdir}/nvm ]; then
( cd $RPM_BUILD_ROOT%{cnxtetcdir}/nvm && cd .. && tar czf nvm.tar.gz nvm && rm -rf nvm/*)
fi

echo "RPM" > "$RPM_BUILD_ROOT/%{cnxtetcdir}/package"

%if "%{_target_kernel}" != "generic"
find "$RPM_BUILD_ROOT/%{cnxtlibdir}" \( -name '*.[chO]' -o -name 'Makefile' -o -name '*.mak' -o -name '*.sh' \) -exec rm -f {} \;
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%files

%defattr(0555, root, root, 755)

%{sbindir}/%{cnxttarget}config
%{sbindir}/%{cnxttarget}stop
%{sbindir}/%{cnxttarget}modconflicts
%{cnxtlibdir}/rc%{cnxttarget}
%if "%{cnxtdriver}" == "hsf" || "%{cnxtdriver}" == "dgc"
%{sbindir}/%{cnxttarget}dcpd
%endif

%if %{scr_support}
%{sbindir}/%{cnxttarget}scr
%{bindir}/qtmodemon
%endif
%if %{dmp_support}
%{sbindir}/%{cnxttarget}dmp
%endif
%if %{blam_support}
%{sbindir}/%{cnxttarget}diag
%endif

%defattr(0444, root, root, 755)
%dir %{cnxtetcdir}
%dir %{cnxtetcdir}/log
%if "%{cnxtdriver}" != "dgc"
%dir %{cnxtetcdir}/nvm
%{cnxtetcdir}/nvm.tar.gz
%endif
%{cnxtetcdir}/package
%dir %{cnxtlibdir}
%doc %{cnxtlibdir}/LICENSE
%dir %{cnxtlibdir}/modules
%dir %{cnxtlibdir}/modules/GPL
%doc %{cnxtlibdir}/modules/GPL/COPYING
%{cnxtlibdir}/modules/binaries

%if %{automrecomp}
%config %{cnxtlibdir}/config.mak
%{cnxtlibdir}/modules/imported
%attr(755, root, root) %{cnxtlibdir}/modules/*.sh
%{cnxtlibdir}/modules/Makefile
%{cnxtlibdir}/modules/*.c
%if "%{cnxtdriver}" != "dgc"
%{cnxtlibdir}/modules/GPL/*.c
%endif
%{cnxtlibdir}/modules/GPL/*.h
%if "%{cnxtdriver}" == "hsf"
%dir %{cnxtlibdir}/modules/GPL/hda
%{cnxtlibdir}/modules/GPL/hda/*.c
%{cnxtlibdir}/modules/GPL/hda/*.h
%{cnxtlibdir}/modules/GPL/hda/Makefile
%endif
%dir %{cnxtlibdir}/modules/include
%{cnxtlibdir}/modules/include/*.h
%endif

%doc BUGS CHANGES INSTALL LICENSE README
%if "%{cnxtdriver}" != "dgc"
%doc FAQ CREDITS
%endif

%pre
if [ "$1" = 1 ]; then
	if [ -e "%{cnxtetcdir}" ]; then
		echo "Removing old %{cnxtetcdir}"
		rm -rf "%{cnxtetcdir}"
	fi
	if [ -e "%{cnxtlibdir}" ]; then
		echo "Removing old %{cnxtlibdir}"
		rm -rf "%{cnxtlibdir}"
	fi
fi
exit 0

%post

if [ -f %{cnxtetcdir}/nvm.tar.gz ]; then
	( cd %{cnxtetcdir} && tar xzf nvm.tar.gz )
fi

#%if ! %{automrecomp}
CNXT_AUTOCONFIG=true
#%endif
if [ -n "${CNXT_AUTOCONFIG}" -a -z "${CNXT_NOAUTOCONFIG}" ]; then
	%{sbindir}/%{cnxttarget}config --auto
	ret=$?
	if [ "${ret}" -eq 123 ]; then
		ret=0
	fi
	if [ "${ret}" -eq 124 -a -n "${CNXT_NOWRONGKERNELFAIL}" ]; then
		ret=0
	fi
	exit ${ret}
else
	echo "To complete the installation and configuration of your modem,"
	echo "please run \"%{cnxttarget}config\" (or \"%{sbindir}/%{cnxttarget}config\")"
	exit 0
fi

%preun
%{sbindir}/%{cnxttarget}stop
if [ "$1" = 0 ]; then

	if [ -z "${CNXT_NOAUTOCONFIG}" ]; then
		%{sbindir}/%{cnxttarget}config --remove
	fi

	if [ -f %{cnxtetcdir}/nvm.tar.gz ]; then
		( cd %{cnxtetcdir} && rm -rf `tar tzf %{cnxtetcdir}/nvm.tar.gz | egrep '^nvm/[^/]+/?$'` )
	fi
else
	exit 0
fi

%if %{_build_doc}
%files doc
%doc *.pdf
%endif

# This must be last since the file CHANGES is automatically appended
%changelog

* Sun May 09 2010 -
	- Released hsfmodem-7.80.02.06.

* Sun May 09 2010 -
	- Improved compatibility with newer kernels and distributions.

* Wed Oct 21 2009 -
	- Released hsfmodem-7.80.02.05.

* Wed Oct 21 2009 -
	- Improved compatibility with newer kernels and distributions.

* Thu Apr 23 2009 -
	- Released hsfmodem-7.80.02.04.

* Thu Apr 23 2009 -
	- Improved compatibility with newer kernels and distributions.

* Fri Feb 20 2009 -
	- Released hsfmodem-7.80.02.03.

* Fri Feb 20 2009 -
	- Added buildlogs to dumpdiag.
	- Added modular HDA support and distinct snd-hda-codec-hsfmodem module.
	- Added message in hsfstop to indicate successful operation.
	- Improved uninstallation.

* Mon Jan 26 2009 -
	- Released hsfmodem-7.80.02.02.

* Mon Jan 26 2009 -
	- Improved compatibility with newer kernels and distributions.
	- Added support for alsa-driver 1.0.19.
	- Fixed spurious timer issue affecting HDA, USB and Yukon modems.

* Thu Dec 18 2008 -
	- Released hsfmodem-7.80.02.01.

* Mon Dec 15 2008 -
	- Improved compatibility with newer kernels and distributions.
	- Merged latest Conexant modem code. (7.80.02)

* Wed Oct 01 2008 -
	- Released hsfmodem-7.68.00.14.

* Wed Oct 01 2008 -
	- Improved compatibility with newer kernels and distributions.
	- Added rpmprecomp/debprecomp targets to easily generate pre-compiled
	RPM/DEB pre-compiled packages for the currently running kernel.
	- Added proper INF configuration for Bryce (14f1:2f40) modems.

* Tue Sep 09 2008 -
	- Released hsfmodem-7.68.00.13.

* Mon Sep 08 2008 -
	- Improved compatibility with newer kernels and distributions.
	- Fixed hsfstop script.

* Fri Jul 18 2008 -
	- Released hsfmodem-7.68.00.12.

* Wed Jul 16 2008 -
	- Improved compatibility with newer kernels and distributions.
	- Added support for newer Rainier modems (OEM).

* Fri Jun 20 2008 -
	- Released hsfmodem-7.68.00.11.

* Fri Jun 20 2008 -
	- Improved compatibility with newer kernels and distributions.

* Thu May 08 2008 -
	- Released hsfmodem-7.68.00.10.

* Thu May 08 2008 -
	- Improved compatibility with newer kernels and distributions.
	- Silenced "returning OSEVENT_WAIT_TIMEOUT" messages.

* Mon Mar 24 2008 -
	- Released hsfmodem-7.68.00.09.

* Mon Mar 24 2008 -
	- Improved compatibility with newer kernels and distributions.

* Wed Feb 27 2008 -
	- Released hsfmodem-7.68.00.08.

* Wed Feb 27 2008 -
	- Added workaround for HDA SMP issue.

* Thu Feb 14 2008 -
	- Released hsfmodem-7.68.00.07.

* Thu Feb 14 2008 -
	- Improved compatibility with newer kernels and distributions.

* Fri Dec 07 2007 -
	- Released hsfmodem-7.68.00.06.

* Fri Nov 16 2007 -
	- Improved HDA support.
	- Improved compatibility with newer kernels and distributions.

* Thu Oct 25 2007 -
	- Released hsfmodem-7.68.00.05.

* Thu Oct 25 2007 -
	- Improved HDA support.
	- Improved compatibility with newer kernels and distributions.

* Fri Oct 19 2007 -
	- Released hsfmodem-7.68.00.04.

* Fri Oct 19 2007 -
	- Improved compatibility with newer kernels and distributions.

* Mon Oct 15 2007 -
	- Released hsfmodem-7.68.00.03.

* Mon Oct 15 2007 -
	- Improved compatibility with newer kernels and distributions.

* Thu Oct 11 2007 -
	- Released hsfmodem-7.68.00.02.

* Thu Oct 11 2007 -
	- Added PCI IDs for additional Rainier modems (OEM).
	- Improved compatibility with newer kernels and distributions.

* Fri Oct 05 2007 -
	- Released hsfmodem-7.68.00.01.

* Fri Oct 05 2007 -
	- Improved compatibility with newer kernels and distributions.

* Tue Oct 02 2007 -
	- Released hsfmodem-7.68.00.00.

* Tue Oct 02 2007 -
	- Merged latest Conexant modem code.

* Tue Oct 02 2007 -
	- Released hsfmodem-7.60.00.15.

* Tue Sep 25 2007 -
	- Added USB IDs for Creative Modem Blaster V.92 USB DE5671-1 and DE5673-1.
	- Improved compatibility with newer kernels and distributions.

* Tue Sep 25 2007 -
	- Released hsfmodem-7.60.00.14.

* Tue Sep 25 2007 -
	- Improved HDA support.

* Mon Sep 24 2007 -
	- Released hsfmodem-7.60.00.13.

* Mon Sep 24 2007 -
	- Improved HDA support.
	- Improved compatibility with newer kernels and distributions.

* Thu Sep 13 2007 -
	- Released hsfmodem-7.60.00.12.

* Thu Sep 13 2007 -
	- Configure HDA PortB as ExtMic instead of line input on Venice devices.

* Wed Sep 12 2007 -
	- Released hsfmodem-7.60.00.11.

* Wed Sep 12 2007 -
	- Improved compatibility with newer kernels and distributions.

* Wed Sep 05 2007 -
	- Released hsfmodem-7.60.00.10.

* Wed Sep 05 2007 -
	- Improved compatibility with newer kernels and distributions.
	- Improved HDA support.
	- Added support for Conexant CX20561 (Hermosa) chip.

* Tue May 08 2007 -
	- Released hsfmodem-7.60.00.09.

* Tue May 08 2007 -
	- Improved compatibility with newer kernels and distributions.
	- Reverted CXT5045_LAPTOP_EAPDHIGHONLY workaround for HP laptop.

* Thu Apr 19 2007 -
	- Released hsfmodem-7.60.00.08.

* Thu Apr 19 2007 -
	- Temporarily reverted Sigmatel 9205 support to generic HDA codec.

* Wed Apr 18 2007 -
	- Released hsfmodem-7.60.00.07.

* Wed Apr 18 2007 -
	- Added 00- prefix to udev rules files to prevent short-circuiting
	by "ttyS*" rule on some distributions.

* Tue Apr 17 2007 -
	- Released hsfmodem-7.60.00.06.

* Tue Apr 17 2007 -
	- Added workaround for suspend2 issue on Red Flag Linux.

* Tue Apr 17 2007 -
	- Released hsfmodem-7.60.00.05.

* Tue Apr 17 2007 -
	- Added HDA HP EAPD workaround.

* Sun Apr 15 2007 -
	- Released hsfmodem-7.60.00.04.

* Sun Apr 15 2007 -
	- Merged HDA drivers with latest ALSA hg code.
	- Improved muting for HP/Compaq laptops.
	- Improved compatibility with newer kernels and distributions.
	- Improved HDA power-management.

* Thu Mar 29 2007 -
	- Added workaround for microphone issue on HP/Compaq laptops.
	- Merged HDA drivers with ALSA 1.0.14rc3.

* Thu Mar 22 2007 -
	- Released hsfmodem-7.60.00.03.

* Thu Mar 22 2007 -
	- Fixed floating-point issue on x86_64.

* Thu Mar 01 2007 -
	- Released hsfmodem-7.60.00.02.

* Wed Feb 28 2007 -
	- Reduced stack consumption to accommodate smaller 4k kernel stacks.
	- Improved compatibility with newer kernels and distributions.

* Mon Jan 29 2007 -
	- Released hsfmodem-7.60.00.01.

* Mon Jan 29 2007 -
	- Fixed intermittent stability issue.
	- Improved compatibility with newer kernels and distributions.

* Wed Jan 10 2007 -
	- Released hsfmodem-7.60.00.00.

* Fri Jan 05 2007 -
	- Merged latest Conexant modem code.

* Fri Jan 05 2007 -
	- Released hsfmodem-7.47.00.09.

* Fri Jan 05 2007 -
	- Corrected EAPD merge issue.

* Thu Jan 04 2007 -
	- Released hsfmodem-7.47.00.08.

* Thu Jan 04 2007 -
	- Fixed EAPD / mute issue with Conexant HDA Audio.

* Thu Dec 28 2006 -
	- Released hsfmodem-7.47.00.07.

* Thu Dec 28 2006 -
	- Merged HDA drivers with ALSA 1.0.14rc1.
	- Implemented support for Conexant HDA Audio/Modem Combo.
	- Improved compatibility with newer kernels and distributions.

* Fri Dec 15 2006 -
	- Released hsfmodem-7.47.00.06.

* Fri Dec 15 2006 -
	- Improved compatibility with newer kernels and distributions.
	- Improved software RING detection

* Thu Nov 09 2006 -
	- Released hsfmodem-7.47.00.05.

* Thu Nov 09 2006 -
	- Improved compatibility with newer kernels and distributions.

* Tue Oct 03 2006 -
	- Released hsfmodem-7.47.00.04.

* Tue Oct 03 2006 -
	- Improved compatibility with newer kernels and distributions.

* Wed Aug 30 2006 -
	- Released hsfmodem-7.47.00.03.

* Wed Aug 30 2006 -
	- Merged HDA drivers with ALSA 1.0.12-final.

* Mon Aug 14 2006 -
	- Released hsfmodem-7.47.00.02.

* Mon Aug 14 2006 -
	- Improved compatibility with newer kernels and distributions.
	- Added USB ID 0803:1301 (Hayes USB V92 model 15355)
	- DCP daemon now automatically restarted.

* Tue May 02 2006 -
	- Released hsfmodem-7.47.00.01.

* Tue May 02 2006 -
	- Merged HDA drivers with ALSA 1.0.11-final.
	- Fixed HDA connection issues.
	- Improved compatibility with newer kernels and distributions.

* Mon Apr 10 2006 -
	- Released hsfmodem-7.47.00.00.

* Mon Apr 10 2006 -
	- Fixed lockup on Core Duo processors.
	- Merged HDA drivers with newer kernel code.
	- Merged latest Conexant modem code.
	- Improved HDA power-management.
	- Improved compatibility with newer kernels and distributions.

* Wed Mar 08 2006 -
	- Released hsfmodem-7.43.00.02.

* Wed Mar 08 2006 -
	- Improved compatibility with newer kernels and distributions.
	- Downgraded inoffensive error messages to warnings.
	- Added mechanism to automatically configure region when hardware
	is dynamically detected.

* Tue Feb 07 2006 -
	- Released hsfmodem-7.43.00.01.

* Tue Feb 07 2006 -
	- Fixed flow-control issue upon device open.
	- Fixed HDA compatibility with 2.6.16-rc1 kernels.
	- Added soft ring detection for HDA.
	- Disable unpatched modules when replacing HDA drivers.

* Sat Jan 28 2006 -
	- Added support for Conexant HD Audio (HDA) modems.
	Note: HDA support requires a recent 2.6 kernel.
	- Merged latest Conexant modem code.
	- Improved compatibility with newer kernels and distributions.

* Wed Dec 07 2005 -
	- Released hsfmodem-7.18.00.08.

* Wed Dec 07 2005 -
	- Fixed PAGE_OFFSET issue causing crashes during initialization
	on some systems.
	- Improved compatibility with newer kernels and distributions.
	- Improved patching facility.

* Fri Oct 21 2005 -
	- Released hsfmodem-7.18.00.07.

* Fri Oct 21 2005 -
	- Improved compatibility with newer kernels and distributions.

* Mon Sep 22 2005 -
	- Released hsfmodem-7.18.00.06.

* Mon Sep 19 2005 -
	- Added --patch option to config script for easier installation of patches.
	- Added ATI PCI ID 1002:4378.
	- Added USB ID 0803:1300 (Zoom Telephonics USB V.92)
	- Improved Debian and Gentoo compatibility.
	- Improved compatibility with newer kernels.

* Mon Jun 20 2005 -
	- Released hsfmodem-7.18.00.05.

* Mon Jun 20 2005 -
	- Added support for Fedora Core 4.
	- Added support for gcc 4.0.
	- Added support for pre-built debian binary packages.
	- Added checks to warn about kernel configuration options which are
	known to be problematic on some systems.
	- Added support for Freedows.
	- Added detection of Ubuntu, Knoppix, Mepis and Freedows distributions.
	- Improved kernel header checks.

* Wed Mar 09 2005 -
	- Fixed small bug in NVM uninstall.
	- Added check to ensure install partition supports symbolic links.

* Thu Mar 03 2005 -
	- Released hsfmodem-7.18.00.03.

* Thu Mar 03 2005 -
	- Added support for 2.6.11 kernels.
	- Fixed gcc-3.4 compilation issues.

* Thu Dec 30 2004 -
	- Released hsfmodem-7.18.00.02.

* Thu Dec 30 2004 -
	- Compatibility improvements.
	- Fixed OsSleep() bug.

* Wed Dec 15 2004 -
	- Released hsfmodem-7.18.00.01.

* Wed Dec 15 2004 -
	- Removed -march=k8 from IMPORTED_COMPILATION_FLAGS on x86_64
	as it is not accepted by some versions of gcc.

* Tue Dec 14 2004 -
	- Released hsfmodem-7.18.00.

* Tue Dec 14 2004 -
	- Added support for ATI and SIS MC97 controllers.
	- Merged latest Conexant modem code (v7.18.00), including
	support for soft-ring detection and MT extensions.
	- Updated nvm/inf/hsf.cty (CTYGen.mst).
	- Various compatibility improvements.
	- Added support for x86_64 architecture.
	- Added support for USB HSF devices with SmartDAA.
	- Added --uninstall option to config script.
	- Centralized .in file generation.
	- Modified scripts to use /var/run/ for temp files.
	- Fixed pppd connect / deadlock issue under 2.6.
	- Fixed reading /proc/driver/hsf/*/lastcallstatus with non-zero offset.
	- Changed module install location from /lib/modules/`uname -r`/misc
	to /lib/modules/`uname -r`/extra.

* Mon Nov 15 2004 -
	- Released hsfmodem-6.03.00lnxt04111500.

* Mon Nov 15 2004 -
	- Fixed module building issues.

* Fri Nov 12 2004 -
	- Released hsfmodem-6.03.00lnxt04111200.

* Fri Nov 12 2004 -
	- Added pre-built binary packages for Sun JDS Release 2.
	- Added udev rules for newer distributions.
	- Fixed USB interface issues.
	- Removed inline attribute for osusb.c:ChangeUrbListEntryState().
	- Removed obsolete serial probeall line from modules.conf.
	- Improved compatibility with kernel build environment.
	- Clarified messages about conflicting modules.

* Tue Aug 24 2004 -
	- Released hsfmodem-6.03.00lnxt04082400.

* Mon Aug 23 2004 -
	- Added automatic detection and disabling of conflicting modules.
	- Added rc startup script to automatically build kernel modules
	if necessary (and feasible, only with generic packages) and load them.
	- Added support for /etc/modprobe.d and workaround for udev (modules
	remain loaded after installation).
	- Fixed building without CONFIG_USB.

* Fri Aug 06 2004 -
	- Added USB device 08E3:111 (Olitec Speed'Com USB V92 Ready).

* Fri Jul 30 2004 -
	- Added ICH PCI ID 10DE:00D9 (NVIDIA nForce3).

* Thu Jul 29 2004 -
	- Released hsfmodem-6.03.00lnxt04072900.

* Thu Jul 29 2004 -
	- Fixed block size bug introduced with USB support.
	- Fixed USB build compatibility with older kernels.
	- Fixed build issues with SuSE 2.6.5-7.95 kernel upgrade.
	- Added support for the latest SuSE 9.1 kernel update (2.6.5-7.75).

* Mon Jun 28 2004 -
	- Released hsfmodem-6.03.00lnxt04062800.

* Mon Jun 28 2004 -
	- Added support for HSF USB modems.

* Fri Jun 18 2004 -
	- Released hsfmodem-6.03.00lnxt04061800.

* Fri Jun 18 2004 -
	- Fixed compilation problem with gcc 3.3.3.
	- Added support for the latest SuSE 9.1 kernel update (2.6.5-7.75).

* Wed May 19 2004 -
	- Added Fedora Core 2 support.
	- Removed __devinitdata from pci_device_id table.

* Thu May 13 2004 -
	- Added ICH6 (8086:266D) support.

* Thu May 13 2004 -
	- Released hsfmodem-6.03.00lnxt04051300.

* Thu May 13 2004 -
	- Reduced stack consumption to accommodate smaller 4k kernel stacks.
	(fixes crashes under recent Fedora test kernels)
	- Added check to ensure config script is executed as root.

* Sat May 08 2004 -
	- Released hsfmodem-6.03.00lnxt04050800.

* Sat May 08 2004 -
	- Fixed regParm issue causing crashes on SuSE 9.1.

* Sat May 01 2004 -
	- Released hsfmodem-6.03.00lnxt04050100.

* Fri Apr 30 2004 -
	- Corrected MODULE_LICENSE() in mod_engine.c to ensure tainting.

* Sun Apr 18 2004 -
	- Added KBUILD_EXTMOD support.

* Tue Apr 13 2004 -
	- Released hsfmodem-6.03.00lnxt04041300.

* Tue Apr 13 2004 -
	- Adjusted serial_core init_termios for minicom.

* Sun Mar 28 2004 -
	- Released hsfmodem-6.03.00lnxt04032800.

* Sat Mar 27 2004 -
	- Implemented delayed writes in osnvm and eliminated use of pid file in
	hsfdcpd to accomodate early module loads on SuSE when root filesystem is
	still read-only.

* Fri Mar 26 2004 -
	- Fixed "hsfserial: falsely claims to have parameter calloutmajor" issue.
	- Support pre-built binary packages for 2.6 kernels.
	- Support SuSE's new-style kernel config and SetupKernelSource.sh on
	TurboLinux.

* Sat Mar 13 2004 -
	- Released hsfmodem-6.03.00lnxt04031300.

* Sat Mar 13 2004 -
	- Fixed devfs_mk_cdev() issue under 2.6.

* Wed Feb 11 2004 -
	- Added ICH PCI ID 8086:7196.

* Tue Jan 27 2004 -
	- Added ICH5 (8086:24D6) support.

* Mon Jan 19 2004 -
	- Released hsfmodem-6.03.00lnxt04011900.

* Mon Jan 19 2004 -
	- Fixed crashes which occured under RedHat's experimental 2.6 kernels.
	- modules/osservices.c: fixed recursive uniprocessor CriticalSection issue.

* Sun Dec 28 2003 -
	- Released hsfmodem-6.03.00lnxt03122800.

* Sun Dec 28 2003 -
	- Added support and pre-built binary packages for Fedora Core 1.
	- Added pre-built binary packages for the latest RedHat, Mandrake,
	and Turbolinux kernel updates.
	- Fixed deprecated tail syntax in kernelcompiler.sh script.
	- Fixed cnxthwpci_common.c compile issue under 2.6 with older gcc versions.
	- Added check for CONFIG_SERIAL_CORE to serial_cnxt.c.
	- Added check for disabled kernel.modprobe (set to /bin/true by 2.4
	rc.sysinit script looking for /proc/ksyms instead of /proc/kallsyms).
	- Integrated Conexant modem improvements.

* Mon Nov 24 2003 -
	- Released hsfmodem-6.03.00lnxt03112400.

* Fri Nov 21 2003 -
	- Added Turbolinux support.

* Fri Nov 21 2003 -
	- Released hsfmodem-6.03.00lnxt03112100.

* Fri Nov 21 2003 -
	- Fixed directory creation issue with NVM_Open() on SuSE 9.0.
	- Added pre-built binary packages for SuSE 2.4.21-144 kernel update.

* Tue Nov 11 2003 -
	- Released hsfmodem-6.03.00lnxt03111100.

* Tue Nov 11 2003 -
	- Added pre-built binary packages for the following distributions:
		- Mandrake 9.2
		- SuSE 9.0
	- modules/osservices.c: fixed more 2.6-test issues.
	- scripts/cnxtconfig.in, modules/kernelcompiler.sh: unset LANG.

* Sat Oct 11 2003 -
	- Allow AT+FCLASS=0 in free version for compatibility reasons.
	- Fixed incorrect documentation symlink on debian.
	- modules/osstdio.c:OsFSeek() now returns proper value.
	- scripts/cnxtconfig.in: use depmod -a instead of -A,
	add char-major- module alias for DCP device and
	update_module_dependencies after removing modules.
	- scripts/cnxtstop.in: exit 0 at end.
	- modules/osservices.c: fixed to build under latest 2.6-test7 kernels.

* Thu Sep 18 2003 -
	- Released hsfmodem-6.03.00lnxt03091800.

* Thu Sep 18 2003 -
	- Removed extraneous CID diagnostics messages.
	- modules/mod_pcibasic2.c: use pcibasic2hsfi profile for 14f1:2f0x.
	- nvm/cvtinf.pl,makefile: do not use generic relays for HSFi since they
	interfere with RING detection.

* Tue Sep 16 2003 -
	- Released hsfmodem-6.03.00lnxt03091600.

* Tue Sep 16 2003 -
	- Corrected slight timing issue with key abort just after dial command.
	(Thanks to Angus Auld for reporting it)

* Fri Sep 12 2003 -
	- Released hsfmodem-6.03.00lnxt03091200.

* Fri Sep 12 2003 -
	- modules/kernelcompiler.sh: improved to avoid building modules
	with gcc 2.x for kernels compiled with gcc 3.x. This was the cause
	of some oops, notably on Debian.
	- INSTALL: removed reference to common.mak which no longer exists.
	- scripts/cnxtdcpd.in: various improvements.

* Wed Sep 10 2003 -
	- Released hsfmodem-6.03.00lnxt03091001.

* Wed Sep 10 2003 -
	- dist/modules/osnvm.c: fixed bug in NVM_ReadCountryParm()
	preventing referenced parameters from being read-in correctly.
	This led to wrong RELAYS values causing the modem to go off hook
	as soon as the driver modules were loaded.
	- nvm/cvtinf.pl: use generic HSF RELAYS to fix possible issues with
	older devices that previously worked.
	- cnxtmodem.spec.in: put static nvm parameter files in nvm.tar.gz
	instead of including them directly in RPMs, to avoid hard links
	which are not supported by older RPM versions (fixes the
	"rpmlib(PartialHardLinkSets)" dependency problem).

* Tue Sep 09 2003 -
	- Released hsfmodem-6.03.00lnxt03090900.

* Tue Sep 09 2003 -
	- Fixed issue with non-standard memory configurations.

* Mon Sep 08 2003 -
	- Released hsfmodem-6.03.00lnxt03090800.

* Mon Sep 01 2003 -
	- Fixed various issues with MC97 drivers (especially ICH)
	that might have caused lockups.

* Fri Aug 29 2003 -
	- Added support for DIAGMGR (SDK2K) and BLAM.
	- Incorporated DMP support in osdiag.

* Thu Aug 28 2003 -
	- Added support for DCP (Digital Call Progress) audio monitoring.
	- Updated region parameters.
	- Disabled dialtone detection (ATX3) by default.

* Tue Aug 26 2003 -
	- Added workaround for serial driver falsely claiming to handle
	our devices.
	- Added workaround to fix problems loading hsfmc97ali after
	trident ac97 audio driver due to a bug in the latter.

* Mon Aug 25 2003 -
	- Improved Debian support, added .deb packaging (make debdist).

* Sun Aug 24 2003 -
	- Significant overhaul done during spring/summer 2003, greatly
	improving modem stability, compatibility, and features!
	- Added V.92, Caller ID and FAX (Class 1/1.0) support.
	- Added support for 2.6 kernels (presently -test3 and -test4).
	- Numerous bugs fixed.
	- config script has been greatly improved.
	- NVM/.inf handling completely redesigned. Parameters updated.
	- Multiple device instances now supported, which means that
	more than one modem can be handled by the driver.
	- pci_register_driver() used instead of pci_find_device().
	- Improved SMP support; also preemptible kernels should now work.
	- Renamed HSF driver package to hsfmodem. Default etc and lib
	directories are now /etc/hsfmodem/ and /usr/lib/hsfmodem/ respectively.

* Sun Apr 27 2003 -
	- Released hsflinmodem-5.03.27lnxtbeta03042700.

* Sun Apr 27 2003 -
	- makefile, hsflinmodem.spec.in: changes to build packages for newer
	SuSE versions with i586 arch and athlon kernels.
	- modules/kernelcompiler.sh: fixed SuSE 8.2 issue.
	- scripts/hsfconfig.in: improved dump_diagnostics() and added --noprobe
	option.
	- scripts/hsfconfig.in: use ${AWK} (gawk if present) instead of awk.
	- scripts/hsfconfig.in: fix link problem in create_temp_kernel_tree().
	- FAQ: updated.

* Fri Apr 18 2003 -
	- modules/serial_hsf.c: hsf_get_lastcallstatus(): use length and check
	offset.
	- imported/hsfengine.O: increased buffer size for AT#UG (lastcallstatus).

* Wed Apr 16 2003 -
	- Released hsflinmodem-5.03.27lnxtbeta03041600.

* Wed Apr 16 2003 -
	- FAQ: major update.
	- makefile: added rule to sign rpm packages.
	- inf/linux_intel_smartmc.inf: added 8086:24C6 device (thanks to
	Gianpaolo Cugola who reported successfully using this inf
	file with his Dell Latitude D600)
	- inf/linux_hsfi.inf: added 14F1:2702 device.

* Wed Apr 16 2003 -
	- Added pre-built binary packages for the following
	distributions/kernel versions:
		Mandrake 9.1 linux-2.4.21-0.13mdk
		Mandrake 9.0 linux-2.4.19-32mdk
		RedHat 9 linux-2.4.20-8 and -9
		RedHat 8.0 linux-2.4.18-27.8.0
		SuSE 8.2 2.4.20-4GB
	- modules/kernelcompiler.sh: updated.

* Wed Apr 16 2003 -
	- modules/osspec/ostime.c: compatibility changes for
	RedHat 9 and newer 2.5 kernels.
	- modules/osspec/ostime.c: fixed important race in mdmthrd().

* Wed Apr 16 2003 -
	- Now officially maintained and distributed by Linuxant.

* Wed Dec 26 2002 -
	- Released hsflinmodem-5.03.27mbsibeta02122600.

* Wed Dec 26 2002 -
	- Merged workarounds for various V.42 problems.
	- Merged new improved Conexant modem code (v5.03.27).
	- Added support for configurable buffer switch delays.

* Wed Dec 25 2002 -
	- Released hsflinmodem-5.03.03mbsibeta02122500.

* Wed Dec 25 2002 -
	- Resolved hangs/disconnects after a few retrains.
	- Simplified RTMgr to eliminate ordering issues affecting
	flow control status events.
	- Fixed AT%E0 command to disable retrains/renegs.
	- Output trace error messages.

* Tue Dec 17 2002 -
	- makefile: added Mandrake 9.0 linux-2.4.19-19mdk kernel.
	- makefile: added RedHat 2.4.18-19.7.x and 2.4.18-19.8.0 kernels.

* Mon Dec 09 2002 -
	- inf/linux_hsf.inf: added 127A:2004 (thanks to Paul Lebeyka).

* Thu Nov 14 2002 -
	- modules/osspec/ostime.c: emulate nested critical sections to
	hopefully fix hangs on SMP.

* Sun Nov 03 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02110301.

* Sun Nov 03 2002 -
	- modules/kernelcompiler.sh: use gcc 3 to build modules for
	SuSE's 2.4.19-4GB kernel.

* Sun Nov 03 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02110300.

* Sun Nov 03 2002 -
	- makefile: added RedHat 2.4.18-17.7.x and 2.4.18-17.8.0 kernels.
	- makefile: added SuSE 2.4.19-4GB kernel.
	- modules/kernelcompiler.sh: use gcc 3 to build modules for
	RedHat's 2.4.18-17.8.0 kernel.
	- scripts/hsfconfig.in: put module configuration in /etc/modules.d and
	run update-modules if present (for gentoo).
	- modules/serial_hsf.c: properly test COMCTRL_MONITOR_POUND_UG_SUPPORTED.

* Tue Oct 29 2002 -
	- Limited printing of MC97 vendor IDs.

* Fri Oct 11 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02101001.

* Fri Oct 11 2002 -
	- modules/kernelcompiler.sh: use gcc 3 to build modules for
	RedHat's 2.4.18-14 kernel.

* Thu Oct 10 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02101000.

* Thu Oct 10 2002 -
	- makefile: added RedHat 8.0 linux-2.4.18-14 kernel.

* Mon Sep 30 2002 -
	- inf/: Changed INTCODE for SWEDEN from 009 to 00.
	(Thanks to Anders Helmersson)

* Mon Sep 30 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02093000.

* Mon Sep 30 2002 -
	- imported/hsfengine.O: fixed RTMgr race condition causing
	flow control to hang when a lot of data is uploaded.
	- makefile: added Mandrake 9.0 linux-2.4.19-16mdk kernel and
	removed obsolete SuSE 2.4.4-4GB kernel.

* Sun Sep 29 2002 -
	- makefile: tardist: changed way tar.gz files are generated to
	avoid gzip "decompression OK, trailing garbage ignored" messages.
	- modules/serial_hsf.c: Added /proc/driver/hsf/0/lastcallstatus and
	loglastcallstatus parameter, which when set to a non-zero value will
	make the driver automatically log the last call status (AT#UG) info.
	- INSTALL: document loglastcallstatus feature.

* Mon Sep 23 2002 -
	- modules/osspec/ostime.c: minor adjustments for 2.5 compatibility.
	- modules/serial_core.c: uart_register_driver(): set driver_name
	to drv->owner->name for proper /proc/tty/driver/ naming;
	renamed HSFSERIAL_INCLUDE_CORE to CNXTSERIAL_INCLUDE_CORE throughout.
	- Fixed potential crash upon MC97 driver initialization failure.
	- imported/makeflags.mak: removed problematic COMPILATION_FLAGS.
	- inf/linux_*_smartmc.inf: set additional HwData bit to improve
	handshaking reliability.
	- modules/kernelcompiler.sh: added gcc-2.96 (for Mandrake 9) and
	RPM_BUILD_ROOT support.

* Fri Sep 06 2002 -
	- inf/linux_ali_smartmc.inf, README: added ALI 10B9:5453 MC97
	controller.

* Thu Sep 05 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02090500.

* Thu Sep 05 2002 -
	- Added initial support for ALI MC97 controller (10B9:5457)
	with Conexant AC-Link modems. Thanks to HP for supporting this effort!
	- README, CREDITS: updated.

* Thu Sep 05 2002 -
	- imported/hsfengine.O: fixed another StatMgr bug.

* Fri Aug 30 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02083000.

* Fri Aug 30 2002 -
	- FAQ: updated.
	- modules/osspec/include/oscompat.h: added check for CONFIG_PREEMPT.
	- modules/osspec/ostime.c: fixed more potential race conditions and
	thread termination problems.
	- makefile: added Mandrake 2.4.18-8.1mdk kernel.
	- Stack checker is now disabled by default.

* Sun Aug 25 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02082500.

* Sun Aug 25 2002 -
	- modules/osspec/include/oscompat.h: added attribute used
	to stack_overflow() to prevent compiler from optimizing it out.
	- modules/osspec/ostime.c: mdmthrd startup/shutdown improvements.

* Sat Aug 24 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02082400.

* Sat Aug 24 2002 -
	- makefile: added RedHat 2.4.18-10 kernel.
	- modules/osspec/ostime.c: removed modem_task_fpu_begin/end() to
	resolve floating point problem in kernel thread which caused
	"MemMgr_Alloc:: failed, ReqSize == 0" errors on some machines.

* Fri Aug 23 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02082300.

* Fri Aug 23 2002 -
	- modules/osspec/include/oscompat.h: added kernel stack checker.
	- modules/osspec/ostime.c: fixed potential race condition
	in OSCriticalSectionAcquire().
	- imported/hsfengine.O: fixed StatMgr/MemMgr issue causing
	memory problems after 9-10 connections.

* Tue Aug 20 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02082000.

* Tue Aug 20 2002 -
	- modules/osspec/osmemory.c: emit error message upon vmalloc()
	or kmalloc() failure.
	- imported/hsfengine.O: memory manager should no longer
	allocate/free os memory while holding locks.

* Mon Aug 19 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02081900.

* Mon Aug 19 2002 -
	- Eliminated OSCallOnMyStack() which was causing major crashes
	due to get_current() expecting that a task_struct be present
	at (esp & ~8191). Let's just hope that we will never exhaust
	the kernel (interrupt) stack..
	- Added modules/kernelcompiler.sh, a small script which tries
	to guess which compiler should be used to build kernel modules.
	- modules/common.mak: use kernelcompiler.sh to determine default CC
	- modules/common.mak, INSTALL: added notes about KERNELSRC for
	Debian 3.0 users.
	- inf/linux_hsf.inf: added 14F1:2F10, 14F1:2F11, 14F1:2F13 (USR OEM).

* Tue Jul 23 2002 -
	- imported/hsfengine.O: asm fixes for gcc-3.2.
	- README: added notice that preemptible kernels are not
	supported (for now).

* Sun Jul 21 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02072100.

* Sun Jul 21 2002 -
	- makefile: added RedHat 2.4.9-34 and 2.4.18-5 kernels.
	- Updated BUGS file.

* Sat Jul 20 2002 -
	- imported/hsfengine.O: important workaround for tricky StatMgr issue
	causing sporadic memory corruption!
	- modules/osspec/include/oscompat.h: added OSContextAllowsSleeping(),
	which is false if we are in interrupt context or holding any lock.
	- modules/osspec/ostime.c: implemented dedicated kernel thread,
	replacing use of tq_scheduler and schedule_task().
	- modules/osspec/ostime.c: TimerThreadFunction(): enclose
	pTimeOutInstance->pTimeOutCallBack() in modem_task_fpu_begin/end().
	- modules/osspec/ostime.c: rewrote OSCallOnMyStack().
	- modules/osspec/ostime.c: use cmpxchg() as OsAtomicCompareAndSwap()
	if available.
	- modules/osspec/ostime.c: added OsAtomicIncrement() and
	OsAtomicDecrement().
	- modules/osspec/ostime.c: OsSleep(): ensure OSContextAllowsSleeping().
	- modules/osspec/osmemory.c: OsHeapAllocate(), OsMemDMAAllocate(): use
	GFP_ATOMIC if not OSContextAllowsSleeping().
	- modules/makefile: libosspec.a for proper dependencies.
	- inf/linux_hsfi.inf: use new official (Conexant) generic relays.

* Thu Jul 11 2002 -
	- inf/linux_hsf.inf: added 14F1:2F12 (3COM/USR model 3094-3095).
	- config.mak: use HSFETCDIR for HSFBININF_FILE and HSFFIRMWR_FILE.
	(Thanks to Fabrice LABORIE)

* Tue Jun 25 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02062500.

* Tue Jun 25 2002 -
	- modules/osspec/include/oscompat.h: get rid of non-standard min/max
	macros under 2.4.9.
	- inf/linux_hsfi.inf: backout generic RELAYS for now.
	- Minor frequency calculation tweak in VIA MC97 controller driver.
	- modules/osspec/osdebug.c: OsErrorVPrintf() now uses default printk
	level.

* Tue Jun 18 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02061800.

* Tue Jun 18 2002 -
	- Fixed frequency calculation issue in VIA MC97 controller driver.
	- Added one-time print of MC97 vendor IDs.
	- OSTermPowerManagement(): use pm_unregister() under 2.4,
	pm_unregister_all() only once under 2.2.

* Mon Jun 17 2002 -
	- Released hsflinmodem-5.03.03.L3mbsibeta02061700.

* Sun Jun 16 2002 -
	- Added preliminary power-management support.
	- Improved hardware interrupt initialization.
	- modules/osspec/ostime.c: OsSleep() and MSECS_TO_TICKS() tweaking.

* Tue Jun 11 2002 -
	- makefile, hsflinmodem.spec.in: don't include big pdf file in tar pkgs.

* Mon Jun 10 2002 -
	- makefile, hsflinmodem.spec.in: better error propagation for RPM builds.

* Sun Jun 02 2002 -
	- Released hsflinmodem-5.03.03.L2mbsibeta02060200.

* Sun Jun 02 2002 -
	- Restructured packaging once again to improve consistency with linux
	distribution practices and further reduce space consumption.
	- Merged latest VIA MC97 controller code, fixing RING detection
	on some newer AC-link modems.

* Sat Jun 01 2002 -
	- Released hsflinmodem-5.03.03.L2mbsibeta02060100.

* Sat Jun 01 2002 -
	- modules/serial_hsf.c: hsf_tx_ready() should ignore hsf_evt_txempty
	for now.
	- modules/osspec/osstdio.c: various enhancements to OsFOpen().

* Fri May 31 2002 -
	- Added initial support for VIA MC97 controller (1106:3068)
	with Conexant AC-Link modems.
	- Fixed delay calculation issue affecting Intel MC97 driver.
	- modules/osspec/linuxres.c: added LinuxPci{Read,Write}Config{w,b}
	- modules/osspec/osmemory.c: OsMemDMAAllocate() should safely
	ignore a NULL pMemHandle.
	- Removed extraneous prefixes from T.31 fax command responses.

* Tue May 28 2002 -
	- modules/serial_core.c: replaced MINOR() with minor() for 2.5.x and
	added missing spin_lock_init(&info->lock) to uart_get().
	- modules/osspec/include/oscompat.h: ensure minor() is defined.

* Sun May 26 2002 -
	- Released hsflinmodem-5.03.03.L2mbsibeta02052600.

* Sun May 26 2002 -
	- Restructured distribution-specific packaging to include
	only needed kernel module binaries without unnecessary source.
	This reduces package sizes and download times. Only the generic
	packages now contain source code providing hsfconfig the ability
	to re-link the kernel modules on-the-fly.
	hsfconfig --auto is once again executed automatically during
	installation of distribution-specific RPMs.
	- Updated README, INSTALL and BUGS documentation files.

* Thu May 23 2002 -
	- Released hsflinmodem-5.03.03.L2mbsibeta02052300.

* Thu May 23 2002 -
	- Merged new improved Conexant modem code (v5.03.03.L2).
	- scripts/hsfconfig.in: remove old /dev/modem after configuring devfs.

* Tue May 21 2002 -
	- Released hsflinmodem-5.03.03.01mbsibeta02052102.

* Tue May 21 2002 -
	- Fixed EPRINTF() macro so that notice about unsupported AC97 codec
	is printed.

* Tue May 21 2002 -
	- Released hsflinmodem-5.03.03.01mbsibeta02052101.

* Tue May 21 2002 -
	- modules/makefile: Added binary module support for SuSE 8.0 (2.4.18-4GB)

* Tue May 21 2002 -
	- Released hsflinmodem-5.03.03.01mbsibeta02052100.

* Mon May 20 2002 -
	- scripts/hsfconfig.in: Improved devfs configuration to hopefully solve
	automatic module loading/device node visibility issues.
	- modules/makefile: Added binary module support for RedHat 7.3
	- modules/makefile, common.mak: implemented workaround for
	obscure make bug causing compilation to fail with the error
	"make: expand.c:489: allocated_variable_append:
	Assertion `current_variable_set_list->next != 0' failed."
	- inf/linux_hsfi.inf: adjusted RELAYS to fix RING detection
	on AOpen FM56-SV (VEN_14F1&DEV_2F01&SUBSYS_00C1A0A0&REV_01) and added
	PCI\VEN_14F1&DEV_2F01.

* Sat May 18 2002 -
	- inf/linux_hsf.inf: added PCI\VEN_127A&DEV_2114&SUBSYS_21141436
	(Thanks to Martin Breuer)
	- scripts/hsfconfig.in: proper ISO-3166 country map entries for
	ARGENTINA, BRAZIL, CZECH_REPUBLIC, and NETHERLANDS.

* Mon May  6 2002 -
	- scripts/hsfconfig.in: warn that some 127a:1025/2005 modems might be HCF

* Sun May  5 2002 -
	- Released hsflinmodem-5.03.03.01mbsibeta02050500.

* Sun May  5 2002 -
	- Changed ttyHSF0 device name to ttySHSF0 for compatibility with wvdial.

* Fri May  3 2002 -
	- Released hsflinmodem-5.03.03.01mbsibeta02050300.

* Fri May  3 2002 -
	- Merged new Conexant modem code (v5.03.03.01).

* Thu May  2 2002 -
	- modules/osspec/include/osuniqredef.h: added to solve name collision
	problem with HCF and other modules.

* Fri Apr 26 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02042600.

* Fri Apr 26 2002 -
	- scripts/hsfconfig.in: incorporated hsfsysid, added OSKERNTYPE
	which is set to the type of kernel installed, not the architecture.
	- modules/makefile: added i386 to REDHAT_MACHINES to accomodate people
	running i386 kernels on i586/i686 machines, and link-identical-binaries
	to save space.

* Mon Apr 22 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02042200.

* Sun Apr 21 2002 -
	- general: Use AT&D2 by default.
	- scripts/hsfconfig.in: added check_unsupported() and other small
	usability improvements.
	- modules/common.mak: changed default KERNELSRC to match hsfconfig.

* Sat Apr 20 2002 -
	- modules/osspec/osstdio.c: if f_op->llseek is NULL, call default_llseek.
	(Thanks to Gratiniano S. de Haro for reporting this problem)
	- imported/hsfich.O: emit error message when encountering unsupported
	AC'97 devices.
	- modules/osspec/osdebug.c: added OsErrorPrintf() and OsErrorVPrintf().
	- scripts/hsfconfig.in: cut down size of default --dumpdiag output.

* Wed Apr 13 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02041300.

* Wed Apr 12 2002 -
	- modules/makefile: revamped directory structure for binaries and added
	several mandrake kernels.
	- makefile: added missing bracket to RPMTOPDIRDEFINED test.
	- scripts/hsfconfig.in: create /lib/modules/.../misc directory if it
	doesn't exist and add build kernel-source check.

* Wed Apr 05 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02040500.

* Wed Apr 03 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02040300.

* Wed Apr 03 2002 -
	- scripts/hsfconfig.in: added check for binary modules with OSKERNVERS_NODASH.
	- modules/osspec/osservices.c: OsSleep() workaround to prevent crashes
	on < 2.2.20 kernels.

* Thu Mar 28 2002 -
	- Added 3COM/USR 14f1:2f14 device type to linux_hsf.inf.
	(thanks to Rob Clark and Bill Ferguson)

* Tue Mar 26 2002 -
	- hsflinmodem.spec.in, makefile: added distribution-specific
	packages containing module binaries for default kernels.

	- modules/osspec/osfloat.c: provide replacement load_mxcsr
	macro for 2.2 if cpu_has_xmm.

	- modules/common.mak: added DISTRO_CFLAGS

	- modules/makefile: added rules to build binaries for mainstream
	distributions.

* Fri Mar 08 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02030800.

* Fri Mar 08 2002 -
	- hsflinmodem.spec.in: only run hsfconfig --auto in %post if
	HSF_AUTOCONFIG env variable is set, so as to match INSTALL documentation.

* Thu Mar 05 2002 -
	- Added "Conflicts: hcflinmodem" to RPM spec file as a temporary
	measure until the symbol name collisions are fixed.

* Thu Feb 26 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02022600.

* Thu Feb 12 2002 -
	- inf/linux_hsf[i].inf: use generic relay parameters.

* Thu Feb 10 2002 -
	- scripts/hsfconfig.in: if /dev/modem is in use, do not create symlink.

* Fri Feb 08 2002 -
	- inf/linux_intel_smartmc.inf: added 8086:2486 device (thanks to
	Lane Thorne <lthorne@techmate.com> who reported successfully using this inf
	file with his Gateway SoftK56 modem.)

* Tue Jan 28 2002 -
	- scripts/hsfconfig.in: added check for <linux/modversions.h>.
	- makefile, hsflinmodem.spec.in: added qtmodemon utility (present in
	debug versions only) to tar and rpm packages.

* Thu Jan 20 2002 -
	- Released hsflinmodem-4.06.06.02mbsibeta02022000.

* Fri Jan 18 2002 -
	- modules/osspec/osfloat.c: Rewrote OSFloatPrefix/Suffix() to hopefully
	fix the bad interactions with processes using floating point code.
	(Thanks to Christopher Mason <cmason@cmason.com> for reporting this)

* Fri Jan 18 2002 -
	- modules/common.mak: Always use gcc by default due to problems
	with kgcc on mandrake8.

* Thu Jan 17 2002 -
	- inf/linux_intel_smartmc.inf: Added entry for Sony Vaio FX101 ICH modem
	(Thanks to Michael Vogt <mvogt@acm.org>)

* Thu Jan 17 2002 -
	- Merged new Conexant modem code (v4.06.06.02).

* Thu Jan 16 2002 -
	- scripts/hsfconfig.in: remove modules, devfs configuration and
	/dev/ entries upon de-installation.

* Thu Jan 10 2002 -
	- modules/serial_hsf.c: comment-out message about unsupported BREAK
	sending because it confuses people.

* Tue Dec 19 2001 -
	- scripts/hsfconfig.in: update-modules support for debian.
	(Thanks to Julien Gaulmin <julien23@free.fr>)

* Tue Dec 15 2001 -
	- makefile: move uninstall target calling hsfconfig -remove before
	subdirs (removing hsfconfig).

* Tue Dec 14 2001 -
	- Released hsflinmodem-4.06.06.01mbsibeta01121400.

* Tue Dec 14 2001 -
	- Fixed module use count incrementation/decrementation to prevent
	crashes under 2.2.
	- Fixed AT&F command.
	- Improved devfs support.

* Tue Dec 09 2001 -
	- scripts/hsfconfig.in: initial devfsd support, improved dump_diagnostics

* Tue Dec 04 2001 -
	- Released hsflinmodem-4.06.06.01mbsibeta01120400.

* Tue Dec 04 2001 -
	- Merged new Conexant modem code (v4.06.06.01).
	many fixes to modulation code- modem should be more stable now.
	- Added old rockwell (127a) device types to linux_hsf.inf.
	- Added riptide (4311) device type to linux_hsf.inf (experimental).
	- Include version string in new nvram.bin file header to prevent
	inconsistencies.
	- Various other minor changes and improvements.

* Mon Dec 03 2001 -
	- INSTALL: rectify and mention more hsfconfig options.
	- modules/osspec/include/oscompat.h: Added parentheses to try_inc_mod_count.

* Thu Nov 29 2001 -
	- Makefile: Added pkgclean target, which is like 'clean' but also
	removes tar and RPM package files.

* Thu Nov 29 2001 -
	- Makefile, hsflinmodem.spec.in: Automatically append CHANGES file
	to hsflinmodem.spec (%changelog section).

* Wed Nov 28 2001 -
	- modules/common.mak: Don't bail out with $(error) if version.h
	doesn't exist under KERNELSRC.

* Wed Nov 28 2001 -
	- modules/common.mak: Fixed CONFIG_MODVERSIONS check for distributions
	that don't use standard autoconf.h.

* Wed Nov 28 2001 -
	- hsfconfig: Use /boot/vmlinuz.{version,autoconf}.h over KERNELSRC/
	files if the former correspond to the running kernel and the later don't.

	- hsfconfig: Use /usr/local/src/linux as KERNELSRC dir if it exists and
	/usr/src/linux doesn't.

* Tue Nov 27 2001 -
	- Fixed oscompat.h problem with INIT_TQUEUE() macro affecting 2.4.[0-2]

* Tue Nov 27 2001 -
	- Initial beta release (hsflinmodem-4.06.00mbsibeta01112702).
