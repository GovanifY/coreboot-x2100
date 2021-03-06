/* SPDX-License-Identifier: GPL-2.0-only */

#include <string.h>
#include <fsp/api.h>
#include <soc/ramstage.h>
#include <smbios.h>
#include <spd.h>

#include "bmcinfo.h"

void mainboard_silicon_init_params(FSPS_UPD *params)
{
	/* Disable eMMC */
	params->FspsConfig.PcdEnableEmmc = 0;

	if (bmcinfo_disable_nic1())
		params->FspsConfig.PcdEnableGbE = 2; // disable lan 1 only
}

/* Override smbios_mainboard_serial_number to retrieve it from BMC */
const char *smbios_mainboard_serial_number(void)
{
	const char *bmc_serial = bmcinfo_serial();
	if (bmc_serial)
		return bmc_serial;
	return CONFIG_MAINBOARD_SERIAL_NUMBER;
}

/* Override smbios_system_set_uuid */
void smbios_system_set_uuid(u8 *uuid)
{
	const u8 *bmc_uuid = bmcinfo_uuid();
	if (bmc_uuid)
		memcpy(uuid, bmc_uuid, 16);
	/* leave all zero */
}

/* Override smbios_mainboard_version */
const char *smbios_mainboard_version(void)
{
	const int hwRev = bmcinfo_hwrev();
	switch (hwRev) {
	case 0:
		return "Z0";
	case 1:
		return "A0";
	case 2:
		return "A1";
	}
	return "";
}

/* Override smbios_mainboard_features_flags */
u8 smbios_mainboard_feature_flags(void)
{
	return 0xc;
}

/* Override smbios_mainboard_location_in_chassis */
const char *smbios_mainboard_location_in_chassis(void)
{
	static char location[4] = "n/a";
	int slot = bmcinfo_slot();
	if (slot >= 0)
		snprintf(location, 4, "N%d", slot);
	return location;
}

/* Override smbios_mainboard_board_type */
smbios_board_type smbios_mainboard_board_type(void)
{
	return SMBIOS_BOARD_TYPE_SERVER_BLADE;
}

smbios_enclosure_type smbios_mainboard_enclosure_type(void)
{
	return SMBIOS_ENCLOSURE_MULTI_SYSTEM_CHASSIS;
}

/* Add any mainboard specific information for dimm */
void mainboard_add_dimm_info(
	struct memory_info *mem_info,
	int channel, int dimm, int index)
{
	/* Mainboard only has DDR4 DIMM slots */
	mem_info->dimm[index].mod_type = DDR4_SPD_UDIMM;
}
