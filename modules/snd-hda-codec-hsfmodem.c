/*
 * HD audio interface patch for Conexant HDA audio/modem codec
 *
 * Copyright (c) 2006 Pototskiy Akex <alex.pototskiy@gmail.com>
 * 		      Takashi Iwai <tiwai@suse.de>
 * 		      Tobin Davis  <tdavis@dsl-only.net>
 *
 * Copyright (c) 2005-2008 Linuxant inc.
 *
 *  This driver is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This driver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

/* XXX: Force these options which are enabled in alsa-driver-linuxant */
#ifndef CONFIG_SND_HDA_HWDEP
#define CONFIG_SND_HDA_HWDEP
#endif

#if 0
#ifndef CONFIG_SND_HDA_POWER_SAVE
#define CONFIG_SND_HDA_POWER_SAVE
#endif
#endif

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <sound/core.h>
#include <sound/hda_codec.h>
#include <sound/hda_local.h>

#include "oscompat.h"
#include "osservices.h"

#include "osresour_ex.h"
#include "oslinux.h"

#include "oshda.h"

int cnxthwhda_suspend(void *devnode, pm_message_t state);
int cnxthwhda_resume(void *devnode);
void cnxthwhda_remove(void *ptr);
int cnxthwhda_probe(void *codec, struct device *ctrlDev, void **ppDevNode, void **ppInterfaceFuncs);

#if defined(__i386__)
#define __shimcall__ __attribute__((regparm(0)))
#else
#define __shimcall__
#endif

//#define CONFIG_SND_DEBUG

struct conexant_spec {

	struct snd_kcontrol_new *mixers[5];
	int num_mixers;

	const struct hda_verb *init_verbs[5];	/* initialization verbs
						 * don't forget NULL
						 * termination!
						 */
	unsigned int num_init_verbs;

	/* playback */
	struct hda_multi_out multiout;	/* playback set-up
					 * max_channels, dacs must be set
					 * dig_out_nid and hp_nid are optional
					 */

	/* capture */
	unsigned int num_adc_nids;
	hda_nid_t *adc_nids;
	hda_nid_t dig_in_nid;		/* digital-in NID; optional */

	unsigned int cur_adc_idx;
	hda_nid_t cur_adc;

	/* PCM information */
	struct hda_pcm pcm[3];	/* used in build_pcms() */

	/* HSF modem */
	void *modem_devnode;
	unsigned int modem_stream_tags[2];
	int modem_do_prepare[2];
	void (*modem_cbHdaEvent)(void *Context, unsigned int res) __shimcall__;
	void *modem_cbHdaEventContext;
	unsigned char modem_cbHdaTag;
};

static int conexant_playback_pcm_open(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_open(codec, &spec->multiout, substream,
					     hinfo);
}

static int conexant_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
					 struct hda_codec *codec,
					 unsigned int stream_tag,
					 unsigned int format,
					 struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_prepare(codec, &spec->multiout,
						stream_tag,
						format, substream);
}

static int conexant_playback_pcm_cleanup(struct hda_pcm_stream *hinfo,
					 struct hda_codec *codec,
					 struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_analog_cleanup(codec, &spec->multiout);
}

/*
 * Digital out
 */
static int conexant_dig_playback_pcm_open(struct hda_pcm_stream *hinfo,
					  struct hda_codec *codec,
					  struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_open(codec, &spec->multiout);
}

static int conexant_dig_playback_pcm_close(struct hda_pcm_stream *hinfo,
					 struct hda_codec *codec,
					 struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_close(codec, &spec->multiout);
}

static int conexant_dig_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
					 struct hda_codec *codec,
					 unsigned int stream_tag,
					 unsigned int format,
					 struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	return snd_hda_multi_out_dig_prepare(codec, &spec->multiout,
					     stream_tag,
					     format, substream);
}

/*
 * Analog capture
 */
static int conexant_capture_pcm_prepare(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      unsigned int stream_tag,
				      unsigned int format,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	snd_hda_codec_setup_stream(codec, spec->adc_nids[substream->number],
				   stream_tag, 0, format);
	return 0;
}

