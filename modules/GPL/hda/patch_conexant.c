/*
 * HD audio interface patch for Conexant HDA audio/modem codec
 *
 * Copyright (c) 2006 Pototskiy Akex <alex.pototskiy@gmail.com>
 * 		      Takashi Iwai <tiwai@suse.de>
 * 		      Tobin Davis  <tdavis@dsl-only.net>
 *
 * Copyright (c) 2005-2006 Linuxant inc.
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

#include <sound/driver.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <sound/core.h>
#include "hda_codec.h"
#include "hda_local.h"

#define CXT_PIN_DIR_IN              0x00
#define CXT_PIN_DIR_OUT             0x01
#define CXT_PIN_DIR_INOUT           0x02
#define CXT_PIN_DIR_IN_NOMICBIAS    0x03
#define CXT_PIN_DIR_INOUT_NOMICBIAS 0x04

#define CONEXANT_HP_EVENT	0x37
#define CONEXANT_MIC_EVENT	0x38


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
	unsigned int cur_eapd;
	unsigned int hp_present;
	unsigned int portb_present;
	unsigned int portc_present;
	unsigned int need_dac_fix;

	/* capture */
	unsigned int num_adc_nids;
	hda_nid_t *adc_nids;
	hda_nid_t dig_in_nid;		/* digital-in NID; optional */

	/* capture source */
	const struct hda_input_mux *input_mux;
	hda_nid_t *capsrc_nids;
	unsigned int cur_mux[3];

	/* channel model */
	const struct hda_channel_mode *channel_mode;
	int num_channel_mode;

	/* PCM information */
	struct hda_pcm pcm[3];	/* used in build_pcms() */

	struct mutex amp_mutex;	/* PCM volume/mute control mutex */
	unsigned int spdif_route;

	/* dynamic controls, init_verbs and input_mux */
	struct auto_pin_cfg autocfg;
	unsigned int num_kctl_alloc, num_kctl_used;
	struct snd_kcontrol_new *kctl_alloc;
	struct hda_input_mux private_imux;
	hda_nid_t private_dac_nids[4];

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
	return snd_hda_multi_out_analog_open(codec, &spec->multiout, substream);
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

	if (spec->num_adc_nids > 1)
	{
//	printk(KERN_DEBUG"conexant_capture_pcm_prepare spec->portb_present =%x\n",spec->portb_present);
//	printk(KERN_DEBUG"conexant_capture_pcm_prepare spec->portc_present =%x\n",spec->portc_present);
//		if(spec->portb_present)
//			snd_hda_codec_setup_stream(codec, spec->adc_nids[substream->number],
//						stream_tag, 0, format);
//		else
			snd_hda_codec_setup_stream(codec, spec->adc_nids[spec->portc_present],
						stream_tag, 0, format);
	}
	else
	{
		snd_hda_codec_setup_stream(codec, spec->adc_nids[substream->number],
					stream_tag, 0, format);
	}
	return 0;
}

static int conexant_capture_pcm_cleanup(struct hda_pcm_stream *hinfo,
				      struct hda_codec *codec,
				      struct snd_pcm_substream *substream)
{
	struct conexant_spec *spec = codec->spec;

	if (spec->num_adc_nids > 1)
	{
		snd_hda_codec_setup_stream(codec, spec->adc_nids[0],
					0, 0, 0);
		snd_hda_codec_setup_stream(codec, spec->adc_nids[1],
					0, 0, 0);
	}
	else
	{
		snd_hda_codec_setup_stream(codec, spec->adc_nids[substream->number],
					0, 0, 0);
	}
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
		//.close = conexant_modem_pcm_close,
		.prepare = conexant_modem_pcm_prepare,
		//.cleanup = conexant_modem_pcm_cleanup,
	},
};

/* Used by conexant_build_pcms to flag that a PCM has no playback stream */
static struct hda_pcm_stream conexant_pcm_null_playback = {
	.substreams = 0,
	.channels_min = 0,
	.channels_max = 0,
};

static int conexant_build_pcms(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	struct hda_pcm *info = spec->pcm;

	codec->num_pcms = 0;
	codec->pcm_info = info;

	if(codec->afg) {
		info->name = "Conexant Analog Audio";
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] = conexant_pcm_analog_playback;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max =
			spec->multiout.max_channels;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid =
			spec->multiout.dac_nids[0];
		info->stream[SNDRV_PCM_STREAM_CAPTURE] = conexant_pcm_analog_capture;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].substreams = 1;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].nid = spec->adc_nids[0];

		info++;
		codec->num_pcms++;
	}

	if (spec->multiout.dig_out_nid) {
		info->name = "Conexant Digital Audio";
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

	if(codec->mfg) {
//printk(KERN_DEBUG"%s: codec=%p mfg=%d\n", __FUNCTION__, codec, codec->mfg);

		conexant_modem_pcm.nid = codec->mfg;

		info->name = "Conexant HSF Modem";
		info->stream[SNDRV_PCM_STREAM_PLAYBACK] = conexant_modem_pcm;
		info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = conexant_modem_pcm.nid;
		info->stream[SNDRV_PCM_STREAM_CAPTURE]  = conexant_modem_pcm;
		info->stream[SNDRV_PCM_STREAM_CAPTURE].nid  = conexant_modem_pcm.nid;
		info->is_modem = 1;

		info++;
		codec->num_pcms++;
	}

	return 0;
}

static int conexant_mux_enum_info(struct snd_kcontrol *kcontrol,
	       			  struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;

	return snd_hda_input_mux_info(spec->input_mux, uinfo);
}

static int conexant_mux_enum_get(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	unsigned int adc_idx = snd_ctl_get_ioffidx(kcontrol, &ucontrol->id);

	ucontrol->value.enumerated.item[0] = spec->cur_mux[adc_idx];
	return 0;
}

static int conexant_mux_enum_put(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	unsigned int adc_idx = snd_ctl_get_ioffidx(kcontrol, &ucontrol->id);

	return snd_hda_input_mux_put(codec, spec->input_mux, ucontrol,
				     spec->capsrc_nids[adc_idx],
				     &spec->cur_mux[adc_idx]);
}

static void *conexant_hsf_interface_funcs[];

static int conexant_init(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	int i, ret;

//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);
	if(codec->mfg) {
		int (*cnxthwhda_probe)(void *codec, struct device *hwDev, void **ppDevNode, void **ppInterfaceFuncs);

//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);
    	//snd_hda_codec_write(codec, AC_NODE_ROOT, 0, AC_VERB_SET_CODEC_RESET, 0);

		cnxthwhda_probe = (void*)symbol_request(cnxthwhda_probe);
		if(!cnxthwhda_probe) {
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_probe) failed\n", __FUNCTION__);
			if(!codec->afg)
				return -ENOENT;
		} else {
			ret = cnxthwhda_probe(codec, codec->bus->card->dev, &spec->modem_devnode, conexant_hsf_interface_funcs);
			if(ret) {
				printk(KERN_ERR"%s: cnxthwhda_probe() failed: %d\n", __FUNCTION__, ret);
				symbol_put(cnxthwhda_probe);
				if(!codec->afg)
					return ret;
			}
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

	if(codec->mfg && spec && spec->modem_devnode) {
		void (*cnxthwhda_remove)(void *ptr);

		cnxthwhda_remove = (void*)symbol_request(cnxthwhda_remove);
		if(cnxthwhda_remove) {
			cnxthwhda_remove(spec->modem_devnode);
			spec->modem_devnode = NULL;
			symbol_put(cnxthwhda_remove);
			symbol_put(cnxthwhda_probe);
		} else {
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_remove) failed\n", __FUNCTION__);
		}
	}

	return 0;
}

static void conexant_free(struct hda_codec *codec)
{
        struct conexant_spec *spec = codec->spec;
        unsigned int i;

//printk(KERN_DEBUG"%s: codec=%p spec=%p\n", __FUNCTION__, codec, spec);
		if(spec) {
			codec->spec = NULL;
			memset(&codec->patch_ops, 0, sizeof(codec->patch_ops));

        	if (spec->kctl_alloc) {
                for (i = 0; i < spec->num_kctl_used; i++)
                        kfree(spec->kctl_alloc[i].name);
                kfree(spec->kctl_alloc);
        	}
			kfree(spec);
		}
}

static void conexant_unsol_event(struct hda_codec *codec, unsigned int res)
{
	struct conexant_spec *spec = codec->spec;

	//printk(KERN_DEBUG"%s: codec=%p res=0x%02x spec=%p cbHdaEvent=%p\n", __FUNCTION__, codec, res, spec, spec->modem_cbHdaEvent);

	if(codec->mfg && spec && spec->modem_cbHdaEvent) {
		if(((res >> 26) & 0x3f) == spec->modem_cbHdaTag) {
			//printk(KERN_DEBUG"%s: res=0x%02x cbHdaEvent=%p\n", __FUNCTION__, res, spec->modem_cbHdaEvent);
//			printk(KERN_DEBUG"%s: calling cbHdaEvent=%p ctx=%p\n", __FUNCTION__, spec->modem_cbHdaEvent, spec->modem_cbHdaEventContext);
			spec->modem_cbHdaEvent(spec->modem_cbHdaEventContext, res);
		} else {
			printk(KERN_DEBUG"%s: ignoring res=0x08%x\n", __FUNCTION__, res);
		}
	}
}

typedef struct tagHDAOSHAL {
	void *hda_codec;
	int bInSuspendResume;
} HDAOSHAL, *PHDAOSHAL;

