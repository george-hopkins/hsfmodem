#!/usr/bin/perl
#
# Copyright (c) 2003-2004 Linuxant inc.
#
# NOTE: The use and distribution of this software is governed by the terms in
# the file LICENSE, which is included in the package. You must read this and
# agree to these terms before using or distributing this software.
#

# This script converts Conexant modem parameters specified in a
# Windows inf or cty file to the structure expected by the Linux driver.


if (@ARGV != 1) {
	die "Usage: $0 <output_dir> < cnxtmdm.{inf,cty}\n";
}

$outdir = shift(@ARGV);

%convert_keys_oem = (
	"AT+MS" => "MS_PARAMS",
#	"" => "EC_PARAMS",
#	"" => "BRK_PARAMS",
#	"" => "ITF_PARAMS",
#	"" => "AUTODETECT",
#	"" => "COMPRESSION_PARAMS",
#	"" => "V44_PARAMS",
#	"" => "RETRAIN_ENABLED",
#	"" => "CADENCE_PARAMS",
#	"" => "FORCE_RENEG_UP",
#	"" => "FORCE_RENEG_DOWN",
#	"" => "V34FAX_ENABLED",
	"SREGS" => "SREG",
#	"" => "NRINGS_TO_ANSWER",
#	"" => "RING_COUNTER",
#	"" => "ESCAPE_CHAR",
#	"" => "CR_CHAR",
#	"" => "LF_CHAR",
#	"" => "BS_CHAR",
#	"" => "BLIND_DIAL_WAIT_TIME",
#	"" => "CARRIER_WAIT_TIME",
#	"" => "PAUSE_DELAY_TIME",
#	"" => "CARDET_RESPONSE_TIME",
#	"" => "HANGUP_DELAY_TIME",
#	"" => "DTMF_DURATION",
#	"" => "ESCAPE_PROMPT_DELAY",
#	"" => "TEST_TIMER",
#	"" => "FLASH_DELAY_TIME",
#	"" => "INACTIVITY_TIME",
#	"" => "DATA_COMPRESSION",
#	"" => "EXTENDED_RESULT_CODES",
#	"" => "SREG_LAST",
#	"" => "DIALSTRING",
#	"" => "PULSE_DIAL_CONFIG",
#	"" => "TONE_DIAL_CONFIG",
#	"" => "DIALTONE_WAIT_TIME",
#	"" => "DIAL_MODE",
#	"" => "USE_S7_WHEN_W",
#	"" => "INVERT_CALLING_TONE",
#	"" => "CALL_PROGRESS_TONE_DEBOUNCE",
#	"" => "CALL_MODE",
#	"" => "CALL_PROGRESS_TIMING",
#	"" => "CALL_PROGRESS_FLAGS",
#	"" => "V8BIS_CONTROL",
#	"" => "V8BIS_RESULT",
#	"" => "PREFERRED_FLEX_VERSION",
#	"" => "V90_ENABLED",
#	"" => "V70_ALLOWED",
#	"" => "ATMESSAGE_I0",
#	"" => "ATMESSAGE_I1",
#	"" => "ATMESSAGE_I2",
#	"" => "ATMESSAGE_I3",
#	"" => "ATMESSAGE_I4",
#	"" => "ATMESSAGE_I5",
#	"" => "ATMESSAGE_I6",
#	"" => "ATMESSAGE_I7",
#	"" => "ATMESSAGE_I8",
#	"" => "ATMESSAGE_I9",
#	"" => "ATMESSAGE_I10",
#	"" => "ATMESSAGE_I11",
#	"" => "ATMESSAGE_I12",
#	"" => "DTE_ECHO",
#	"" => "DTE_CONFIG",
#	"" => "SYNC_MODE",
#	"" => "RLSD_BEHAVIOR",
#	"" => "DTR_BEHAVIOR",
#	"" => "SPEAKER_VOLUME",
#	"" => "SPEAKER_CONTROL",
#	"" => "PULSE_MAKE_BREAK",
#	"" => "IMMEDIATE_RING_DETECTED",
#	"" => "ANSWER_TONE_DETECTOR",
#	"" => "BELL_TONE_DETECTOR",
#	"" => "DTMF_GENERATOR",
#	"" => "MODULATION_REPORT",
#	"" => "MODULATION_INFO",
#	"" => "BUSY_TONE_CADENCE",
#	"" => "RINGBACK_TONE_CADENCE",
#	"" => "RING_CADENCE",
#	"" => "REORDER_TONE_CADENCE",
#	"" => "SDIAL_TONE_CADENCE",
#	"" => "DATA_CALL_TONE_CADENCE",
#	"" => "FAX_CALL_TONE_CADENCE",
#	"" => "FAX_AUTO_ANSWER",
	"FLAGS" => "OEM_FLAGS",
	"FLAGS2" => "OEM_FLAGS2",
	"FILTER" => "OEM_FILTERS",
	"SPKR_MUTE_DELAY" => "OEM_SPKR_MUTE_DELAY",
#	"" => "OEM_READ_MASK",
#	"" => "OEM_THRESHOLD",
#	"" => "COUNTRY_CODE",
#	"" => "PREVIOUS_COUNTRY_CODE",
#	"" => "COUNTRY_NAME",
#	"" => "COUNTRY_CODE_LIST",
#	"" => "COUNTRY_STRUCT",
#	"" => "COUNTRY_BLIST",
#	"" => "FILTERS",
#	"" => "DTMF",
	"RING" => "RING_PARAMS",
#	"" => "RLSDOFFSET",
#	"" => "THRESHOLD",
#	"" => "TXLEVELS",
#	"" => "RELAYS",
#	"" => "SPEED_ADJUST",
#	"" => "SREG_LIMITS",
#	"" => "PR_IGNORE_TIME",
#	"" => "USER_SYMRATE",
#	"" => "COUNTRY_CALLERID1",
#	"" => "COUNTRY_CALLERID2",
#	"" => "COUNTRY_CALL_WAITING",
#	"" => "VCID_FORMAT",
#	"" => "VRID_FORMAT",
#	"" => "CALLER_ID_INFO",
#	"" => "VRID_CRC",
#	"" => "VRID_TYPE",
#	"" => "VRID_LENGTH",
#	"" => "VRID_DATA",
#	"" => "VDR_PARAMS",
	"DEFAULT_CW" => "CALL_WAITING",
	"DEFAULT_MOH" => "MOH",
	"DEFAULT_PIG" => "PCMUPSTREAM",
#	"" => "QC",
#	"" => "QC_PROFILE",
#	"" => "V92_CONTROL",
#	"" => "PROFILE_STORED",
#	"" => "PROFILE_FACTORY",
#	"" => "POUND_UD",
#	"" => "ATPARSER_ONLINE",
#	"" => "BLACK_LIST",
#	"" => "TONE_PRIMARY_PARAMS",
#	"" => "TONE_ALTERNATE_PARAMS",
#	"" => "TONE_ATBEL_PARAMS",
#	"" => "TONE_ATV25_PARAMS",
#	"" => "TONE_1100_PARAMS",
#	"" => "TONE_2130_PARAMS",
#	"" => "TONE_2312_PARAMS",
#	"" => "TONE_2750_PARAMS",
#	"" => "TONE_2912_PARAMS",
#	"" => "VOICETONEACALLPROG_PARAMS",
#	"" => "VOICETONEB_1100_PARAMS",
#	"" => "VOICETONEC_1300_PARAMS",
#	"" => "VOICETONEC_2100_PARAMS",
#	"" => "VOICETONEB_2225_PARAMS",
#	"" => "V8BTONE_2100_PARAMS",
#	"" => "V8BTONE_2225_PARAMS",
#	"" => "CI_CENTER_TONEB_PARAMS",
#	"" => "CI_SIDE_TONEC_PARAMS",
#	"" => "VOICEVIEW_TONEC_2912_PARAMS",
#	"" => "VOICEVIEW_TONEB_2312_PARAMS",
#	"" => "VOICEVIEW_TONEB_2750_PARAMS",
#	"" => "VOICEVIEW_TONEA_2130_PARAMS",
#	"" => "VOICEVIEW_TONEA_NOT_ST_PARAMS",
#	"" => "TONEC_1650_PARAMS",
#	"" => "CReSEG1_2002PARAMS",
#	"" => "TONE_1270_PARAMS",
#	"" => "TONE_AS",
#	"" => "TONE_GEN_1650",
#	"" => "TONE_GEN_V25CALL",
#	"" => "TONE_GEN_CNG",
#	"" => "TONE_GEN_CED",
#	"" => "TONE_GEN_CI",
	"BuffersDelay" => "BUFFERS_DELAY",
#	"" => "PCI_DEVICE_ID",
#	"" => "PCI_VENDOR_ID",
	"HwData" => "OEM_HWDATA",
	"DAAType" => "OEM_DAATYPE",
	"SmartDAAParams" => "OEM_SMARTDAA_PARAMS",
	"SmartDAAParamsK3" => "OEM_SMARTDAA_PARAMSK3",
	"SmartDAAParamsHal" => "OEM_SMARTDAA_PARAMSHAL",
	"HwAdapterType" => "HW_ADAPTER_TYPE",
	"SOFT_RING_PARAMS" => "OEM_SOFT_RING_PARAMS",
#	"" => "YUKON_FIRMWARE_DATA",
#	"" => "YUKON_FIRMWARE_DELETE",
#	"" => "CTRY_TONE",
#	"" => "CTRY_CADENCE",
#	"" => "CTRY_CALLERID",
#	"" => "HSFI_TYPE",
#	"" => "CENTERAL_SITE",
#	"" => "RXSAMPLES_OUT_FILENAME",
#	"" => "TXSAMPLES_OUT_FILENAME",
#	"" => "RXLEN_OUT_FILENAME",
#	"" => "RXSAMPLES_IN_FILENAME",
#	"" => "RXLEN_IN_FILENAME",
#	"" => "PRESOURCES",
#	"" => "HW_BASE_ADDRESS",
#	"" => "DSL_HWIO_PARAMS",
#	"" => "SETTINGS_ATUR_DESCRIPTION",
#	"" => "SETTINGS_HSTUR_DESCRIPTION",
#	"" => "SETTINGS_HSTUC_DESCRIPTION",
#	"" => "SETTINGS_ATUR_PARAMS",
#	"" => "SETTINGS_HSTUR_PARAMS",
#	"" => "SETTINGS_HSTUC_PARAMS",
#	"" => "SETTINGS_NILECTRL_PARAMS",
#	"" => "SETTINGS_ATUR_DATA",
#	"" => "SETTINGS_ATUC_DATA",
#	"" => "SETTINGS_HSTUR_DATA",
#	"" => "SETTINGS_HSTUC_DATA",
#	"" => "SETTINGS_NILECTRL_DATA",
#	"" => "G9922ATU_C_CONFIGURATION",
#	"" => "G9922ATU_R_CONFIGURATION",
#	"" => "HSTU_C_CONFIGURATION",
#	"" => "HSTU_R_CONFIGURATION",
#	"" => "NILE_CTRL_CONFIGURATION",
#	"" => "HS_MODULATION_DB",
#	"" => "CO_HS_MODULATION_DB",
#	"" => "RT_HS_MODULATION_DB",
#	"" => "LOWMIPS",
#	"" => "DSL_HWIO_SAMPLE_RATE",
#	"" => "VOICE_FIRST",
#	"" => "VOICE_FDSP_PARAMS",
#	"" => "VOICE_FDSP_PREV_PARAMS",
#	"" => "VOICE_VBT_PARAMS",
#	"" => "VOICE_VDID_PARAMS",
#	"" => "VOICE_VDT_PARAMS",
#	"" => "VOICE_VGR_PARAMS",
#	"" => "VOICE_VGT_PARAMS",
#	"" => "VOICE_VLS_PARAMS",
#	"" => "VOICE_VSD_PARAMS",
#	"" => "VOICE_VSM_PARAMS",
#	"" => "VOICE_VTS_PARAMS",
#	"" => "VOICE_MISC_PARAMS",
	"MISC_VOICE_PARMS" => "VOICE_CACHED_MISC_PARAMS",
#	"" => "VOICE_VIT_PARAMS",
#	"" => "VOICE_RELAY_CONFIG",
#	"" => "VOICE_BAUD_RATE",
#	"" => "VOICE_VRA_PARAMS",
#	"" => "VOICE_VRN_PARAMS",
#	"" => "VOICE_VTD_PARAMS",
#	"" => "VOICE_DEFAULT_ONHOOK_RELAY",
#	"" => "VOICE_RX_GAIN",
#	"" => "VOICE_TX_GAIN",
#	"" => "VOICE_HOOK_CONTROL",
#	"" => "HANDSET_STATE",
#	"" => "VOICE_SUPPORTED_VSM_CODE",
#	"" => "VOICE_BOND_OPTIONS",
#	"" => "DTMF_REPORTING_ENABLE",
#	"" => "VOICE_PREVIOUS_MODE",
#	"" => "VOICE_DETECTION_MASK",

);

