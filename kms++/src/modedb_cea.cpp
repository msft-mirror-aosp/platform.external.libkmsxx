/* From Linux kernel: drm_edid.c */
/*
 * Copyright (c) 2006 Luc Verhaegen (quirks list)
 * Copyright (c) 2007-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 * Copyright 2010 Red Hat, Inc.
 *
 * DDC probing routines (drm_ddc_read & drm_do_probe_ddc_edid) originally from
 * FB layer.
 *   Copyright (C) 2006 Dennis Munsie <dmunsie@cecropia.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <kms++/modedb.h>

#include <xf86drm.h>

namespace kms
{
#define DIV_ROUND(n, d) (((n) + (d) / 2) / (d))

#define DRM_MODE(nm, c, hd, hss, hse, ht, hsk, vd, vss, vse, vt, vs, f)                             \
	.name = nm, .clock = (c),                                                                   \
	.hdisplay = (hd), .hsync_start = (hss), .hsync_end = (hse), .htotal = (ht), .hskew = (hsk), \
	.vdisplay = (vd), .vsync_start = (vss), .vsync_end = (vse), .vtotal = (vt), .vscan = (vs),  \
	.vrefresh = DIV_ROUND(c * 1000, ht * vt) * (((f)&DRM_MODE_FLAG_INTERLACE) ? 2 : 1),         \
	.flags = (f), .type = 0

/*
 * Probably taken from CEA-861 spec.
 * This table is converted from xorg's hw/xfree86/modes/xf86EdidModes.c.
 */
