/* SPDX-License-Identifier: GPL-2.0-only */

#include <arch/stages.h>
#include <soc/mt6366.h>

void platform_romstage_main(void)
{
	mt6366_init();
}