static int conexant_capture_pcm_cleanup(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	snd_hda_codec_cleanup_stream(codec, spec->adc_nids[substream->number]);
	return 0;
}

static int conexant_modem_pcm_prepare(struct hda_pcm_stream *hinfo,
			      struct hda_codec *codec,
			      unsigned int stream_tag,
			      unsigned int format,
			      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;

	//printk(KERN_DEBUG"%s: codec=%p stream=%d stream_tag=%x format=0x%x substream=%p\n", __FUNCTION__, codec, substream->stream, stream_tag, format, substream);

	spec->modem_stream_tags[substream->stream] = stream_tag;

	return 0;
}

static int conexant_modem_pcm_open(struct hda_pcm_stream *hinfo, struct hda_codec *codec,
			    struct snd_pcm_substream *substream)
{
	static unsigned int rates[] = { 16000 };
	static struct snd_pcm_hw_constraint_list hw_constraints_rates = {
		.count = ARRAY_SIZE(rates),
		.list = rates,
		.mask = 0,
	};

	//printk(KERN_DEBUG"%s: codec=%p substream=%p\n", __FUNCTION__, codec, substream);

	return snd_pcm_hw_constraint_list(substream->runtime, 0, SNDRV_PCM_HW_PARAM_RATE, &hw_constraints_rates);
}


static struct hda_pcm_stream conexant_pcm_analog_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.nid = 0, /* fill later */
	.ops = {
		.open = conexant_playback_pcm_open,
		.prepare = conexant_playback_pcm_prepare,
		.cleanup = conexant_playback_pcm_cleanup
	},
};

static struct hda_pcm_stream conexant_pcm_analog_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.nid = 0, /* fill later */
	.ops = {
		.prepare = conexant_capture_pcm_prepare,
		.cleanup = conexant_capture_pcm_cleanup
	},
};


static struct hda_pcm_stream conexant_pcm_digital_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.nid = 0, /* fill later */
	.ops = {
		.open = conexant_dig_playback_pcm_open,
		.close = conexant_dig_playback_pcm_close,
		.prepare = conexant_dig_playback_pcm_prepare
	},
};

static struct hda_pcm_stream conexant_pcm_digital_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	/* NID is set in alc_build_pcms */
};

static int cx5051_capture_pcm_prepare(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      unsigned int stream_tag,
				      unsigned int format,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	spec->cur_adc = spec->adc_nids[spec->cur_adc_idx];
	snd_hda_codec_setup_stream(codec, spec->cur_adc, stream_tag, 0, format);
	return 0;
}

static int cx5051_capture_pcm_cleanup(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;
	snd_hda_codec_cleanup_stream(codec, spec->cur_adc);
	spec->cur_adc = 0;
	return 0;
}

static struct hda_pcm_stream cx5051_pcm_analog_capture = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 2,
	.nid = 0, /* fill later */
	.ops = {
		.prepare = cx5051_capture_pcm_prepare,
		.cleanup = cx5051_capture_pcm_cleanup
	},
};

static struct hda_pcm_stream conexant_modem_pcm = {
	.substreams = 1,
	.channels_min = 1,
	.channels_max = 1,
	.nid = 0x1,
	.rates = SNDRV_PCM_RATE_16000,
	.formats = SNDRV_PCM_FMTBIT_S16_LE,
	.maxbps = 16,
	.ops = {
		.open = conexant_modem_pcm_open,
		.prepare = conexant_modem_pcm_prepare,
	},
};

