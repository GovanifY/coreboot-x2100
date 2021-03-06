/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <variant/gpio.h>

void bootblock_mainboard_init(void)
{
	gpio_configure_pads(early_gpio_table, ARRAY_SIZE(early_gpio_table));
}