typedef struct tagOS_DEVNODE {
    void *hwDev;
    // intentionally left incomplete
} OS_DEVNODE, *POS_DEVNODE;

#ifdef CONFIG_PM
static int conexant_suspend(struct hda_codec *codec, pm_message_t state)
{
	struct conexant_spec *spec = codec->spec;
	int (*cnxthwhda_suspend)(void *devnode, pm_message_t state);
	int ret = 0;

//printk(KERN_DEBUG"%s: codec=%p spec=%p state=0x%x\n", __FUNCTION__, codec, spec, *((u32 *)&state));

	if(spec && spec->modem_devnode) {
		cnxthwhda_suspend = (void*)symbol_request(cnxthwhda_suspend);
		if(!cnxthwhda_suspend) {
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_suspend) failed\n", __FUNCTION__);
			return -ENOSYS;
		}

		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume++;
		}
		ret = cnxthwhda_suspend(spec->modem_devnode, state);
		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume--;
		}
		symbol_put(cnxthwhda_suspend);
	}

	return ret;
}

static int conexant_resume(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	int i;
	int ret = 0;
	hda_nid_t mfg;

	mfg = codec->mfg;
	codec->mfg = 0;
	codec->patch_ops.init(codec);
	codec->mfg = mfg;

	for (i = 0; i < spec->num_mixers; i++)
		snd_hda_resume_ctls(codec, spec->mixers[i]);
	if (spec->multiout.dig_out_nid)
		snd_hda_resume_spdif_out(codec);
	if (spec->dig_in_nid)
		snd_hda_resume_spdif_in(codec);

	if(spec && spec->modem_devnode) {
		int (*cnxthwhda_resume)(void *devnode);

//printk(KERN_DEBUG"%s: codec=%p spec=%p\n", __FUNCTION__, codec, spec);

		cnxthwhda_resume = (void*)symbol_request(cnxthwhda_resume);
		if(!cnxthwhda_resume) {
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_resume) failed\n", __FUNCTION__);
			return -ENOSYS;
		}

    	//snd_hda_codec_write(codec, AC_NODE_ROOT, 0, AC_VERB_SET_CODEC_RESET, 0);

		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume++;
		}

		ret = cnxthwhda_resume(spec->modem_devnode);

		if(((POS_DEVNODE)spec->modem_devnode)->hwDev) {
			((PHDAOSHAL)((POS_DEVNODE)spec->modem_devnode)->hwDev)->bInSuspendResume--;
		}

		symbol_put(cnxthwhda_resume);
	}

	return ret;
}
#endif

static int conexant_build_controls(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	unsigned int i;
	int err;

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

static int patch_cxthsf(struct hda_codec *codec)
{
	struct conexant_spec *spec;
	int (*cnxthwhda_probe)(void *codec, struct device *hwDev, void **ppDevNode);

//printk(KERN_DEBUG"%s: codec=%p\n", __FUNCTION__, codec);
	if(!codec->mfg) { // we only support modems
		return -ENODEV;
	}

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;
	mutex_init(&spec->amp_mutex);
	codec->spec = spec;

	codec->patch_ops = conexant_patch_ops;

	cnxthwhda_probe = (void*)symbol_request(cnxthwhda_probe);
	if(cnxthwhda_probe) {
		symbol_put(cnxthwhda_probe);
	} else {
		printk(KERN_ERR"%s: symbol_request(cnxthwhda_probe) failed\n", __FUNCTION__);
		codec->spec = NULL;
		memset(&codec->patch_ops, 0, sizeof(codec->patch_ops));
		kfree(spec);
		return -ENOENT;
	}

	return 0;
}

/*
 * EAPD control
 * the private value = nid | (invert << 8)
 */

static int cxt_eapd_info(struct snd_kcontrol *kcontrol,
			      struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
	uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
	return 0;
}

static int cxt_eapd_get(struct snd_kcontrol *kcontrol,
			     struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	int invert = (kcontrol->private_value >> 8) & 1;
	if (invert)
		ucontrol->value.integer.value[0] = !spec->cur_eapd;
	else
		ucontrol->value.integer.value[0] = spec->cur_eapd;
	return 0;

}

static int cxt_eapd_put(struct snd_kcontrol *kcontrol,
			     struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	int invert = (kcontrol->private_value >> 8) & 1;
	hda_nid_t nid = kcontrol->private_value & 0xff;
	unsigned int eapd;

	eapd = ucontrol->value.integer.value[0];
	if (invert)
		eapd = !eapd;
	if (eapd == spec->cur_eapd && !codec->in_resume)
		return 0;
	
	spec->cur_eapd = eapd;
#if 0
	if ((nid == 0x11) || (nid == 0x10)) {
		snd_hda_codec_write(codec, 0x10,
				0, AC_VERB_SET_EAPD_BTLENABLE,
				eapd ? 0x02 : 0x00);
	}
#else
	snd_hda_codec_write(codec, nid,
			    0, AC_VERB_SET_EAPD_BTLENABLE,
			    eapd ? 0x02 : 0x00);
#endif
	return 1;
}

/* controls for test mode */
#ifdef CONFIG_SND_DEBUG

#define CXT_EAPD_SWITCH(xname, nid, mask) \
	{ .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, .index = 0,  \
	  .info = cxt_eapd_info, \
	  .get = cxt_eapd_get, \
	  .put = cxt_eapd_put, \
	  .private_value = nid | (mask<<16) }



static int conexant_ch_mode_info(struct snd_kcontrol *kcontrol,
				 struct snd_ctl_elem_info *uinfo)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	return snd_hda_ch_mode_info(codec, uinfo, spec->channel_mode,
				    spec->num_channel_mode);
}

static int conexant_ch_mode_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	return snd_hda_ch_mode_get(codec, ucontrol, spec->channel_mode,
				   spec->num_channel_mode,
				   spec->multiout.max_channels);
}

static int conexant_ch_mode_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	int err = snd_hda_ch_mode_put(codec, ucontrol, spec->channel_mode,
				      spec->num_channel_mode,
				      &spec->multiout.max_channels);
	if (err >= 0 && spec->need_dac_fix)
		spec->multiout.num_dacs = spec->multiout.max_channels / 2;
	return err;
}

#define CXT_PIN_MODE(xname, nid, dir) \
	{ .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, .index = 0,  \
	  .info = conexant_ch_mode_info, \
	  .get = conexant_ch_mode_get, \
	  .put = conexant_ch_mode_put, \
	  .private_value = nid | (dir<<16) }

#endif /* CONFIG_SND_DEBUG */

/* Conexant 5051 specific */
static hda_nid_t cxt5051_dac_nids[1] = { 0x10 };
static hda_nid_t cxt5051_adc_nids[2] = { 0x14,0x15 };
static hda_nid_t cxt5051_capsrc_nids[2] = { 0x14,0x15 };
#define CXT5051_SPDIF_OUT	0x1C
#define CXT5051_PORTB_EVENT	0x38
#define CXT5051_PORTC_EVENT	0x39

static struct hda_channel_mode cxt5051_modes[1] = {
	{ 2, NULL },
};

static struct hda_input_mux cxt5051_capture_source = {
	.num_items = 2,
	.items = {
		{ "IntMic", 0x1 },
		{ "LineIn", 0x2 },
	}
};

/* turn on/off EAPD (+ mute HP) as a master switch */
static int cxt5051_hp_master_sw_put(struct snd_kcontrol *kcontrol,
				    struct snd_ctl_elem_value *ucontrol)
{
	static struct hda_verb spk_on[] = {
		{0x1A, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
		{}
	};
	static struct hda_verb spk_off[] = {
		{0x1A, AC_VERB_SET_PIN_WIDGET_CONTROL, 0},
		{}
	};
	static struct hda_verb hp_on[] = {
		{0x16, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
		{}
	};
	static struct hda_verb hp_off[] = {
		{0x16, AC_VERB_SET_PIN_WIDGET_CONTROL, 0},
		{}
	};
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	if (!cxt_eapd_put(kcontrol, ucontrol))
		return 0;

	/* toggle internal speakers mute depending of presence of
	 * the headphone jack
	 */
	bits = (!spec->hp_present && spec->cur_eapd) ? 0 : 0x80;
	if (bits)
		snd_hda_sequence_write(codec, spk_off);
	else
		snd_hda_sequence_write(codec, spk_on);

	bits = (spec->cur_eapd) ? 0 : 0x80;
	if (bits)
		snd_hda_sequence_write(codec, hp_off);
	else
		snd_hda_sequence_write(codec, hp_on);