static int conexant_build_pcms(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	struct hda_pcm *info = spec->pcm;

	//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);

	codec->num_pcms = 0;
	codec->pcm_info = info;

	if (codec->afg) {
		info->name = "CONEXANT Analog";
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] = conexant_pcm_analog_playback;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max =
			spec->multiout.max_channels;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid =
			spec->multiout.dac_nids[0];
		if (codec->vendor_id == 0x14f15051)
			info->stream[SNDRV_PCM_STREAM_CAPTURE] =
				cx5051_pcm_analog_capture;
		else
			info->stream[SNDRV_PCM_STREAM_CAPTURE] =
				conexant_pcm_analog_capture;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].substreams = spec->num_adc_nids;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].nid = spec->adc_nids[0];

		info++;
		codec->num_pcms++;
	}

	if (spec->multiout.dig_out_nid) {
		info->name = "Conexant Digital Audio";
		info->pcm_type = HDA_PCM_TYPE_SPDIF;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] =
			conexant_pcm_digital_playback;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid =
			spec->multiout.dig_out_nid;
		if (spec->dig_in_nid) {
			info->stream[SNDRV_PCM_STREAM_CAPTURE] =
				conexant_pcm_digital_capture;
			info->stream[SNDRV_PCM_STREAM_CAPTURE].nid =
				spec->dig_in_nid;
		}

		info++;
		codec->num_pcms++;
	}

	if (codec->mfg) {
		conexant_modem_pcm.nid = codec->mfg;

		info->name = "Conexant HSF Modem";
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] = conexant_modem_pcm;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = conexant_modem_pcm.nid;
		info->stream[SNDRV_PCM_STREAM_CAPTURE]  = conexant_modem_pcm;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].nid  = conexant_modem_pcm.nid;
		info->pcm_type = HDA_PCM_TYPE_MODEM;

		info++;
		codec->num_pcms++;
	}

	return 0;
}

static void *conexant_hsfmodem_interface_funcs[];

static int conexant_init(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	int i, ret;

	//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);
	if(codec->mfg) {
		//snd_hda_codec_write(codec, AC_NODE_ROOT, 0, AC_VERB_SET_CODEC_RESET, 0);

		ret = cnxthwhda_probe(codec, codec->bus->card->dev, &spec->modem_devnode, conexant_hsfmodem_interface_funcs);
		if(ret) {
			printk(KERN_ERR"%s: cnxthwhda_probe() failed: %d\n", __FUNCTION__, ret);
			if(!codec->afg)
				return ret;
		}
	}

	for (i = 0; i < spec->num_init_verbs; i++)
		snd_hda_sequence_write(codec, spec->init_verbs[i]);

	return 0;
}

static int conexant_exit(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;

	//printk(KERN_DEBUG"%s: codec=%p spec=%p\n", __FUNCTION__, codec, spec);

	if (codec->mfg && spec && spec->modem_devnode) {
		if (((POS_DEVNODE)spec->modem_devnode)->hwInUse)
			return -EBUSY;

		cnxthwhda_remove(spec->modem_devnode);
		spec->modem_devnode = NULL;
	}

	return 0;
}

static void conexant_free(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;

	if (spec) {
		codec->spec = NULL;
		memset(&codec->patch_ops, 0, sizeof(codec->patch_ops));
		kfree(codec->spec);
	}
}

static void conexant_unsol_event(struct hda_codec *codec, unsigned int res)
{
	struct conexant_spec *spec = codec->spec;

	//printk(KERN_DEBUG"%s: codec=%p res=0x%02x spec=%p cbHdaEvent=%p\n", __FUNCTION__, codec, res, spec, spec->modem_cbHdaEvent);
  
	if(codec->mfg && spec && spec->modem_cbHdaEvent) {
		if(((res >> 26) & 0x3f) == spec->modem_cbHdaTag) {
			//printk(KERN_DEBUG"%s: res=0x%02x cbHdaEvent=%p\n", __FUNCTION__, res, spec->modem_cbHdaEvent);
			//printk(KERN_DEBUG"%s: calling cbHdaEvent=%p ctx=%p\n", __FUNCTION__, spec->modem_cbHdaEvent, spec->modem_cbHdaEventContext);
			spec->modem_cbHdaEvent(spec->modem_cbHdaEventContext, res);
		} else {
			printk(KERN_DEBUG"%s: ignoring res=0x08%x\n", __FUNCTION__, res);
		}
	}
}

