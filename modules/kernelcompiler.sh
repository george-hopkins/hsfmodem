#!/bin/sh

# This small script tries to determine which compiler should
# be used to build kernel modules.
#
# Written by Marc Boucher <marc@linuxant.com>

KERNELVER="$1"
KERNELSRC="$2"

unset LANG; unset LOCALE; unset LC_TIME; unset LC_ALL

if [ -n "${RPM_BUILD_ROOT}${DEB_HOST_GNU_SYSTEM}" ]; then
	if [ -f "${KERNELSRC}/kernelcompiler" ]; then
		kstr="`cat \"${KERNELSRC}/kernelcompiler\"`"
	else
		kstr="gcc version 3.2"
	fi
else
	kstr="`cat /proc/version | sed -e 's/.*(\(gcc [^)]*\)).*/\1/'`"
fi

case "$kstr" in
gcc?[vV]ersion\ *)
	kstr="`echo \"$kstr\" | sed -e 's/^gcc.[vV]ersion //' -e 's/ .*//'`"
	;;
*)
	echo 1>&2 "$0: cannot determine version, using gcc"
	echo gcc
	exit 0
	;;
esac

kgccver="${kstr}"
kgccvermin="`echo \"${kgccver}\" | sed -e 's/^\([^\.]*\.[^\.]*\)\..*/\1/'`"
kgccvermaj="`echo \"${kgccver}\" | sed -e 's/^\([^\.]*\)\..*/\1/'`"

for c in gcc /usr/bin/gcc[0-9]* /usr/bin/gcc-[0-9]* kgcc egcs gcc3 gcc296 gcc-2.96 gcc295 gcc-2.95; do
	case "$c" in
	/*)
		c="`basename $c`"
		;;
	esac

	if which $c >/dev/null 2>&1; then
		v="`eval $c -v 2>&1 | tail -n 1`"
		case "$v" in
		gcc?[vV]ersion\ *)
			;;
		*)
			continue
			;;
		esac

		v="`echo $v | sed -e 's/^gcc.[vV]ersion //' -e 's/ .*//'`"
		if [ "$v" = "$kgccver" ]; then
			echo $c
			exit 0
		fi
		if [ -z "${minmatch}" ]; then
			v="`echo $v | sed -e 's/^\([^\.]*\.[^\.]*\)\..*/\1/'`"
			if [ "$v" = "$kgccvermin" ]; then
				minmatch="$c"
				continue
			fi
			if [ -z "${majmatch}" ]; then
				v="`echo $v | sed -e 's/^\([^\.]*\)\..*/\1/'`"
				if [ "$v" = "$kgccvermaj" ]; then
					majmatch="$c"
					continue
				fi
			fi
		fi
	fi
done

if [ -n "${minmatch}" ]; then
	echo "${minmatch}"
	exit 0
fi

if [ -n "${majmatch}" ]; then
	echo "${majmatch}"
	exit 0
fi

echo 1>&2 "$0: gcc version $kstr or similar is required to compile modules for your kernel"
echo false
exit 1
