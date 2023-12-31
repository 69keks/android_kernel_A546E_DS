// SPDX-License-Identifier: GPL-2.0
/*
 * Samsung Exynos SoC series Pablo driver
 *
 * Copyright (c) 2021 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/firmware.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/videodev2.h>
#include <videodev2_exynos_camera.h>
#include <linux/v4l2-mediabus.h>
#include <linux/bug.h>

#include "is-core.h"
#include "is-cmd.h"
#include "is-err.h"
#include "is-video.h"
#include "is-dvfs.h"

int is_shrp_video_probe(struct is_video *video, void *data, u32 video_num, u32 index)
{
	int ret;
	struct is_core *core = (struct is_core *)data;

	video->resourcemgr = &core->resourcemgr;
	video->group_id = GROUP_ID_SHRP;
	video->group_ofs = offsetof(struct is_device_ischain, group_shrp);
	video->subdev_id = ENTRY_SHRP;
	video->subdev_ofs = offsetof(struct is_group, leader);
	video->buf_rdycount = 0;

	ret = is_video_probe(video,
		IS_VIDEO_SHRP_NAME,
		video_num,
		VFL_DIR_M2M,
		&core->resourcemgr.mem,
		&core->v4l2_dev,
		NULL, NULL, NULL);
	if (ret)
		dev_err(&core->pdev->dev, "failure in is_video_probe(): %d)\n", ret);

	return ret;
}