#ifdef CONFIG_PM
static int conexant_suspend(struct hda_codec *codec, pm_message_t state)
{
	struct conexant_spec *spec = codec->spec;
	int ret = 0;

	//printk(KERN_DEBUG"%s: codec=%p spec=%p state=0x%x\n", __FUNCTION__, codec, spec, *((u32 *)&state));

	if(spec && spec->modem_devnode) {
		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume++;
		}
		ret = cnxthwhda_suspend(spec->modem_devnode, state);
		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume--;
		}
	}

	return ret;
}
  
static int conexant_resume(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	int ret = 0;
	hda_nid_t mfg;

	mfg = codec->mfg;
	codec->mfg = 0;
	codec->patch_ops.init(codec);
	codec->mfg = mfg;

	snd_hda_codec_resume_amp(codec);
	snd_hda_codec_resume_cache(codec);

	if(spec && spec->modem_devnode) {
    	//snd_hda_codec_write(codec, AC_NODE_ROOT, 0, AC_VERB_SET_CODEC_RESET, 0);

		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume++;
		}

		ret = cnxthwhda_resume(spec->modem_devnode);

		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume--;
		}
	}

	return ret;
}
#endif

static int conexant_build_controls(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	unsigned int i;
	int err;

	//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);

	for (i = 0; i < spec->num_mixers; i++) {
		err = snd_hda_add_new_ctls(codec, spec->mixers[i]);
		if (err < 0)
			return err;
	}
	if (spec->multiout.dig_out_nid) {
		err = snd_hda_create_spdif_out_ctls(codec,
						    spec->multiout.dig_out_nid);
		if (err < 0)
			return err;
		err = snd_hda_create_spdif_share_sw(codec,
						    &spec->multiout);
		if (err < 0)
			return err;
		spec->multiout.share_spdif = 1;
	} 
	if (spec->dig_in_nid) {
		err = snd_hda_create_spdif_in_ctls(codec,spec->dig_in_nid);
		if (err < 0)
			return err;
	}
	return 0;
}

static struct hda_codec_ops conexant_patch_ops = {
	.build_controls = conexant_build_controls,
	.build_pcms = conexant_build_pcms,
	.init = conexant_init,
	.exit = conexant_exit,
	.free = conexant_free,
	.unsol_event = conexant_unsol_event,
#ifdef CONFIG_PM
	.suspend = conexant_suspend,
	.resume = conexant_resume,
#endif
};

static int patch_cxthsfmodem(struct hda_codec *codec)
{
	struct conexant_spec *spec;

	//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);
	if(!codec->mfg) { // we only support modems here
		return -ENODEV;
	}

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;
	codec->spec = spec;

	codec->patch_ops = conexant_patch_ops;

	return 0;
}

static struct hda_codec_preset snd_hda_preset_conexant[] = {
	{ .id = 0x14f12bfa, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsfmodem },
	{ .id = 0x14f12c06, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsfmodem },
	{ .id = 0x14f10000, .mask = 0xffff0000, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsfmodem },
  	{} /* terminator */
};


static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecGetAddr(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->addr;
}

static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecGetVendorId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->vendor_id;
}

static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecGetSubsystemId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->subsystem_id;
}

static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecGetRevisionId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->revision_id;
}

static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecRead(PHDAOSHAL pHdaOsHal, unsigned short nid, int direct, unsigned int verb, unsigned int para)
{
#if 1
	return snd_hda_codec_read((struct hda_codec *)pHdaOsHal->hda_codec, nid, direct, verb, para);
#else
	unsigned int res;

	res = snd_hda_codec_read((struct hda_codec *)pHdaOsHal->hda_codec, nid, direct, verb, para);

	printk(KERN_DEBUG"%s: nid=%x direct=%d verb=0x%x para=0x%x res=0x%08x\n", __FUNCTION__, nid, direct, verb, para, res);

	return res;
#endif
}

static __shimcall__
unsigned int conexant_hsfmodem_OsHdaCodecWallclock(PHDAOSHAL pHdaOsHal)
{
	return snd_hda_codec_wallclock((struct hda_codec *)pHdaOsHal->hda_codec);
}

