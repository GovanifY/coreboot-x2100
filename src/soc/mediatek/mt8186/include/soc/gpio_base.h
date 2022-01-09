/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * This file is created based on MT8186 Functional Specification
 * Chapter number: 5.1
 */

#ifndef SOC_MEDIATEK_MT8186_GPIO_BASE_H
#define SOC_MEDIATEK_MT8186_GPIO_BASE_H

#include <stdint.h>

typedef union {
	u32 raw;
	struct {
		u32 id     : 8;
		u32 flag   : 3;
		u32 bit    : 5;
		u32 base   : 8;
		u32 offset : 8;
	};
} gpio_t;

#endif