%convert_keys_other = (
# HCF Country parameters have CALLWAITING_PARMS instead of CALL_WAITING
	"CALLWAITING_PARMS" => "CALL_WAITING",
);

while(<STDIN>) {

    s/[\r\n]$//g;

    next unless (/^HKR,(OEM,|(OEM|Country|Profile|Region)\\)/);

    $entry = "";

    if(/^HKR,(Country|Profile|Region)\\/) {
	($entry,$idx,$key,$type,$rest) = /^HKR\s*,\s*(\w+)\\([0-9a-fA-F]+)\s*,\s*([\w\+]+)\s*,\s*([^,]*)\s*,\s*(.*)$/;

	if($entry eq "Country") {
		$entry = "Region";
	}
	if (defined $convert_keys_other{$key}) {
	    $key = $convert_keys_other{$key};
	}
    } else {
	$idx="";
	if(/^HKR,OEM\\/) {
	    ($entry,$i1,$i2,$type,$rest) = /^HKR\s*,\s*(\w+)\\([\w\+]+)\s*,\s*([\w\+]+)\s*,\s*([^,]*)\s*,\s*(.*)$/;
	    $key=$i1 . "_" . $i2;
	} else {
	    if(/^HKR,OEM,/) {
		($entry,$key,$type,$rest) = /^HKR\s*,\s*(\w+)\s*,\s*([\w\+]+)\s*,\s*([^,]*)\s*,\s*(.*)$/;
	    }
	}
	if (defined $convert_keys_oem{$key}) {
	    $key = $convert_keys_oem{$key};
	} else {
	    $key =~ s/^ATI_0*(\d)/ATMESSAGE_I\1/;
	}
    }

    if($entry eq "") {
	print STDERR "$0: unable to parse $_\n";
	next;
    }

    $rest =~ s/^\\//g;
    while(/\\$/) {
	$_ = <STDIN>;
	s/[\r\n]$//g;
	$rest .= $_ . "\n";
    }

#    print "idx: ", $idx, " key: ", $key, " type: ", $type, " rest: ", $rest, "\n";


    if ($key eq "RELAYS" || $key eq "SMART_RELAYS" || $key eq "SILABRELAYS") {
	# add padding to convert 16-bit RELAYS to 32-bits
	$rest =~ s/^([0-9A-F]{2},[0-9A-F]{2})([,\\]*)$/\1,00,00\2/gmi;

	# HCF generic relays
	if ($rest eq "07,80,00,00,\\\n07,80,00,00,\\\n04,80,00,00,\\\n00,80,00,00,\\\n05,80,00,00,\\\n07,80,00,00,\\\n01,80,00,00,\\\n03,80,00,00,\\\n00,80,00,00,\\\n03,80,00,00,\\\n00,80,00,00,\\\n00,80,00,00\n") {
		$rest="6F,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7E,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7E,80,00,00\n";
	}
	# HSF generic relays
	if (!(($outdir =~ /hsfi/) || ($outdir =~ /bry/)) && $rest eq "57,80,00,00,\\\n57,80,00,00,\\\n56,80,00,00,\\\n56,80,00,00,\\\n57,80,00,00,\\\n57,80,00,00,\\\n57,80,00,00,\\\n57,80,00,00,\\\n56,80,00,00,\\\n57,80,00,00,\\\n56,80,00,00,\\\n56,80,00,00\n") {
		$rest="7F,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7E,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7F,80,00,00,\\\n7E,80,00,00,\\\n7E,80,00,00\n";
	}
	# replace cadmus1 with cadmus2 relays
	if (($outdir =~ /cadmus2/) && $rest eq "00,08,00,00,\\\n00,00,00,00,\\\n00,08,00,00,\\\n00,08,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,08,00,00,\\\n00,00,00,00,\\\n00,08,00,00,\\\n00,08,00,00\n") {
	    if ($cty{$entry}{$idx}{'T35CODE'} =~ /^(04|0A|0F|2E|31|3C|3D|46|52|57|59|7B|82|8B|A0|A5|A6|B4),00$/) {
		$rest="05,00,20,00,\\\n00,00,00,00,\\\n05,00,20,00,\\\n05,00,20,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n05,00,20,00,\\\n00,00,00,00,\\\n05,00,20,00,\\\n05,00,20,00\n";
	    } else {
		$rest="05,00,20,00,\\\n00,00,00,00,\\\n01,00,00,00,\\\n01,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n00,00,00,00,\\\n01,00,00,00,\\\n00,00,00,00,\\\n01,00,00,00,\\\n01,00,00,00\n";
	    }
	}
    }

    if($ENV{'CVTINF_SWAP'} && $key ne "T35CODE" && $key ne "SREG") {
	if($key eq "FILTER" || $key eq "OEM_FILTERS") {
		# escape 32-bit values
		$rest =~ s/^([0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)$/__\1/gm;
		# one 16-bit value per line to facilitate swapping:
		while($rest =~ s/^([0-9A-F]{2},[0-9A-F]{2})\s*,\s*(([0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)+)$/\1,\\\n\2/gm) {};
		# remove escaping
		$rest =~ s/^__//gm;
	}
	if($key eq "MOD_THRESHOLD") {
		# one 16-bit value per line to facilitate swapping:
		while($rest =~ s/^([0-9A-F]{2},[0-9A-F]{2})\s*,\s*(([0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)+)$/\1,\\\n\2/gm) {};
	}
	if($key eq "CALL_WAITING") {
		# escape 16-bit values
		$rest =~ s/^(([0-9A-F]{2},[0-9A-F]{2},){7}[0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)$/__\1/gm;
		# one 32-bit value per line to facilitate swapping:
		while($rest =~ s/^([0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2})\s*,\s*(([0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)+)$/\1,\\\n\2/gm) {};
		# remove escaping
		$rest =~ s/^__//gm;

		# escape 32-bit values
		$rest =~ s/^([0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)$/__\1/gm;
		# one 16-bit value per line to facilitate swapping:
		while($rest =~ s/^([0-9A-F]{2},[0-9A-F]{2})\s*,\s*(([0-9A-F]{2},[0-9A-F]{2}[\\ 	,]*)+)$/\1,\\\n\2/gm) {};
		# remove escaping
		$rest =~ s/^__//gm;
	}

	# swap 32-bit values:
	$rest =~ s/^([0-9A-F]{2}),([0-9A-F]{2}),([0-9A-F]{2}),([0-9A-F]{2})([\\ 	,]*)$/\4,\3,\2,\1\5/gm;
	# swap 16-bit values:
	$rest =~ s/^([0-9A-F]{2}),([0-9A-F]{2})([\\ 	,]*)$/\2,\1\3/gm;

	sub reverse_bits {
	    my($flags) = @_;

	    $flags =~ s/,//g;
	    $flags = hex($flags);
	    my($inv) = 0;
	    for (my($i) = 0; $i < 32; $i++) {
		if($flags & (1 << $i)) {
		    $inv |= 1 << (31 - $i);
		}
	    }
	    $flags = sprintf "%08X", $inv;
	    $flags =~ s/^([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})$/\1,\2,\3,\4/g;
	    return $flags;
	}

	if($key eq "FLAGS" || $key eq "OEM_FLAGS" || $key eq "OEM_FLAGS2" || $key eq "V92_CONTROL") {
		$rest = reverse_bits($rest);
	}

	if($key eq "OEM_SMARTDAA_PARAMS") {
		$_ = $rest;
		# last 32-bit value is Flags bitfield which needs to be reversed
		($flags) = /([0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2}).*$/;
		$flags = reverse_bits($flags);
		$rest =~ s/[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2},[0-9A-F]{2}(.*)$/$flags\1/;
	}
    }

    if (${idx} eq "") {
	$cty{$entry}{$key} = $rest;
    } else {
	$cty{$entry}{$idx}{$key} = $rest;
    }

}

if ( ! -d $outdir ) {
    die "mkdir $outdir failed: $!" if ! mkdir $outdir;
}

foreach $entry (keys %cty) {

    if ($entry eq "OEM") {
	$entrydir = $outdir;
    } else {
	$entrydir = $outdir . "/" . $entry;
    }

    if ( ! -d $entrydir ) {
	die "mkdir $entrydir failed: $!" if ! mkdir $entrydir;
    }

    if ($entry eq "OEM") {

	foreach $key (keys %{$cty{$entry}}) {
	    open (OUT, ">", "$entrydir/$key");
	    print OUT $cty{$entry}{$key}, "\n";
	    close OUT;
	}

	next;
    }

    foreach $idx (keys %{$cty{$entry}}) {
	if ($entry eq "Profile") {
		$idxdir = "$entrydir/$idx";
	} else {
	    $_ = $cty{$entry}{$idx}{"T35CODE"};
	    ($b1,$b2) = /^\s*([0-9a-fA-F][0-9a-fA-F])\s*,\s*([0-9a-fA-F][0-9a-fA-F])\s*.*/;
	    if (($b1 eq "") || ($b2 eq "")) {
		die "$0: no T35CODE for $entry index $idx\n";
	    }
	    $idxdir = "$entrydir/$b2$b1";
	}

	if(defined $cty{$entry}{$idx}{"TYPE"}) {
		$type=$cty{$entry}{$idx}{"TYPE"};
		delete $cty{$entry}{$idx}{"TYPE"};
	} else {
		$type="00";
	}

	foreach $key (keys %{$cty{$entry}{$idx}}) {

	    if ( $key != "REFERENCE" ) {
		$zeroval = $cty{$entry}{$idx}{$key};
		$zeroval =~ s/#.*$//mg;
		$zeroval =~ s/[0,\\]//g;
		if ($zeroval =~ /^[0,\s]*$/) {
		    delete $cty{$entry}{$idx}{$key};
#		    print $idxdir,"/", $key, " only contains nulls, skipping\n";
		    next;
		}
	    }

	    if ($key eq "T35CODE") {
		delete $cty{$entry}{$idx}{$key};
		next;
	    }

	    if ($key eq "NAME") {
		open (OUT, ">", "${idxdir}_$key");
		if($type eq "01" ) {
			$cty{$entry}{$idx}{$key} =~ s/"$/*"/;
		}
		print OUT $cty{$entry}{$idx}{$key}, "\n";
		close OUT;
		delete $cty{$entry}{$idx}{$key};
	    }
	}

	if(defined $cty{$entry}{$idx}{"REFERENCE"}) {
	    $refidx = sprintf "%04X", hex($cty{$entry}{$idx}{"REFERENCE"});
	    $ref = "../Profile/" . $refidx;
	    if ((scalar keys %{$cty{$entry}{$idx}}) eq 1) {
#		print $idxdir, " -> ", $ref, "\n";
		symlink($ref, $idxdir);
		next;
	    } else {
#	    	if ( ! -d $idxdir ) {
#			die "$mkdir $idxdir failed: !" if ! mkdir $idxdir;
#	    	}
#		foreach $key (keys %{$cty{"Profile"}{$refidx}}) {
#			if(!defined $cty{$entry}{$idx}{$key}) {
#			    print $ref . "/" . $key, " -> ", $idxdir . "/" . $key, "\n";
#			    symlink($ref . "/" . $key, $idxdir . "/" . $key);
#			}
#		}
	    }
	}

	if ( ! -d $idxdir ) {
	    die "$mkdir $idxdir failed: !" if ! mkdir $idxdir;
	}

	foreach $key (keys %{$cty{$entry}{$idx}}) {
	    open (OUT, ">", "$idxdir/$key");
	    print OUT $cty{$entry}{$idx}{$key}, "\n";
	    close OUT;
	}
    }
}