static __shimcall__
void conexant_hsfmodem_OsHdaCodecSetEventCallback(PHDAOSHAL pHdaOsHal, void (*cbHdaEvent)(void *Context, unsigned int res), void *cbHdaEventContext, unsigned char *cbHdaTag)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;

	*cbHdaTag = ((struct hda_codec *)pHdaOsHal->hda_codec)->mfg;

	spec->modem_cbHdaTag = *cbHdaTag;
	spec->modem_cbHdaEventContext = cbHdaEventContext;
	spec->modem_cbHdaEvent = (void*)cbHdaEvent;
}

static __shimcall__
void conexant_hsfmodem_OsHdaCodecClearEventCallback(PHDAOSHAL pHdaOsHal, unsigned char cbHdaTag)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;

	if(spec) {
		spec->modem_cbHdaEvent = NULL;
		spec->modem_cbHdaEventContext = NULL;
		spec->modem_cbHdaTag = 0;
	}
}


#include <sound/pcm_params.h>

static int snd_interval_refine_set(struct snd_interval *i, unsigned int val)
{
	struct snd_interval t;
	t.empty = 0;
	t.min = t.max = val;
	t.openmin = t.openmax = 0;
	t.integer = 1;
	return snd_interval_refine(i, &t);
}

static int _snd_pcm_hw_param_set(struct snd_pcm_hw_params *params,
				 snd_pcm_hw_param_t var, unsigned int val,
				 int dir)
{
	int changed;
	if (hw_is_mask(var)) {
		struct snd_mask *m = hw_param_mask(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_mask_none(m);
		} else {
			if (dir > 0)
				val++;
			else if (dir < 0)
				val--;
			changed = snd_mask_refine_set(hw_param_mask(params, var), val);
		}
	} else if (hw_is_interval(var)) {
		struct snd_interval *i = hw_param_interval(params, var);
		if (val == 0 && dir < 0) {
			changed = -EINVAL;
			snd_interval_none(i);
		} else if (dir == 0)
			changed = snd_interval_refine_set(i, val);
		else {
			struct snd_interval t;
			t.openmin = 1;
			t.openmax = 1;
			t.empty = 0;
			t.integer = 0;
			if (dir < 0) {
				t.min = val - 1;
				t.max = val;
			} else {
				t.min = val;
				t.max = val+1;
			}
			changed = snd_interval_refine(i, &t);
		}
	} else
		return -EINVAL;
	if (changed) {
		params->cmask |= 1 << var;
		params->rmask |= 1 << var;
	}
	return changed;
}

static int conexant_modem_snd_pcm_change_params(struct snd_pcm_substream *substream, int hw_param_buffer_bytes)
{
	struct snd_pcm_hw_params *sparams;
	int err;
 
	sparams = kmalloc(sizeof(*sparams), GFP_KERNEL);
	if (/*!params ||*/ !sparams) {
		return -ENOMEM;
	}

	_snd_pcm_hw_params_any(sparams);
	_snd_pcm_hw_param_set(sparams, SNDRV_PCM_HW_PARAM_BUFFER_BYTES,
                                       hw_param_buffer_bytes, 0);

	snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_DROP, NULL);

	if ((err = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_HW_PARAMS, sparams)) < 0) {
		printk(KERN_ERR"%s: SNDRV_PCM_IOCTL_HW_PARAMS failed (%d)\n", __FUNCTION__, err);
		//return err;
	}

	return 0;
}

static int conexant_modem_snd_pcm_prepare_substream(PHDAOSHAL pHdaOsHal, struct snd_pcm_substream *substream)
{
	int err;
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;

	if(spec->modem_do_prepare[substream->stream]) {
		err = snd_pcm_kernel_ioctl(substream, SNDRV_PCM_IOCTL_PREPARE, NULL);
		if (err < 0) {
			printk(KERN_ERR"%s: SNDRV_PCM_IOCTL_PREPARE failed (%d)\n", __FUNCTION__, err);
			return err;
		}
		spec->modem_do_prepare[substream->stream] = 0;
	}

	return 0;
}