	return 1;
}

/* bind volumes of both NID 0x10 and 0x11 */
static int cxt5051_hp_master_vol_put(struct snd_kcontrol *kcontrol,
				     struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	long *valp = ucontrol->value.integer.value;
	int change;

	change = snd_hda_codec_amp_update(codec, 0x10, 0, HDA_OUTPUT, 0,
					  0x7f, valp[0] & 0x7f);
	change |= snd_hda_codec_amp_update(codec, 0x10, 1, HDA_OUTPUT, 0,
					   0x7f, valp[1] & 0x7f);
	return change;
}

/* toggle input of built-in and mic jack appropriately */
static void cxt5051_portb_automic(struct hda_codec *codec)
{
	static struct hda_verb mic_jack_on[] = {
		{0x14, AC_VERB_SET_CONNECT_SEL,0x1},
		{}
	};
	static struct hda_verb mic_jack_off[] = {
		{0x14, AC_VERB_SET_CONNECT_SEL,0x1},
		{}
	};
	unsigned int present;
	struct conexant_spec *spec = codec->spec;

	present = snd_hda_codec_read(codec, 0x17, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;
	printk(KERN_DEBUG"cxt5051_portb_automic present =%x\n",present);

	if (present)
	{
		spec->portb_present = 1;
		snd_hda_sequence_write(codec, mic_jack_on);
	}
	else
	{
		spec->portb_present = 0;
		snd_hda_sequence_write(codec, mic_jack_off);
	}

}

static void cxt5051_portc_automic(struct hda_codec *codec)
{
	unsigned int present;
	struct conexant_spec *spec = codec->spec;
	present = snd_hda_codec_read(codec, 0x18, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;
	printk(KERN_DEBUG"cxt5051_portc_automic present =%x\n",present);

	if (present)
			spec->portc_present = 1;
	else
			spec->portc_present = 0;

}

/* mute internal speaker if HP is plugged */
static void cxt5051_hp_automute(struct hda_codec *codec)
{
	static struct hda_verb spk_on[] = {
		{0x1A, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
		{}
	};
	static struct hda_verb spk_off[] = {
		{0x1A, AC_VERB_SET_PIN_WIDGET_CONTROL, 0},
		{}
	};

	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	spec->hp_present = snd_hda_codec_read(codec, 0x16, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;


	bits = (spec->hp_present || !spec->cur_eapd) ? 0x80 : 0; 
	if (bits)
		snd_hda_sequence_write(codec, spk_off);
	else
		snd_hda_sequence_write(codec, spk_on);

	printk(KERN_DEBUG"cxt5051_hp_automute bits =%x\n",bits);
}

/* unsolicited event for HP jack sensing */
static void cxt5051_hp_unsol_event(struct hda_codec *codec,
				   unsigned int res)
{
	switch (res >> 26) {
	case CONEXANT_HP_EVENT:
		cxt5051_hp_automute(codec);
		break;
	case CXT5051_PORTB_EVENT:
		cxt5051_portb_automic(codec);
		break;
	case CXT5051_PORTC_EVENT:
		cxt5051_portc_automic(codec);
		break;

	default:
		conexant_unsol_event(codec, res);
		break;

	}
}

static struct snd_kcontrol_new cxt5051_mixers[] = {
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Capture Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put
	},
//	HDA_CODEC_VOLUME("Int Mic Volume", 0x14, 0x00, HDA_INPUT),
//	HDA_CODEC_MUTE("Int Mic Switch", 0x14, 0x00, HDA_INPUT),
	HDA_CODEC_VOLUME("Internal Mic Volume", 0x14, 0x00, HDA_INPUT),
	HDA_CODEC_MUTE("Internal Mic Switch", 0x14, 0x00, HDA_INPUT),
	HDA_CODEC_VOLUME("External Mic Volume", 0x14, 0x01, HDA_INPUT),
	HDA_CODEC_MUTE("External Mic Switch", 0x14, 0x01, HDA_INPUT),
	HDA_CODEC_VOLUME("Docking Mic Volume", 0x15, 0x00, HDA_INPUT),
	HDA_CODEC_MUTE("Docking Mic Switch", 0x15, 0x00, HDA_INPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Volume",
		.info = snd_hda_mixer_amp_volume_info,
		.get = snd_hda_mixer_amp_volume_get,
		.put = cxt5051_hp_master_vol_put,
		.private_value = HDA_COMPOSE_AMP_VAL(0x10, 3, 0, HDA_OUTPUT),
	},
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Switch",
		.info = cxt_eapd_info,
		.get = cxt_eapd_get,
		.put = cxt5051_hp_master_sw_put,
		.private_value = 0x1A,
	},

	{}
};

static struct hda_verb cxt5051_init_verbs[] = {
	/* Line in, Mic */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE,AC_AMP_SET_INPUT|AC_AMP_SET_LEFT
	|AC_AMP_SET_RIGHT|0x3},
	{0x17, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_80 },
	{0x18, AC_VERB_SET_AMP_GAIN_MUTE,AC_AMP_SET_INPUT|AC_AMP_SET_LEFT
	|AC_AMP_SET_RIGHT|0x3},
	{0x18, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_80 },
	{0x1D, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN},
	{0x1D, AC_VERB_SET_AMP_GAIN_MUTE,AC_AMP_SET_INPUT|AC_AMP_SET_LEFT
	|AC_AMP_SET_RIGHT|0x3},
	/* SPK  */
	{0x1A, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT },
	{0x1A, AC_VERB_SET_CONNECT_SEL,0x00},
	/* HP, Amp  */
	{0x16, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_HP|PIN_OUT },
	{0x16, AC_VERB_SET_CONNECT_SEL,0x00},
	/* DAC1 */	
	{0x10, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x01},
	{0x10, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x02},
	{0x10, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x03},
	{0x10, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x04},
	/* Record selector: Int mic */
	{0x14, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x44},
	{0x14, AC_VERB_SET_AMP_GAIN_MUTE,AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|
	AC_AMP_SET_LEFT|0x44|1<<AC_AMP_SET_INDEX_SHIFT},
	{0x15, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x44},
	{0x15, AC_VERB_SET_AMP_GAIN_MUTE,AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|
	AC_AMP_SET_LEFT|0x44|1<<AC_AMP_SET_INDEX_SHIFT},
	/* SPDIF route: PCM */
	{ 0x1C, AC_VERB_SET_CONNECT_SEL, 0x0 },
	/* EAPD */
	{0x1A, AC_VERB_SET_EAPD_BTLENABLE, 0x2 }, /* default on */ 
	{0x16, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_HP_EVENT},
	{0x17, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CXT5051_PORTB_EVENT},
	{0x18, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CXT5051_PORTC_EVENT},
	{ } /* end */
};


#if 0
static struct hda_verb cxt5051_mic_wid1d_init_verbs[] = {
	/* pin sensing on HP jack */
	{0x14, AC_VERB_SET_CONNECT_SEL,0x0},
	{ } /* end */
};
#endif

/* For HP's configuration always use portB as Internal and External Mic */
static struct hda_verb cxt5051_mic_wid17_init_verbs[] = {
	/* pin sensing on HP jack */
	{0x14, AC_VERB_SET_CONNECT_SEL,0x1},
	{ } /* end */
};


/* initialize jack-sensing, too */
static int cxt5051_init(struct hda_codec *codec)
{
	conexant_init(codec);
	return 0;
}


enum {
	CXT5051_LAPTOP,	 /* Laptops w/ EAPD support */
	CXT5051_MODELS
};

static const char *cxt5051_models[CXT5051_MODELS] = {
	[CXT5051_LAPTOP]	= "laptop",
};

static struct snd_pci_quirk cxt5051_cfg_tbl[] = {
	SND_PCI_QUIRK(0x14F1, 0x0101, "Conexant Reference board", CXT5051_LAPTOP),
	{}
};

static int patch_cxt5051(struct hda_codec *codec)
{
	struct conexant_spec *spec;
	int board_config;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;
	mutex_init(&spec->amp_mutex);
	codec->spec = spec;

	spec->multiout.max_channels = 2;
	spec->multiout.num_dacs = ARRAY_SIZE(cxt5051_dac_nids);
	spec->multiout.dac_nids = cxt5051_dac_nids;
	spec->multiout.dig_out_nid = CXT5051_SPDIF_OUT;
	spec->num_adc_nids = 2;
	spec->adc_nids = cxt5051_adc_nids;
	spec->capsrc_nids = cxt5051_capsrc_nids;
	spec->input_mux = &cxt5051_capture_source;
	spec->num_mixers = 1;
	spec->mixers[0] = cxt5051_mixers;
	spec->num_init_verbs = 1;
	spec->init_verbs[0] = cxt5051_init_verbs;
	spec->spdif_route = 0;
	spec->num_channel_mode = ARRAY_SIZE(cxt5051_modes),
	spec->channel_mode = cxt5051_modes,
	spec->portb_present = 0;
	spec->portc_present = 0;

	codec->patch_ops = conexant_patch_ops;

	board_config = snd_hda_check_board_config(codec, CXT5051_MODELS,
						  cxt5051_models,
						  cxt5051_cfg_tbl);
	switch (board_config) {
	default:
	case CXT5051_LAPTOP:
		codec->patch_ops.unsol_event = cxt5051_hp_unsol_event;
		spec->input_mux = &cxt5051_capture_source;
		spec->num_init_verbs = 2;
		spec->init_verbs[1] = cxt5051_mic_wid17_init_verbs;
		spec->mixers[0] = cxt5051_mixers;
		codec->patch_ops.init = cxt5051_init;
		break;
	}

	if(codec->mfg) {
		int (*cnxthwhda_probe)(void *codec, struct device *hwDev, void **ppDevNode);
		cnxthwhda_probe = (void*)symbol_request(cnxthwhda_probe);
		if(cnxthwhda_probe)
			symbol_put(cnxthwhda_probe);
		else
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_probe) failed\n", __FUNCTION__);
	}

	return 0;
}




/* Conexant 5045 specific */

static hda_nid_t cxt5045_dac_nids[1] = { 0x19 };
static hda_nid_t cxt5045_adc_nids[1] = { 0x1a };
static hda_nid_t cxt5045_capsrc_nids[1] = { 0x1a };
#define CXT5045_SPDIF_OUT	0x13

static struct hda_channel_mode cxt5045_modes[1] = {
	{ 2, NULL },
};

static struct hda_input_mux cxt5045_capture_source = {
	.num_items = 2,
	.items = {
		{ "IntMic", 0x1 },
		{ "ExtMic", 0x2 },
	}
};

/* turn on/off EAPD (+ mute HP) as a master switch */
static int cxt5045_hp_master_sw_put(struct snd_kcontrol *kcontrol,
				    struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	if (!cxt_eapd_put(kcontrol, ucontrol))
		return 0;

	/* toggle internal speakers mute depending of presence of
	 * the headphone jack
	 */
	bits = (!spec->hp_present && spec->cur_eapd) ? 0 : 0x80;
	snd_hda_codec_amp_update(codec, 0x10, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x10, 1, HDA_OUTPUT, 0, 0x80, bits);

	bits = spec->cur_eapd ? 0 : 0x80;
	snd_hda_codec_amp_update(codec, 0x11, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x11, 1, HDA_OUTPUT, 0, 0x80, bits);
	return 1;
}

/* bind volumes of both NID 0x10 and 0x11 */
static int cxt5045_hp_master_vol_put(struct snd_kcontrol *kcontrol,
				     struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	long *valp = ucontrol->value.integer.value;
	int change;

	change = snd_hda_codec_amp_update(codec, 0x10, 0, HDA_OUTPUT, 0,
					  0x7f, valp[0] & 0x7f);
	change |= snd_hda_codec_amp_update(codec, 0x10, 1, HDA_OUTPUT, 0,
					   0x7f, valp[1] & 0x7f);
	snd_hda_codec_amp_update(codec, 0x11, 0, HDA_OUTPUT, 0,
				 0x7f, valp[0] & 0x7f);
	snd_hda_codec_amp_update(codec, 0x11, 1, HDA_OUTPUT, 0,
				 0x7f, valp[1] & 0x7f);
	return change;
}

/* toggle input of built-in and mic jack appropriately */
static void cxt5045_hp_automic(struct hda_codec *codec)
{
	static struct hda_verb mic_jack_on[] = {
		{0x14, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT | AC_AMP_MUTE},
		{0x12, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT},
		{0x1a, AC_VERB_SET_CONNECT_SEL, 0x02},
		{}
	};
	static struct hda_verb mic_jack_off[] = {
		{0x12, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT | AC_AMP_MUTE},
		{0x14, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT},
		{0x1a, AC_VERB_SET_CONNECT_SEL, 0x01},
		{}
	};
	unsigned int present;

	present = snd_hda_codec_read(codec, 0x12, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;
//printk("%s: mic_present=%x\n", __FUNCTION__, present);
	if (present)
		snd_hda_sequence_write(codec, mic_jack_on);
	else
		snd_hda_sequence_write(codec, mic_jack_off);
}


/* mute internal speaker if HP is plugged */
static void cxt5045_hp_automute(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	spec->hp_present = snd_hda_codec_read(codec, 0x11, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;
//printk("%s: hp_present=%x\n", __FUNCTION__, spec->hp_present);

	bits = (spec->hp_present || !spec->cur_eapd) ? 0x80 : 0; 
	snd_hda_codec_amp_update(codec, 0x10, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x10, 1, HDA_OUTPUT, 0, 0x80, bits);
}

/* unsolicited event for HP jack sensing */
static void cxt5045_hp_unsol_event(struct hda_codec *codec,
				   unsigned int res)
{
	switch (res >> 26) {
	case CONEXANT_HP_EVENT:
//printk("%s: CONEXANT_HP_EVENT\n", __FUNCTION__);
		cxt5045_hp_automute(codec);
		break;
	case CONEXANT_MIC_EVENT:
//printk("%s: CONEXANT_MIC_EVENT\n", __FUNCTION__);
		cxt5045_hp_automic(codec);
		break;
	default:
//printk("%s: CONEXANT_OTHER_EVENT\n", __FUNCTION__);
		conexant_unsol_event(codec, res);
		break;

	}
}

static struct snd_kcontrol_new cxt5045_mixers[] = {
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Capture Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put
	},
	HDA_CODEC_VOLUME("Int Mic Volume", 0x1a, 0x01, HDA_INPUT),
	HDA_CODEC_MUTE("Int Mic Switch", 0x1a, 0x01, HDA_INPUT),
	HDA_CODEC_VOLUME("Ext Mic Volume", 0x1a, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("Ext Mic Switch", 0x1a, 0x02, HDA_INPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Volume",
		.info = snd_hda_mixer_amp_volume_info,
		.get = snd_hda_mixer_amp_volume_get,
		.put = cxt5045_hp_master_vol_put,
		.private_value = HDA_COMPOSE_AMP_VAL(0x10, 3, 0, HDA_OUTPUT),
	},
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Switch",
		.info = cxt_eapd_info,
		.get = cxt_eapd_get,
		.put = cxt5045_hp_master_sw_put,
		.private_value = 0x10,
	},

	{}
};

static struct hda_verb cxt5045_init_verbs[] = {
	/* ExtMic in, IntMic */
	{0x12, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_80 },
	{0x12, AC_VERB_SET_CONNECT_SEL, 0x1 },
	{0x14, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_80 },
	/* HP, Amp  */
	{0x11, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_HP },
	{0x17, AC_VERB_SET_CONNECT_SEL,0x01},
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x01},
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x02},
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x03},
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x04},
	/* Record selector: Int mic */
	{0x1a, AC_VERB_SET_CONNECT_SEL,0x1},
	{0x1a, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x17},
	/* SPDIF route: PCM */
	{ 0x13, AC_VERB_SET_CONNECT_SEL, 0x0 },
	/* EAPD */
	{0x10, AC_VERB_SET_EAPD_BTLENABLE, 0x2 }, /* default on */ 
	{ } /* end */
};


