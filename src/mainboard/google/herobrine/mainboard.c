/* SPDX-License-Identifier: GPL-2.0-only */

#include "board.h"
#include <boardid.h>
#include <bootblock_common.h>
#include <soc/clock.h>
#include <console/console.h>
#include <device/device.h>
#include <device/mmio.h>
#include <bootblock_common.h>
#include <soc/clock.h>
#include <soc/qupv3_config_common.h>
#include <soc/qup_se_handlers_common.h>
#include <soc/qcom_qup_se.h>

static void configure_sdhci(void)
{
	/* Program eMMC drive strength to 16/10/10 mA */
	write32((void *)SDC1_TLMM_CFG_ADDR, 0x9FE4);
	/* Program SD card drive strength to 16/10/10 mA */
	write32((void *)SDC2_TLMM_CFG_ADDR, 0x1FE4);
}

static void mainboard_init(struct device *dev)
{
	/* Configure clock for eMMC */
	clock_configure_sdcc(1, 384 * MHz);
	/* Configure clock for SD card */
	clock_configure_sdcc(2, 50 * MHz);
	configure_sdhci();

	gpi_firmware_load(QUP_0_GSI_BASE);
	gpi_firmware_load(QUP_1_GSI_BASE);

	/*
	 * When coreboot firmware disables serial output,
	 * we still need to load console UART QUP FW for OS.
	 */
	if (!CONFIG(CONSOLE_SERIAL))
		qupv3_se_fw_load_and_init(QUPV3_0_SE5, SE_PROTOCOL_UART, FIFO);

	qupv3_se_fw_load_and_init(QUPV3_1_SE5, SE_PROTOCOL_I2C, MIXED);	 /* Touch I2C */
	qupv3_se_fw_load_and_init(QUPV3_0_SE7, SE_PROTOCOL_UART, FIFO);  /* BT UART */

	if (CONFIG(BOARD_GOOGLE_HEROBRINE)) {
		/* Audio I2C */
		qupv3_se_fw_load_and_init(QUPV3_0_SE0, SE_PROTOCOL_I2C, MIXED);
		/* Trackpad I2C */
		qupv3_se_fw_load_and_init(QUPV3_0_SE1, SE_PROTOCOL_I2C, MIXED);
		/* SAR sensor I2C */
		qupv3_se_fw_load_and_init(QUPV3_0_SE2, SE_PROTOCOL_I2C, MIXED);
		/* Fingerprint SPI */
		qupv3_se_fw_load_and_init(QUPV3_1_SE3, SE_PROTOCOL_SPI, MIXED);
	} else if (CONFIG(BOARD_GOOGLE_PIGLIN)) {
		/* APPS I2C */
		qupv3_se_fw_load_and_init(QUPV3_0_SE1, SE_PROTOCOL_I2C, GSI);
		/* ESIM SPI */
		qupv3_se_fw_load_and_init(QUPV3_1_SE4, SE_PROTOCOL_SPI, MIXED);
		/* Fingerprint SPI */
		qupv3_se_fw_load_and_init(QUPV3_1_SE6, SE_PROTOCOL_SPI, MIXED);
	}

	/* Take FPMCU out of reset. Power was already applied
	   in romstage and should have stabilized by now. */
	if (CONFIG(HEROBRINE_HAS_FINGERPRINT))
		gpio_output(GPIO_FP_RST_L, 1);

}

static void mainboard_enable(struct device *dev)
{
	dev->ops->init = &mainboard_init;
}

struct chip_operations mainboard_ops = {
	.name = CONFIG_MAINBOARD_PART_NUMBER,
	.enable_dev = mainboard_enable,
};