static __shimcall__
int conexant_hsfmodem_OsHdaCodecOpenDMA(PHDAOSHAL pHdaOsHal, int hw_param_buffer_bytes, void **ppPlaybackStream, void **ppCaptureStream)
{
	int i, err;
	struct hda_codec *codec = (struct hda_codec *)pHdaOsHal->hda_codec;
	struct hda_pcm *info = codec->pcm_info;
	struct snd_pcm *pcm;
	struct conexant_spec *spec = codec->spec;

	struct snd_pcm_substream *psubstream = NULL, *csubstream = NULL;
	static struct file fil;

	for(i = 0; i < codec->num_pcms && info[i].pcm_type != HDA_PCM_TYPE_MODEM; i++);
	if(i == codec->num_pcms) {
		printk(KERN_ERR"%s: modem pcm not found\n", __FUNCTION__);
		return -ENOENT;
	}
	pcm = info[i].pcm;

	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_PLAYBACK, &fil, &psubstream)) < 0) {
		printk(KERN_ERR"%s: snd_pcm_open_substream STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
		return err;
	}

	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_CAPTURE, &fil, &csubstream)) < 0) {
		printk(KERN_ERR"%s: snd_pcm_open_substream STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
		return err;
	}

	err = conexant_modem_snd_pcm_change_params(psubstream, hw_param_buffer_bytes);
	if (err < 0) {
		printk(KERN_ERR"%s: conexant_modem_snd_pcm_change_params STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
	}

	err = conexant_modem_snd_pcm_change_params(csubstream, hw_param_buffer_bytes);
	if (err < 0) {
		printk(KERN_ERR"%s: conexant_modem_snd_pcm_change_params STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
	}

#if 0
	printk(KERN_DEBUG"%s: psubstream=%p dma_buffer_p=%p area=%p addr=0x%lx bytes=%d\n", __FUNCTION__,
			psubstream,
			psubstream->runtime->dma_buffer_p,
			psubstream->runtime->dma_area,
			(unsigned long)psubstream->runtime->dma_addr,
			psubstream->runtime->dma_bytes);

	printk(KERN_DEBUG"%s: csubstream=%p dma_buffer_p=%p area=%p addr=0x%lx bytes=%d\n", __FUNCTION__,
			csubstream,
			csubstream->runtime->dma_buffer_p,
			csubstream->runtime->dma_area,
			(unsigned long)csubstream->runtime->dma_addr,
			csubstream->runtime->dma_bytes);
#endif

	spec->modem_do_prepare[psubstream->stream] = 1;
	spec->modem_do_prepare[csubstream->stream] = 1;

	if ((err = conexant_modem_snd_pcm_prepare_substream(pHdaOsHal, psubstream)) < 0)
		return err;

	if ((err = conexant_modem_snd_pcm_prepare_substream(pHdaOsHal, csubstream)) < 0)
		return err;

	*ppPlaybackStream = psubstream;
	*ppCaptureStream = csubstream;

	return 0;
}

static void conexant_modem_snd_pcm_close_stream(struct snd_pcm_substream *substream)
{
#if SNDRV_PCM_VERSION <= SNDRV_PROTOCOL_VERSION(2, 0, 7)
	snd_pcm_stream_lock_irq(substream);
	if (snd_pcm_running(substream))
		snd_pcm_stop(substream, SNDRV_PCM_STATE_SETUP);
	snd_pcm_stream_unlock_irq(substream);

	if (substream->ops->hw_free != NULL)
		substream->ops->hw_free(substream);

	substream->ops->close(substream);
#endif
	//printk(KERN_DEBUG"%s: substream=%p refcount=%d\n", __FUNCTION__, substream, substream->ref_count);
	snd_pcm_release_substream(substream);
}

static __shimcall__
void conexant_hsfmodem_OsHdaCodecCloseDMA(PHDAOSHAL pHdaOsHal, void *pPlaybackStream, void *pCaptureStream)
{
	//printk(KERN_DEBUG"%s: pHdaOsHal=%p pPlaybackStream=%p pCaptureStream=%p\n", __FUNCTION__, pHdaOsHal, pPlaybackStream, pCaptureStream);
	if(pHdaOsHal) {
		if(pPlaybackStream)
			conexant_modem_snd_pcm_close_stream(pPlaybackStream);
		if(pCaptureStream)
			conexant_modem_snd_pcm_close_stream(pCaptureStream);
	}
}

static __shimcall__
void conexant_hsfmodem_OsHdaCodecDMAInfo(PHDAOSHAL pHdaOsHal, void *Stream, unsigned char *StreamID, unsigned long *FifoSize, short unsigned int **ppBufAddr)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;
	struct snd_pcm_substream *substream = (struct snd_pcm_substream *)Stream;

#if 0
	printk(KERN_DEBUG"%s: substream=%p stream=%u id/tag=%u fifo_size=%u bufAddr=%p\n", __FUNCTION__,
			substream,
			substream->stream,
			spec->modem_stream_tags[substream->stream],
			substream->runtime->hw.fifo_size,
			substream->runtime->dma_area);
#endif

	*StreamID = spec->modem_stream_tags[substream->stream];
	*FifoSize = substream->runtime->hw.fifo_size; // XXX
	*ppBufAddr = (short unsigned int *)substream->runtime->dma_area;
}

static __shimcall__
int conexant_hsfmodem_OsHdaCodecSetDMAState(PHDAOSHAL pHdaOsHal, OSHDA_STREAM_STATE streamState, void *pPlaybackStream, void *pCaptureStream)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;
        struct snd_pcm_substream *psubstream = (struct snd_pcm_substream *)pPlaybackStream;
        struct snd_pcm_substream *csubstream = (struct snd_pcm_substream *)pCaptureStream;
	int err = 0, cmd;
	unsigned long flags, flags2;

	switch(streamState) {
		case OsHdaStreamStateRun:
//			printk(KERN_DEBUG"%s: Run\n", __FUNCTION__);
			if ((err = conexant_modem_snd_pcm_prepare_substream(pHdaOsHal, psubstream)) < 0)
				return err;
			if ((err = conexant_modem_snd_pcm_prepare_substream(pHdaOsHal, csubstream)) < 0)
				return err;
 
			cmd = SNDRV_PCM_IOCTL_START;
			psubstream->runtime->start_threshold = 1;
			psubstream->runtime->stop_threshold = psubstream->runtime->boundary;
			csubstream->runtime->start_threshold = 1;
			csubstream->runtime->stop_threshold = csubstream->runtime->boundary;
			break;
		case OsHdaStreamStateStop:
//			printk(KERN_DEBUG"%s: Stop\n", __FUNCTION__);
			cmd = SNDRV_PCM_IOCTL_DROP;
			break;
		case OsHdaStreamStateReset:
//			printk(KERN_DEBUG"%s: Reset\n", __FUNCTION__);
			cmd = SNDRV_PCM_IOCTL_RESET;
			psubstream->runtime->start_threshold = psubstream->runtime->boundary;
			csubstream->runtime->start_threshold = csubstream->runtime->boundary;
			break;
		default:
			printk(KERN_ERR"%s: unknown state %d\n", __FUNCTION__, streamState);
			return -ENOSYS;
	}

	switch(cmd) {
		case SNDRV_PCM_IOCTL_START:
			snd_pcm_stream_lock_irqsave(psubstream, flags);
			snd_pcm_stream_lock_irqsave(csubstream, flags2);
			psubstream->ops->trigger(psubstream, SNDRV_PCM_TRIGGER_START);
			csubstream->ops->trigger(csubstream, SNDRV_PCM_TRIGGER_START);
			snd_pcm_stream_unlock_irqrestore(csubstream, flags2);
			snd_pcm_stream_unlock_irqrestore(psubstream, flags);
			break;
		case SNDRV_PCM_IOCTL_DROP:
			snd_pcm_stream_lock_irqsave(psubstream, flags);
			snd_pcm_stream_lock_irqsave(csubstream, flags2);
			psubstream->runtime->start_threshold = psubstream->runtime->boundary;
			csubstream->runtime->start_threshold = csubstream->runtime->boundary;
			psubstream->ops->trigger(psubstream, SNDRV_PCM_TRIGGER_STOP);
			csubstream->ops->trigger(csubstream, SNDRV_PCM_TRIGGER_STOP);
			snd_pcm_stream_unlock_irqrestore(csubstream, flags2);
			snd_pcm_stream_unlock_irqrestore(psubstream, flags);
			break;
		case SNDRV_PCM_IOCTL_RESET:
			//psubstream->ops->trigger(psubstream, SNDRV_PCM_TRIGGER_STOP);
			//csubstream->ops->trigger(csubstream, SNDRV_PCM_TRIGGER_STOP);
			/*FALLTHROUGH*/
		default:
			err = snd_pcm_kernel_ioctl(psubstream, cmd, NULL);
			if (err < 0) {
				printk(KERN_ERR"%s: snd_pcm_kernel_ioctl (playback) failed (%d)\n", __FUNCTION__, err);
			}

			err = snd_pcm_kernel_ioctl(csubstream, cmd, NULL);
			if (err < 0) {
				printk(KERN_ERR"%s: snd_pcm_kernel_ioctl (capture) failed (%d)\n", __FUNCTION__, err);
			}
			break;
	}

	if(cmd != SNDRV_PCM_IOCTL_START) {
		spec->modem_do_prepare[psubstream->stream] = 1;
		spec->modem_do_prepare[csubstream->stream] = 1;
	}

	return err;
}