static struct hda_verb cxt5045_hp_sense_init_verbs[] = {
	/* pin sensing on HP jack */
	{0x11, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_HP_EVENT},
	{ } /* end */
};

static struct hda_verb cxt5045_mic_sense_init_verbs[] = {
	/* pin sensing on HP jack */
	{0x12, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_MIC_EVENT},
	{ } /* end */
};

#ifdef CONFIG_SND_DEBUG
/* Test configuration for debugging, modelled after the ALC260 test
 * configuration.
 */
static struct hda_input_mux cxt5045_test_capture_source = {
	.num_items = 5,
	.items = {
		{ "MIXER", 0x0 },
		{ "MIC1 pin", 0x1 },
		{ "LINE1 pin", 0x2 },
		{ "HP-OUT pin", 0x3 },
		{ "CD pin", 0x4 },
        },
};

static struct snd_kcontrol_new cxt5045_test_mixer[] = {

	HDA_CODEC_VOLUME("Int Mic Volume", 0x1a, 0x01, HDA_INPUT),
	HDA_CODEC_MUTE("Int Mic Switch", 0x1a, 0x01, HDA_INPUT),
	HDA_CODEC_VOLUME("Ext Mic Volume", 0x1a, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("Ext Mic Switch", 0x1a, 0x02, HDA_INPUT),

	/* Output controls */
	HDA_CODEC_VOLUME("Speaker Playback Volume", 0x10, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Speaker Playback Switch", 0x10, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Node 11 Playback Volume", 0x11, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Node 11 Playback Switch", 0x11, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Node 12 Playback Volume", 0x12, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Node 12 Playback Switch", 0x12, 0x0, HDA_OUTPUT),
	
	/* Modes for retasking pin widgets */
	CXT_PIN_MODE("HP-OUT pin mode", 0x11, CXT_PIN_DIR_INOUT),
	CXT_PIN_MODE("LINE1 pin mode", 0x12, CXT_PIN_DIR_INOUT),

	/* EAPD Switch Control */
	CXT_EAPD_SWITCH("External Amplifier", 0x10, 0x0),

	/* Loopback mixer controls */

	HDA_CODEC_VOLUME("Mixer-1 Volume", 0x17, 0x0, HDA_INPUT),
	HDA_CODEC_MUTE("Mixer-1 Switch", 0x17, 0x0, HDA_INPUT),
	HDA_CODEC_VOLUME("Mixer-2 Volume", 0x17, 0x1, HDA_INPUT),
	HDA_CODEC_MUTE("Mixer-2 Switch", 0x17, 0x1, HDA_INPUT),
	HDA_CODEC_VOLUME("Mixer-3 Volume", 0x17, 0x2, HDA_INPUT),
	HDA_CODEC_MUTE("Mixer-3 Switch", 0x17, 0x2, HDA_INPUT),
	HDA_CODEC_VOLUME("Mixer-4 Volume", 0x17, 0x3, HDA_INPUT),
	HDA_CODEC_MUTE("Mixer-4 Switch", 0x17, 0x3, HDA_INPUT),
	HDA_CODEC_VOLUME("Mixer-5 Volume", 0x17, 0x4, HDA_INPUT),
	HDA_CODEC_MUTE("Mixer-5 Switch", 0x17, 0x4, HDA_INPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Input Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put,
	},
	{ } /* end */
};

static struct hda_verb cxt5045_test_init_verbs[] = {
	/* Set connections */
	{ 0x10, AC_VERB_SET_CONNECT_SEL, 0x0 },
	{ 0x11, AC_VERB_SET_CONNECT_SEL, 0x0 },
	{ 0x12, AC_VERB_SET_CONNECT_SEL, 0x0 },
	/* Enable retasking pins as output, initially without power amp */
	{0x12, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
	{0x11, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},

	/* Disable digital (SPDIF) pins initially, but users can enable
	 * them via a mixer switch.  In the case of SPDIF-out, this initverb
	 * payload also sets the generation to 0, output to be in "consumer"
	 * PCM format, copyright asserted, no pre-emphasis and no validity
	 * control.
	 */
	{0x13, AC_VERB_SET_DIGI_CONVERT_1, 0},

	/* Start with output sum widgets muted and their output gains at min */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)},
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)},

	/* Unmute retasking pin widget output buffers since the default
	 * state appears to be output.  As the pin mode is changed by the
	 * user the pin mode control will take care of enabling the pin's
	 * input/output buffers as needed.
	 */
	{0x12, AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE},
	{0x11, AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE},

	/* Mute capture amp left and right */
	{0x1a, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)},

	/* Set ADC connection select to match default mixer setting (mic1
	 * pin)
	 */
	{0x1a, AC_VERB_SET_CONNECT_SEL, 0x00},
	{0x17, AC_VERB_SET_CONNECT_SEL, 0x00},

	/* Mute all inputs to mixer widget (even unconnected ones) */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)}, /* Mixer pin */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)}, /* Mic1 pin */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(2)}, /* Line pin */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(3)}, /* HP pin */
	{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(4)}, /* CD pin */

	{ }
};
#endif