const Videomode cea_modes[] = {
	/* 1 - 640x480@60Hz */
	{
		DRM_MODE("640x480", 25175, 640, 656,
			 752, 800, 0, 480, 490, 492, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 2 - 720x480@60Hz */
	{
		DRM_MODE("720x480", 27000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 3 - 720x480@60Hz */
	{
		DRM_MODE("720x480", 27000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 4 - 1280x720@60Hz */
	{
		DRM_MODE("1280x720", 74250, 1280, 1390,
			 1430, 1650, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 5 - 1920x1080i@60Hz */
	{
		DRM_MODE("1920x1080i", 74250, 1920, 2008,
			 2052, 2200, 0, 1080, 1084, 1094, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 6 - 720(1440)x480i@60Hz */
	{
		DRM_MODE("720x480i", 13500, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 7 - 720(1440)x480i@60Hz */
	{
		DRM_MODE("720x480i", 13500, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 8 - 720(1440)x240@60Hz */
	{
		DRM_MODE("720x240", 13500, 720, 739,
			 801, 858, 0, 240, 244, 247, 262, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_DBLCLK),
	},
	/* 9 - 720(1440)x240@60Hz */
	{
		DRM_MODE("720x240", 13500, 720, 739,
			 801, 858, 0, 240, 244, 247, 262, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_DBLCLK),
	},
	/* 10 - 2880x480i@60Hz */
	{
		DRM_MODE("2880x480i", 54000, 2880, 2956,
			 3204, 3432, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 11 - 2880x480i@60Hz */
	{
		DRM_MODE("2880x480i", 54000, 2880, 2956,
			 3204, 3432, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 12 - 2880x240@60Hz */
	{
		DRM_MODE("2880x240", 54000, 2880, 2956,
			 3204, 3432, 0, 240, 244, 247, 262, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 13 - 2880x240@60Hz */
	{
		DRM_MODE("2880x240", 54000, 2880, 2956,
			 3204, 3432, 0, 240, 244, 247, 262, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 14 - 1440x480@60Hz */
	{
		DRM_MODE("1440x480", 54000, 1440, 1472,
			 1596, 1716, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 15 - 1440x480@60Hz */
	{
		DRM_MODE("1440x480", 54000, 1440, 1472,
			 1596, 1716, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 16 - 1920x1080@60Hz */
	{
		DRM_MODE("1920x1080", 148500, 1920, 2008,
			 2052, 2200, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 17 - 720x576@50Hz */
	{
		DRM_MODE("720x576", 27000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 18 - 720x576@50Hz */
	{
		DRM_MODE("720x576", 27000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 19 - 1280x720@50Hz */
	{
		DRM_MODE("1280x720", 74250, 1280, 1720,
			 1760, 1980, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 20 - 1920x1080i@50Hz */
	{
		DRM_MODE("1920x1080i", 74250, 1920, 2448,
			 2492, 2640, 0, 1080, 1084, 1094, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 21 - 720(1440)x576i@50Hz */
	{
		DRM_MODE("720x576i", 13500, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 22 - 720(1440)x576i@50Hz */
	{
		DRM_MODE("720x576i", 13500, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 23 - 720(1440)x288@50Hz */
	{
		DRM_MODE("720x288", 13500, 720, 732,
			 795, 864, 0, 288, 290, 293, 312, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_DBLCLK),
	},
	/* 24 - 720(1440)x288@50Hz */
	{
		DRM_MODE("720x288", 13500, 720, 732,
			 795, 864, 0, 288, 290, 293, 312, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_DBLCLK),
	},
	/* 25 - 2880x576i@50Hz */
	{
		DRM_MODE("2880x576i", 54000, 2880, 2928,
			 3180, 3456, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 26 - 2880x576i@50Hz */
	{
		DRM_MODE("2880x576i", 54000, 2880, 2928,
			 3180, 3456, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 27 - 2880x288@50Hz */
	{
		DRM_MODE("2880x288", 54000, 2880, 2928,
			 3180, 3456, 0, 288, 290, 293, 312, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 28 - 2880x288@50Hz */
	{
		DRM_MODE("2880x288", 54000, 2880, 2928,
			 3180, 3456, 0, 288, 290, 293, 312, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 29 - 1440x576@50Hz */
	{
		DRM_MODE("1440x576", 54000, 1440, 1464,
			 1592, 1728, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 30 - 1440x576@50Hz */
	{
		DRM_MODE("1440x576", 54000, 1440, 1464,
			 1592, 1728, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 31 - 1920x1080@50Hz */
	{
		DRM_MODE("1920x1080", 148500, 1920, 2448,
			 2492, 2640, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 32 - 1920x1080@24Hz */
	{
		DRM_MODE("1920x1080", 74250, 1920, 2558,
			 2602, 2750, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 33 - 1920x1080@25Hz */
	{
		DRM_MODE("1920x1080", 74250, 1920, 2448,
			 2492, 2640, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 34 - 1920x1080@30Hz */
	{
		DRM_MODE("1920x1080", 74250, 1920, 2008,
			 2052, 2200, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 35 - 2880x480@60Hz */
	{
		DRM_MODE("2880x480", 108000, 2880, 2944,
			 3192, 3432, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 36 - 2880x480@60Hz */
	{
		DRM_MODE("2880x480", 108000, 2880, 2944,
			 3192, 3432, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 37 - 2880x576@50Hz */
	{
		DRM_MODE("2880x576", 108000, 2880, 2928,
			 3184, 3456, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 38 - 2880x576@50Hz */
	{
		DRM_MODE("2880x576", 108000, 2880, 2928,
			 3184, 3456, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 39 - 1920x1080i@50Hz */
	{
		DRM_MODE("1920x1080i", 72000, 1920, 1952,
			 2120, 2304, 0, 1080, 1126, 1136, 1250, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 40 - 1920x1080i@100Hz */
	{
		DRM_MODE("1920x1080i", 148500, 1920, 2448,
			 2492, 2640, 0, 1080, 1084, 1094, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 41 - 1280x720@100Hz */
	{
		DRM_MODE("1280x720", 148500, 1280, 1720,
			 1760, 1980, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 42 - 720x576@100Hz */
	{
		DRM_MODE("720x576", 54000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 43 - 720x576@100Hz */
	{
		DRM_MODE("720x576", 54000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 44 - 720(1440)x576i@100Hz */
	{
		DRM_MODE("720x576i", 27000, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 45 - 720(1440)x576i@100Hz */
	{
		DRM_MODE("720x576i", 27000, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 46 - 1920x1080i@120Hz */
	{
		DRM_MODE("1920x1080i", 148500, 1920, 2008,
			 2052, 2200, 0, 1080, 1084, 1094, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
				 DRM_MODE_FLAG_INTERLACE),
	},
	/* 47 - 1280x720@120Hz */
	{
		DRM_MODE("1280x720", 148500, 1280, 1390,
			 1430, 1650, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 48 - 720x480@120Hz */
	{
		DRM_MODE("720x480", 54000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 49 - 720x480@120Hz */
	{
		DRM_MODE("720x480", 54000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 50 - 720(1440)x480i@120Hz */
	{
		DRM_MODE("720x480i", 27000, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 51 - 720(1440)x480i@120Hz */
	{
		DRM_MODE("720x480i", 27000, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 52 - 720x576@200Hz */
	{
		DRM_MODE("720x576", 108000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 53 - 720x576@200Hz */
	{
		DRM_MODE("720x576", 108000, 720, 732,
			 796, 864, 0, 576, 581, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 54 - 720(1440)x576i@200Hz */
	{
		DRM_MODE("720x576i", 54000, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 55 - 720(1440)x576i@200Hz */
	{
		DRM_MODE("720x576i", 54000, 720, 732,
			 795, 864, 0, 576, 580, 586, 625, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 56 - 720x480@240Hz */
	{
		DRM_MODE("720x480", 108000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 57 - 720x480@240Hz */
	{
		DRM_MODE("720x480", 108000, 720, 736,
			 798, 858, 0, 480, 489, 495, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC),
	},
	/* 58 - 720(1440)x480i@240 */
	{
		DRM_MODE("720x480i", 54000, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 59 - 720(1440)x480i@240 */
	{
		DRM_MODE("720x480i", 54000, 720, 739,
			 801, 858, 0, 480, 488, 494, 525, 0,
			 DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_NVSYNC |
				 DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
	},
	/* 60 - 1280x720@24Hz */
	{
		DRM_MODE("1280x720", 59400, 1280, 3040,
			 3080, 3300, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 61 - 1280x720@25Hz */
	{
		DRM_MODE("1280x720", 74250, 1280, 3700,
			 3740, 3960, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 62 - 1280x720@30Hz */
	{
		DRM_MODE("1280x720", 74250, 1280, 3040,
			 3080, 3300, 0, 720, 725, 730, 750, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 63 - 1920x1080@120Hz */
	{
		DRM_MODE("1920x1080", 297000, 1920, 2008,
			 2052, 2200, 0, 1080, 1084, 1089, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* 64 - 1920x1080@100Hz */
	{
		DRM_MODE("1920x1080", 297000, 1920, 2448,
			 2492, 2640, 0, 1080, 1084, 1094, 1125, 0,
			 DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC),
	},
	/* TERMINATOR */
	{},
};

} // namespace kms
