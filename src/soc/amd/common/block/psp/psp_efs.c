/* SPDX-License-Identifier: GPL-2.0-only */

#include <amdblocks/psp_efs.h>
#include <arch/mmio.h>
#include <boot_device.h>
#include <commonlib/region.h>
#include <types.h>

static struct embedded_firmware *efs;

bool efs_is_valid(void)
{
	if (!efs)
		efs = rdev_mmap(boot_device_ro(), EFS_OFFSET, sizeof(*efs));

	if (!efs || efs->signature != EMBEDDED_FW_SIGNATURE)
		return false;

	return true;
}

bool read_efs_spi_settings(uint8_t *mode, uint8_t *speed)
{
	if (!efs_is_valid())
		return false;

	*mode = efs->SPI_MODE_FIELD;
	*speed = efs->SPI_SPEED_FIELD;
	return true;
}