/* initialize jack-sensing, too */
static int cxt5045_init(struct hda_codec *codec)
{
	conexant_init(codec);
	cxt5045_hp_automute(codec);
	return 0;
}


enum {
	CXT5045_LAPTOP_HPSENSE,
	CXT5045_LAPTOP_MICSENSE,
	CXT5045_LAPTOP_HPMICSENSE,
#ifdef CONFIG_SND_DEBUG
	CXT5045_TEST,
#endif
	CXT5045_MODELS
};

static const char *cxt5045_models[CXT5045_MODELS] = {
	[CXT5045_LAPTOP_HPSENSE]	= "laptop-hpsense",
	[CXT5045_LAPTOP_MICSENSE]	= "laptop-micsense",
	[CXT5045_LAPTOP_HPMICSENSE]	= "laptop-hpmicsense",
#ifdef CONFIG_SND_DEBUG
	[CXT5045_TEST]		= "test",
#endif
};

static struct snd_pci_quirk cxt5045_cfg_tbl[] = {
	SND_PCI_QUIRK(0x103c, 0x30a5, "HP", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30b7, "HP DV6000Z", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30bb, "HP DV8000", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30b2, "HP DV Series", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30b5, "HP DV2120", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30cd, "HP DV Series", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30d5, "HP", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x103c, 0x30d9, "HP Spartan", CXT5045_LAPTOP_HPSENSE),
	SND_PCI_QUIRK(0x1734, 0x10ad, "Fujitsu Si1520", CXT5045_LAPTOP_MICSENSE),
	SND_PCI_QUIRK(0x1734, 0x10cb, "Fujitsu Si3515", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x1509, 0x1e40, "FIC", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x1509, 0x2f05, "FIC", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x1509, 0x2f06, "FIC", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x1631, 0xc106, "Packard Bell", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x1631, 0xc107, "Packard Bell", CXT5045_LAPTOP_HPMICSENSE),
	SND_PCI_QUIRK(0x8086, 0x2111, "Conexant Reference board", CXT5045_LAPTOP_HPSENSE),
	{}
};

static int patch_cxt5045(struct hda_codec *codec)
{
	struct conexant_spec *spec;
	int board_config;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;
	mutex_init(&spec->amp_mutex);
	codec->spec = spec;

	spec->multiout.max_channels = 2;
	spec->multiout.num_dacs = ARRAY_SIZE(cxt5045_dac_nids);
	spec->multiout.dac_nids = cxt5045_dac_nids;
	spec->multiout.dig_out_nid = CXT5045_SPDIF_OUT;
	spec->num_adc_nids = 1;
	spec->adc_nids = cxt5045_adc_nids;
	spec->capsrc_nids = cxt5045_capsrc_nids;
	spec->input_mux = &cxt5045_capture_source;
	spec->num_mixers = 1;
	spec->mixers[0] = cxt5045_mixers;
	spec->num_init_verbs = 1;
	spec->init_verbs[0] = cxt5045_init_verbs;
	spec->spdif_route = 0;
	spec->num_channel_mode = ARRAY_SIZE(cxt5045_modes),
	spec->channel_mode = cxt5045_modes,


	codec->patch_ops = conexant_patch_ops;

	board_config = snd_hda_check_board_config(codec, CXT5045_MODELS,
						  cxt5045_models,
						  cxt5045_cfg_tbl);
//printk(KERN_DEBUG"%s: board_config=%d\n", __FUNCTION__, board_config);
	switch (board_config) {
	case CXT5045_LAPTOP_HPSENSE:
		codec->patch_ops.unsol_event = cxt5045_hp_unsol_event;
		spec->input_mux = &cxt5045_capture_source;
		spec->num_init_verbs = 2;
		spec->init_verbs[1] = cxt5045_hp_sense_init_verbs;
		spec->mixers[0] = cxt5045_mixers;
		codec->patch_ops.init = cxt5045_init;
		break;
	case CXT5045_LAPTOP_MICSENSE:
		codec->patch_ops.unsol_event = cxt5045_hp_unsol_event;
		spec->input_mux = &cxt5045_capture_source;
		spec->num_init_verbs = 2;
		spec->init_verbs[1] = cxt5045_mic_sense_init_verbs;
		spec->mixers[0] = cxt5045_mixers;
		codec->patch_ops.init = cxt5045_init;
		break;
	default:
	case CXT5045_LAPTOP_HPMICSENSE:
		codec->patch_ops.unsol_event = cxt5045_hp_unsol_event;
		spec->input_mux = &cxt5045_capture_source;
		spec->num_init_verbs = 3;
		spec->init_verbs[1] = cxt5045_hp_sense_init_verbs;
		spec->init_verbs[2] = cxt5045_mic_sense_init_verbs;
		spec->mixers[0] = cxt5045_mixers;
		codec->patch_ops.init = cxt5045_init;
		break;
#ifdef CONFIG_SND_DEBUG
	case CXT5045_TEST:
		spec->input_mux = &cxt5045_test_capture_source;
		spec->mixers[0] = cxt5045_test_mixer;
		spec->init_verbs[0] = cxt5045_test_init_verbs;
#endif	
		break;
		
	}

	if(codec->mfg) {
		int (*cnxthwhda_probe)(void *codec, struct device *hwDev, void **ppDevNode);
		cnxthwhda_probe = (void*)symbol_request(cnxthwhda_probe);
		if(cnxthwhda_probe)
			symbol_put(cnxthwhda_probe);
		else
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_probe) failed\n", __FUNCTION__);
	}

	return 0;
}


/* Conexant 5047 specific */
#define CXT5047_SPDIF_OUT	0x11

static hda_nid_t cxt5047_dac_nids[2] = { 0x10, 0x1c };
static hda_nid_t cxt5047_adc_nids[1] = { 0x12 };
static hda_nid_t cxt5047_capsrc_nids[1] = { 0x1a };

static struct hda_channel_mode cxt5047_modes[1] = {
	{ 2, NULL },
};

static struct hda_input_mux cxt5047_capture_source = {
	.num_items = 1,
	.items = {
		{ "Mic", 0x2 },
	}
};

static struct hda_input_mux cxt5047_hp_capture_source = {
	.num_items = 1,
	.items = {
		{ "ExtMic", 0x2 },
	}
};

static struct hda_input_mux cxt5047_toshiba_capture_source = {
	.num_items = 2,
	.items = {
		{ "ExtMic", 0x2 },
		{ "Line-In", 0x1 },
	}
};

/* turn on/off EAPD (+ mute HP) as a master switch */
static int cxt5047_hp_master_sw_put(struct snd_kcontrol *kcontrol,
				    struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	if (!cxt_eapd_put(kcontrol, ucontrol))
		return 0;

	/* toggle internal speakers mute depending of presence of
	 * the headphone jack
	 */
	bits = (!spec->hp_present && spec->cur_eapd) ? 0 : 0x80;
	snd_hda_codec_amp_update(codec, 0x1d, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x1d, 1, HDA_OUTPUT, 0, 0x80, bits);
	bits = spec->cur_eapd ? 0 : 0x80;
	snd_hda_codec_amp_update(codec, 0x13, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x13, 1, HDA_OUTPUT, 0, 0x80, bits);
	return 1;
}

/* bind volumes of both NID 0x13 (Headphones) and 0x1d (Speakers) */
static int cxt5047_hp_master_vol_put(struct snd_kcontrol *kcontrol,
				     struct snd_ctl_elem_value *ucontrol)
{
	struct hda_codec *codec = snd_kcontrol_chip(kcontrol);
	long *valp = ucontrol->value.integer.value;
	int change;

	change = snd_hda_codec_amp_update(codec, 0x1d, 0, HDA_OUTPUT, 0,
					  0x7f, valp[0] & 0x7f);
	change |= snd_hda_codec_amp_update(codec, 0x1d, 1, HDA_OUTPUT, 0,
					   0x7f, valp[1] & 0x7f);
	snd_hda_codec_amp_update(codec, 0x13, 0, HDA_OUTPUT, 0,
				 0x7f, valp[0] & 0x7f);
	snd_hda_codec_amp_update(codec, 0x13, 1, HDA_OUTPUT, 0,
				 0x7f, valp[1] & 0x7f);
	return change;
}