static __shimcall__
unsigned long conexant_hsfmodem_OsHdaCodecGetDMAPos(PHDAOSHAL pHdaOsHal, void *Stream)
{
	struct hda_codec *codec = (struct hda_codec *)pHdaOsHal->hda_codec;
	struct snd_pcm_substream *substream = (struct snd_pcm_substream *)Stream;
	int ret;

	if(codec->bus->ops.get_linkpos)
		ret = codec->bus->ops.get_linkpos(substream);
	else
		ret = frames_to_bytes(substream->runtime, substream->ops->pointer(substream));

	//printk(KERN_DEBUG"%s: substream=%p pos=%ld, ret=%d\n", __FUNCTION__, substream, pos, ret);

	return ret;
}


static void *conexant_hsfmodem_interface_funcs[] = {
	conexant_hsfmodem_OsHdaCodecGetAddr,
	conexant_hsfmodem_OsHdaCodecGetVendorId,
	conexant_hsfmodem_OsHdaCodecGetSubsystemId,
	conexant_hsfmodem_OsHdaCodecGetRevisionId,
	conexant_hsfmodem_OsHdaCodecRead,
	conexant_hsfmodem_OsHdaCodecWallclock,
	conexant_hsfmodem_OsHdaCodecSetEventCallback,
	conexant_hsfmodem_OsHdaCodecClearEventCallback,
	conexant_hsfmodem_OsHdaCodecOpenDMA,
	conexant_hsfmodem_OsHdaCodecCloseDMA,
	conexant_hsfmodem_OsHdaCodecDMAInfo,
	conexant_hsfmodem_OsHdaCodecSetDMAState,
	conexant_hsfmodem_OsHdaCodecGetDMAPos,
	NULL
};

MODULE_ALIAS("snd-hda-codec-id:14f12bfa");
MODULE_ALIAS("snd-hda-codec-id:14f12c06");
MODULE_ALIAS("snd-hda-codec-id:14f1");

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Conexant HD HSF modem codec");

static struct hda_codec_preset_list conexant_list = {
	.preset = snd_hda_preset_conexant,
	.owner = THIS_MODULE,
};

static int __init patch_conexant_init(void)
{
	return snd_hda_add_codec_preset(&conexant_list);
}

static void __exit patch_conexant_exit(void)
{
	snd_hda_delete_codec_preset(&conexant_list);
}

module_init(patch_conexant_init)
module_exit(patch_conexant_exit)
