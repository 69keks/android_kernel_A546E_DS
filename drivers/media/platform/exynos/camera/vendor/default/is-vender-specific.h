/*
* Samsung Exynos5 SoC series FIMC-IS driver
 *
 * exynos5 fimc-is vender functions
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef IS_VENDER_SPECIFIC_H
#define IS_VENDER_SPECIFIC_H

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/sched/rt.h>

struct is_vender_specific {
	struct i2c_client	*eeprom_client[ROM_ID_MAX];
	bool			rom_valid[ROM_ID_MAX];

	u32			sensor_id[SENSOR_POSITION_MAX];
	const char		*sensor_name[SENSOR_POSITION_MAX];

	u32			ois_sensor_index;
	u32                     aperture_sensor_index;
	u32			mcu_sensor_index;
	bool			zoom_running;
	int			isDualized[SENSOR_POSITION_MAX];
};

#endif