/* mute internal speaker if HP is plugged */
static void cxt5047_hp_automute(struct hda_codec *codec)
{
	struct conexant_spec *spec = codec->spec;
	unsigned int bits;

	spec->hp_present = snd_hda_codec_read(codec, 0x13, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;

	bits = (spec->hp_present || !spec->cur_eapd) ? 0x80 : 0;
	snd_hda_codec_amp_update(codec, 0x1d, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x1d, 1, HDA_OUTPUT, 0, 0x80, bits);
	/* Mute/Unmute PCM 2 for good measure - some systems need this */
	snd_hda_codec_amp_update(codec, 0x1c, 0, HDA_OUTPUT, 0, 0x80, bits);
	snd_hda_codec_amp_update(codec, 0x1c, 1, HDA_OUTPUT, 0, 0x80, bits);
}

/* toggle input of built-in and mic jack appropriately */
static void cxt5047_hp_automic(struct hda_codec *codec)
{
	static struct hda_verb mic_jack_on[] = {
		{0x15, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT | AC_AMP_MUTE},
		{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT},
		{}
	};
	static struct hda_verb mic_jack_off[] = {
		{0x17, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT | AC_AMP_MUTE},
		{0x15, AC_VERB_SET_AMP_GAIN_MUTE, AC_AMP_SET_RIGHT | AC_AMP_SET_LEFT | AC_AMP_SET_OUTPUT},
		{}
	};
	unsigned int present;

	present = snd_hda_codec_read(codec, 0x15, 0,
				     AC_VERB_GET_PIN_SENSE, 0) & 0x80000000;
	if (present)
		snd_hda_sequence_write(codec, mic_jack_on);
	else
		snd_hda_sequence_write(codec, mic_jack_off);
}

/* unsolicited event for HP jack sensing */
static void cxt5047_hp_unsol_event(struct hda_codec *codec,
				  unsigned int res)
{
	switch (res >> 26) {
	case CONEXANT_HP_EVENT:
		cxt5047_hp_automute(codec);
		break;
	case CONEXANT_MIC_EVENT:
		cxt5047_hp_automic(codec);
		break;
	default:
		conexant_unsol_event(codec, res);
		break;
	}
}

static struct snd_kcontrol_new cxt5047_mixers[] = {
	HDA_CODEC_VOLUME("Mic Bypass Capture Volume", 0x19, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("Mic Bypass Capture Switch", 0x19, 0x02, HDA_INPUT),
	HDA_CODEC_VOLUME("Mic Gain Volume", 0x1a, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Mic Gain Switch", 0x1a, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Capture Volume", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_MUTE("Capture Switch", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_VOLUME("PCM Volume", 0x10, 0x00, HDA_OUTPUT),
	HDA_CODEC_MUTE("PCM Switch", 0x10, 0x00, HDA_OUTPUT),
	HDA_CODEC_VOLUME("PCM-2 Volume", 0x1c, 0x00, HDA_OUTPUT),
	HDA_CODEC_MUTE("PCM-2 Switch", 0x1c, 0x00, HDA_OUTPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Volume",
		.info = snd_hda_mixer_amp_volume_info,
		.get = snd_hda_mixer_amp_volume_get,
		.put = cxt5047_hp_master_vol_put,
		.private_value = HDA_COMPOSE_AMP_VAL(0x13, 3, 0, HDA_OUTPUT),
	},
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Switch",
		.info = cxt_eapd_info,
		.get = cxt_eapd_get,
		.put = cxt5047_hp_master_sw_put,
		.private_value = 0x13,
	},

	{}
};

static struct snd_kcontrol_new cxt5047_toshiba_mixers[] = {
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Capture Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put
	},
	HDA_CODEC_VOLUME("Mic Bypass Capture Volume", 0x19, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("Mic Bypass Capture Switch", 0x19, 0x02, HDA_INPUT),
	HDA_CODEC_VOLUME("Capture Volume", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_MUTE("Capture Switch", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_VOLUME("PCM Volume", 0x10, 0x00, HDA_OUTPUT),
	HDA_CODEC_MUTE("PCM Switch", 0x10, 0x00, HDA_OUTPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Volume",
		.info = snd_hda_mixer_amp_volume_info,
		.get = snd_hda_mixer_amp_volume_get,
		.put = cxt5047_hp_master_vol_put,
		.private_value = HDA_COMPOSE_AMP_VAL(0x13, 3, 0, HDA_OUTPUT),
	},
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Switch",
		.info = cxt_eapd_info,
		.get = cxt_eapd_get,
		.put = cxt5047_hp_master_sw_put,
		.private_value = 0x13,
	},

	{}
};

static struct snd_kcontrol_new cxt5047_hp_mixers[] = {
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Capture Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put
	},
	HDA_CODEC_VOLUME("Mic Bypass Capture Volume", 0x19, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("Mic Bypass Capture Switch", 0x19,0x02,HDA_INPUT),
	HDA_CODEC_VOLUME("Capture Volume", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_MUTE("Capture Switch", 0x12, 0x03, HDA_INPUT),
	HDA_CODEC_VOLUME("PCM Volume", 0x10, 0x00, HDA_OUTPUT),
	HDA_CODEC_MUTE("PCM Switch", 0x10, 0x00, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Master Playback Volume", 0x13, 0x00, HDA_OUTPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Master Playback Switch",
		.info = cxt_eapd_info,
		.get = cxt_eapd_get,
		.put = cxt5047_hp_master_sw_put,
		.private_value = 0x13,
	},
	{ } /* end */
};

static struct hda_verb cxt5047_init_verbs[] = {
	/* Line in, Mic, Built-in Mic */
	{0x14, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN },
	{0x15, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_50 },
	{0x17, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_IN|AC_PINCTL_VREF_50 },
	/* HP, Speaker  */
	{0x13, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT },
	{0x1d, AC_VERB_SET_CONNECT_SEL,0x0},
	/* Record selector: Mic */
	{0x12, AC_VERB_SET_CONNECT_SEL,0x03},
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x17},
	{0x1A, AC_VERB_SET_CONNECT_SEL,0x02},
	{0x1A, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x00},
	{0x1A, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_OUTPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x03},
	/* SPDIF route: PCM */
	{ 0x18, AC_VERB_SET_CONNECT_SEL, 0x0 },
	/* Enable unsolicited events */
	{0x13, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_HP_EVENT},
	{0x15, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_MIC_EVENT},
	{ } /* end */
};

/* configuration for Toshiba Laptops */
static struct hda_verb cxt5047_toshiba_init_verbs[] = {
	{0x13, AC_VERB_SET_EAPD_BTLENABLE, 0x0 }, /* default on */
	/* pin sensing on HP and Mic jacks */
	{0x13, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_HP_EVENT},
	{0x15, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_MIC_EVENT},
	/* Speaker routing */
	{0x1d, AC_VERB_SET_CONNECT_SEL,0x1},
	{}
};

/* configuration for HP Laptops */
static struct hda_verb cxt5047_hp_init_verbs[] = {
	/* pin sensing on HP jack */
	{0x13, AC_VERB_SET_UNSOLICITED_ENABLE, AC_USRSP_EN | CONEXANT_HP_EVENT},
	/* Record selector: Ext Mic */
	{0x12, AC_VERB_SET_CONNECT_SEL,0x03},
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE,
	 AC_AMP_SET_INPUT|AC_AMP_SET_RIGHT|AC_AMP_SET_LEFT|0x17},
	/* Speaker routing */
	{0x1d, AC_VERB_SET_CONNECT_SEL,0x1},
	{}
};

/* Test configuration for debugging, modelled after the ALC260 test
 * configuration.
 */
#ifdef CONFIG_SND_DEBUG
static struct hda_input_mux cxt5047_test_capture_source = {
	.num_items = 4,
	.items = {
		{ "LINE1 pin", 0x0 },
		{ "MIC1 pin", 0x1 },
		{ "MIC2 pin", 0x2 },
		{ "CD pin", 0x3 },
        },
};

static struct snd_kcontrol_new cxt5047_test_mixer[] = {

	/* Output only controls */
	HDA_CODEC_VOLUME("OutAmp-1 Volume", 0x10, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("OutAmp-1 Switch", 0x10,0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("OutAmp-2 Volume", 0x1c, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("OutAmp-2 Switch", 0x1c, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Speaker Playback Volume", 0x1d, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Speaker Playback Switch", 0x1d, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("HeadPhone Playback Volume", 0x13, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("HeadPhone Playback Switch", 0x13, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Line1-Out Playback Volume", 0x14, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Line1-Out Playback Switch", 0x14, 0x0, HDA_OUTPUT),
	HDA_CODEC_VOLUME("Line2-Out Playback Volume", 0x15, 0x0, HDA_OUTPUT),
	HDA_CODEC_MUTE("Line2-Out Playback Switch", 0x15, 0x0, HDA_OUTPUT),

	/* Modes for retasking pin widgets */
	CXT_PIN_MODE("LINE1 pin mode", 0x14, CXT_PIN_DIR_INOUT),
	CXT_PIN_MODE("MIC1 pin mode", 0x15, CXT_PIN_DIR_INOUT),

	/* EAPD Switch Control */
	CXT_EAPD_SWITCH("External Amplifier", 0x13, 0x0),

	/* Loopback mixer controls */
	HDA_CODEC_VOLUME("MIC1 Playback Volume", 0x12, 0x01, HDA_INPUT),
	HDA_CODEC_MUTE("MIC1 Playback Switch", 0x12, 0x01, HDA_INPUT),
	HDA_CODEC_VOLUME("MIC2 Playback Volume", 0x12, 0x02, HDA_INPUT),
	HDA_CODEC_MUTE("MIC2 Playback Switch", 0x12, 0x02, HDA_INPUT),
	HDA_CODEC_VOLUME("LINE Playback Volume", 0x12, 0x0, HDA_INPUT),
	HDA_CODEC_MUTE("LINE Playback Switch", 0x12, 0x0, HDA_INPUT),
	HDA_CODEC_VOLUME("CD Playback Volume", 0x12, 0x04, HDA_INPUT),
	HDA_CODEC_MUTE("CD Playback Switch", 0x12, 0x04, HDA_INPUT),

	HDA_CODEC_VOLUME("Capture-1 Volume", 0x19, 0x0, HDA_INPUT),
	HDA_CODEC_MUTE("Capture-1 Switch", 0x19, 0x0, HDA_INPUT),
	HDA_CODEC_VOLUME("Capture-2 Volume", 0x19, 0x1, HDA_INPUT),
	HDA_CODEC_MUTE("Capture-2 Switch", 0x19, 0x1, HDA_INPUT),
	HDA_CODEC_VOLUME("Capture-3 Volume", 0x19, 0x2, HDA_INPUT),
	HDA_CODEC_MUTE("Capture-3 Switch", 0x19, 0x2, HDA_INPUT),
	HDA_CODEC_VOLUME("Capture-4 Volume", 0x19, 0x3, HDA_INPUT),
	HDA_CODEC_MUTE("Capture-4 Switch", 0x19, 0x3, HDA_INPUT),
	{
		.iface = SNDRV_CTL_ELEM_IFACE_MIXER,
		.name = "Input Source",
		.info = conexant_mux_enum_info,
		.get = conexant_mux_enum_get,
		.put = conexant_mux_enum_put,
	},
	HDA_CODEC_VOLUME("Input-1 Volume", 0x1a, 0x0, HDA_INPUT),
	HDA_CODEC_MUTE("Input-1 Switch", 0x1a, 0x0, HDA_INPUT),
	HDA_CODEC_VOLUME("Input-2 Volume", 0x1a, 0x1, HDA_INPUT),
	HDA_CODEC_MUTE("Input-2 Switch", 0x1a, 0x1, HDA_INPUT),
	HDA_CODEC_VOLUME("Input-3 Volume", 0x1a, 0x2, HDA_INPUT),
	HDA_CODEC_MUTE("Input-3 Switch", 0x1a, 0x2, HDA_INPUT),
	HDA_CODEC_VOLUME("Input-4 Volume", 0x1a, 0x3, HDA_INPUT),
	HDA_CODEC_MUTE("Input-4 Switch", 0x1a, 0x3, HDA_INPUT),
	HDA_CODEC_VOLUME("Input-5 Volume", 0x1a, 0x4, HDA_INPUT),
	HDA_CODEC_MUTE("Input-5 Switch", 0x1a, 0x4, HDA_INPUT),

	{ } /* end */
};

static struct hda_verb cxt5047_test_init_verbs[] = {
	/* Enable retasking pins as output, initially without power amp */
	{0x15, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
	{0x14, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},
	{0x13, AC_VERB_SET_PIN_WIDGET_CONTROL, PIN_OUT},

	/* Disable digital (SPDIF) pins initially, but users can enable
	 * them via a mixer switch.  In the case of SPDIF-out, this initverb
	 * payload also sets the generation to 0, output to be in "consumer"
	 * PCM format, copyright asserted, no pre-emphasis and no validity
	 * control.
	 */
	{0x18, AC_VERB_SET_DIGI_CONVERT_1, 0},

	/* Ensure mic1, mic2, line1 pin widgets take input from the 
	 * OUT1 sum bus when acting as an output.
	 */
	{0x1a, AC_VERB_SET_CONNECT_SEL, 0},
	{0x1b, AC_VERB_SET_CONNECT_SEL, 0},

	/* Start with output sum widgets muted and their output gains at min */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)},
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)},

	/* Unmute retasking pin widget output buffers since the default
	 * state appears to be output.  As the pin mode is changed by the
	 * user the pin mode control will take care of enabling the pin's
	 * input/output buffers as needed.
	 */
	{0x15, AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE},
	{0x14, AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE},
	{0x13, AC_VERB_SET_AMP_GAIN_MUTE, AMP_OUT_UNMUTE},

	/* Mute capture amp left and right */
	{0x12, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)},

	/* Set ADC connection select to match default mixer setting (mic1
	 * pin)
	 */
	{0x12, AC_VERB_SET_CONNECT_SEL, 0x00},

	/* Mute all inputs to mixer widget (even unconnected ones) */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(0)}, /* mic1 pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(1)}, /* mic2 pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(2)}, /* line1 pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(3)}, /* line2 pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(4)}, /* CD pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(5)}, /* Beep-gen pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(6)}, /* Line-out pin */
	{0x19, AC_VERB_SET_AMP_GAIN_MUTE, AMP_IN_MUTE(7)}, /* HP-pin pin */

	{ }
};
#endif


/* initialize jack-sensing, too */
static int cxt5047_hp_init(struct hda_codec *codec)
{
	conexant_init(codec);
	cxt5047_hp_automute(codec);
	return 0;
}


enum {
	CXT5047_LAPTOP,		/* Laptops w/o EAPD support */
	CXT5047_LAPTOP_HP,	/* Some HP laptops */
	CXT5047_LAPTOP_EAPD,	/* Laptops with EAPD support */
#ifdef CONFIG_SND_DEBUG
	CXT5047_TEST,
#endif
	CXT5047_MODELS
};

static const char *cxt5047_models[CXT5047_MODELS] = {
	[CXT5047_LAPTOP]	= "laptop",
	[CXT5047_LAPTOP_HP]	= "laptop-hp",
	[CXT5047_LAPTOP_EAPD]	= "laptop-eapd",
#ifdef CONFIG_SND_DEBUG
	[CXT5047_TEST]		= "test",
#endif
};

static struct snd_pci_quirk cxt5047_cfg_tbl[] = {
	SND_PCI_QUIRK(0x103c, 0x30a0, "HP DV1000", CXT5047_LAPTOP),
	SND_PCI_QUIRK(0x103c, 0x30b2, "HP DV2000T/DV3000T", CXT5047_LAPTOP),
	SND_PCI_QUIRK(0x103c, 0x30b5, "HP DV2000Z", CXT5047_LAPTOP),
	SND_PCI_QUIRK(0x103c, 0x30a5, "HP DV5200T/DV8000T", CXT5047_LAPTOP_HP),
	SND_PCI_QUIRK(0x1179, 0xff31, "Toshiba P100", CXT5047_LAPTOP_EAPD),
	{}
};

static int patch_cxt5047(struct hda_codec *codec)
{
	struct conexant_spec *spec;
	int board_config;

	spec = kzalloc(sizeof(*spec), GFP_KERNEL);
	if (!spec)
		return -ENOMEM;
	mutex_init(&spec->amp_mutex);
	codec->spec = spec;

	spec->multiout.max_channels = 2;
	spec->multiout.num_dacs = ARRAY_SIZE(cxt5047_dac_nids);
	spec->multiout.dac_nids = cxt5047_dac_nids;
	spec->multiout.dig_out_nid = CXT5047_SPDIF_OUT;
	spec->num_adc_nids = 1;
	spec->adc_nids = cxt5047_adc_nids;
	spec->capsrc_nids = cxt5047_capsrc_nids;
	spec->input_mux = &cxt5047_capture_source;
	spec->num_mixers = 1;
	spec->mixers[0] = cxt5047_mixers;
	spec->num_init_verbs = 1;
	spec->init_verbs[0] = cxt5047_init_verbs;
	spec->spdif_route = 0;
	spec->num_channel_mode = ARRAY_SIZE(cxt5047_modes),
	spec->channel_mode = cxt5047_modes,

	codec->patch_ops = conexant_patch_ops;
	codec->patch_ops.unsol_event = cxt5047_hp_unsol_event;

	board_config = snd_hda_check_board_config(codec, CXT5047_MODELS,
						  cxt5047_models,
						  cxt5047_cfg_tbl);
	switch (board_config) {
	case CXT5047_LAPTOP:
		break;
	case CXT5047_LAPTOP_HP:
		spec->input_mux = &cxt5047_hp_capture_source;
		spec->num_init_verbs = 2;
		spec->init_verbs[1] = cxt5047_hp_init_verbs;
		spec->mixers[0] = cxt5047_hp_mixers;
		codec->patch_ops.init = cxt5047_hp_init;
		break;
	case CXT5047_LAPTOP_EAPD:
		spec->input_mux = &cxt5047_toshiba_capture_source;
		spec->num_init_verbs = 2;
		spec->init_verbs[1] = cxt5047_toshiba_init_verbs;
		spec->mixers[0] = cxt5047_toshiba_mixers;
		break;
#ifdef CONFIG_SND_DEBUG
	case CXT5047_TEST:
		spec->input_mux = &cxt5047_test_capture_source;
		spec->mixers[0] = cxt5047_test_mixer;
		spec->init_verbs[0] = cxt5047_test_init_verbs;
#endif	
	}

	if(codec->mfg) {
		int (*cnxthwhda_probe)(void *codec, struct device *hwDev, void **ppDevNode);
		cnxthwhda_probe = (void*)symbol_request(cnxthwhda_probe);
		if(cnxthwhda_probe)
			symbol_put(cnxthwhda_probe);
		else
			printk(KERN_ERR"%s: symbol_request(cnxthwhda_probe) failed\n", __FUNCTION__);
	}

	return 0;
}

struct hda_codec_preset snd_hda_preset_conexant[] = {
	{ .id = 0x14f15045, .name = "CX20549 (Venice)",
	  .patch = patch_cxt5045 },
	{ .id = 0x14f15047, .name = "CX20551 (Waikiki)",
	  .patch = patch_cxt5047 },
	{ .id = 0x14f15051, .name = "CX20561 (Hermosa)",
	  .patch = patch_cxt5051 },
	{ .id = 0x14f12bfa, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsf },
	{ .id = 0x14f12c06, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsf },
	{ .id = 0x14f10000, .mask = 0xffff0000, .mfg = 2, .name = "HSF",
	  .patch = patch_cxthsf },
	{} /* terminator */
};



typedef enum {
	OsHdaStreamStateReset = 0,
	OsHdaStreamStateStop  = 1,
	OsHdaStreamStateRun   = 2
} OSHDA_STREAM_STATE;

static __shimcall__
unsigned int conexant_hsf_OsHdaCodecGetAddr(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->addr;
}

static __shimcall__
unsigned int conexant_hsf_OsHdaCodecGetVendorId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->vendor_id;
}

static __shimcall__
unsigned int conexant_hsf_OsHdaCodecGetSubsystemId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->subsystem_id;
}

static __shimcall__
unsigned int conexant_hsf_OsHdaCodecGetRevisionId(PHDAOSHAL pHdaOsHal)
{
	return ((struct hda_codec *)pHdaOsHal->hda_codec)->revision_id;
}

static __shimcall__
unsigned int conexant_hsf_OsHdaCodecRead(PHDAOSHAL pHdaOsHal, unsigned short nid, int direct, unsigned int verb, unsigned int para)
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
unsigned int conexant_hsf_OsHdaCodecWallclock(PHDAOSHAL pHdaOsHal)
{
	return snd_hda_codec_wallclock((struct hda_codec *)pHdaOsHal->hda_codec);
}

static __shimcall__
void conexant_hsf_OsHdaCodecSetEventCallback(PHDAOSHAL pHdaOsHal, void (*cbHdaEvent)(void *Context, unsigned int res), void *cbHdaEventContext, unsigned char *cbHdaTag)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;

	*cbHdaTag = ((struct hda_codec *)pHdaOsHal->hda_codec)->mfg;

//	printk(KERN_DEBUG"%s: codec=%p %p(%p) tag=%d\n", __FUNCTION__, pHdaOsHal->hda_codec, cbHdaEvent, cbHdaEventContext, (int)*cbHdaTag);

	spec->modem_cbHdaTag = *cbHdaTag;
	spec->modem_cbHdaEventContext = cbHdaEventContext;
	spec->modem_cbHdaEvent = (void*)cbHdaEvent;
}

