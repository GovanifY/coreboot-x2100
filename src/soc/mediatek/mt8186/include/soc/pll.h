/* SPDX-License-Identifier: GPL-2.0-only */

/*
 * This file is created based on MT8186 Functional Specification
 * Chapter number: 3.2
 */

#ifndef SOC_MEDIATEK_MT8186_PLL_H
#define SOC_MEDIATEK_MT8186_PLL_H

#include <soc/pll_common.h>

/* top_div rate */
enum {
	CLK26M_HZ	= 26 * MHz,
};

/* top_mux rate */
enum {
	UART_HZ		= CLK26M_HZ,
};

#endif /* SOC_MEDIATEK_MT8186_PLL_H */