static __shimcall__
void conexant_hsf_OsHdaCodecClearEventCallback(PHDAOSHAL pHdaOsHal, unsigned char cbHdaTag)
{
	struct conexant_spec *spec = ((struct hda_codec *)pHdaOsHal->hda_codec)->spec;

	if(spec) {
//		printk(KERN_DEBUG"%s: codec=%p Tag=%d\n", __FUNCTION__, pHdaOsHal->hda_codec, cbHdaTag);
		spec->modem_cbHdaEvent = NULL;
		spec->modem_cbHdaEventContext = NULL;
		spec->modem_cbHdaTag = 0;
	}
}


#include <sound/pcm_params.h>

#ifndef FOUND_SND_PCM_HW_PARAM_SET
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
#endif /* !FOUND_SND_PCM_HW_PARAM_SET */

static int conexant_modem_snd_pcm_change_params(struct snd_pcm_substream *substream, int hw_param_buffer_bytes)
{
	//snd_pcm_runtime_t *runtime = substream->runtime;
	snd_pcm_hw_params_t *sparams;
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

//printk(KERN_DEBUG"%s: substream=%p\n", __FUNCTION__, substream);

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
int conexant_hsf_OsHdaCodecOpenDMA(PHDAOSHAL pHdaOsHal, int hw_param_buffer_bytes, void **ppPlaybackStream, void **ppCaptureStream)
{
	int i, err;
	struct hda_codec *codec = (struct hda_codec *)pHdaOsHal->hda_codec;
	struct hda_pcm *info = codec->pcm_info;
	snd_pcm_t *pcm;
	struct conexant_spec *spec = codec->spec;

	struct snd_pcm_substream *psubstream = NULL, *csubstream = NULL;
#ifndef FOUND_OPEN_SUBSTREAM_NOFILE
	static struct file fil;
#endif

	for(i = 0; i < codec->num_pcms && !info[i].is_modem; i++);
	if(i == codec->num_pcms) {
		printk(KERN_ERR"%s: modem pcm not found\n", __FUNCTION__);
		return -ENOENT;
	}
	pcm = info[i].pcm;

//printk(KERN_DEBUG"%s: pcm=%p hw_param_buffer_bytes=%d\n", __FUNCTION__, pcm, hw_param_buffer_bytes);

#ifdef FOUND_OPEN_SUBSTREAM_NOFILE
	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_PLAYBACK,
#else
	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_PLAYBACK, &fil,
#endif
					&psubstream)) < 0) {
		printk(KERN_ERR"%s: snd_pcm_open_substream STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
		return err;
	}

#ifdef FOUND_OPEN_SUBSTREAM_NOFILE
	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_CAPTURE,
#else
	if ((err = snd_pcm_open_substream(pcm, SNDRV_PCM_STREAM_CAPTURE, &fil,
#endif
					&csubstream)) < 0) {
		printk(KERN_ERR"%s: snd_pcm_open_substream STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
		return err;
	}

#ifdef FOUND_OPEN_SUBSTREAM_NOFILE
	err = snd_pcm_hw_constraints_init(psubstream);
	if (err < 0) {
		printk(KERN_ERR"%s: snd_pcm_hw_constraints_init STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
	}
	if ((err = psubstream->ops->open(psubstream)) < 0) {
		printk(KERN_ERR"%s: ops open STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
	}
	//psubstream->ffile = file;
	err = snd_pcm_hw_constraints_complete(psubstream);
	if (err < 0) {
		printk(KERN_ERR"%s: snd_pcm_hw_constraints_complete STREAM_PLAYBACK failed (%d)\n", __FUNCTION__, err);
	}

	err = snd_pcm_hw_constraints_init(csubstream);
	if (err < 0) {
		printk(KERN_ERR"%s: snd_pcm_hw_constraints_init STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
	}
	if ((err = csubstream->ops->open(csubstream)) < 0) {
		printk(KERN_ERR"%s: ops open STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
	}
	//csubstream->ffile = file;
	err = snd_pcm_hw_constraints_complete(csubstream);
	if (err < 0) {
		printk(KERN_ERR"%s: snd_pcm_hw_constraints_complete STREAM_CAPTURE failed (%d)\n", __FUNCTION__, err);
	}
#endif

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
	//printk(KERN_DEBUG"%s: substream=%p\n", __FUNCTION__, substream);

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
void conexant_hsf_OsHdaCodecCloseDMA(PHDAOSHAL pHdaOsHal, void *pPlaybackStream, void *pCaptureStream)
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
void conexant_hsf_OsHdaCodecDMAInfo(PHDAOSHAL pHdaOsHal, void *Stream, unsigned char *StreamID, unsigned long *FifoSize, short unsigned int **ppBufAddr)
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
int conexant_hsf_OsHdaCodecSetDMAState(PHDAOSHAL pHdaOsHal, OSHDA_STREAM_STATE streamState, void *pPlaybackStream, void *pCaptureStream)
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
#ifdef FOUND_OPEN_SUBSTREAM_NOFILE
			err = snd_pcm_kernel_playback_ioctl(psubstream, cmd, NULL);
#else
			err = snd_pcm_kernel_ioctl(psubstream, cmd, NULL);
#endif
			if (err < 0) {
				printk(KERN_ERR"%s: snd_pcm_kernel_ioctl (playback) failed (%d)\n", __FUNCTION__, err);
			}

#ifdef FOUND_OPEN_SUBSTREAM_NOFILE
			err = snd_pcm_kernel_capture_ioctl(csubstream, cmd, NULL);
#else
			err = snd_pcm_kernel_ioctl(csubstream, cmd, NULL);
#endif
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
unsigned long conexant_hsf_OsHdaCodecGetDMAPos(PHDAOSHAL pHdaOsHal, void *Stream)
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


static void *conexant_hsf_interface_funcs[] = {
        conexant_hsf_OsHdaCodecGetAddr,
        conexant_hsf_OsHdaCodecGetVendorId,
        conexant_hsf_OsHdaCodecGetSubsystemId,
        conexant_hsf_OsHdaCodecGetRevisionId,
        conexant_hsf_OsHdaCodecRead,
        conexant_hsf_OsHdaCodecWallclock,
        conexant_hsf_OsHdaCodecSetEventCallback,
        conexant_hsf_OsHdaCodecClearEventCallback,
        conexant_hsf_OsHdaCodecOpenDMA,
        conexant_hsf_OsHdaCodecCloseDMA,
        conexant_hsf_OsHdaCodecDMAInfo,
        conexant_hsf_OsHdaCodecSetDMAState,
        conexant_hsf_OsHdaCodecGetDMAPos,
        NULL
};

