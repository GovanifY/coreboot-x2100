/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _COREBOOT_SRC_MAINBOARD_GOOGLE_HEROBRINE_BOARD_H_
#define _COREBOOT_SRC_MAINBOARD_GOOGLE_HEROBRINE_BOARD_H_

#include <assert.h>
#include <boardid.h>
#include <gpio.h>

#define GPIO_SD_CD_L	GPIO(91)

#define QCOM_SC7280_SKU1 0x0
#define QCOM_SC7280_SKU2 0x1
#define QCOM_SC7280_SKU3 0x2

/* Fingerprint-specific GPIOs. Only for fingerprint-enabled devices. */
#if CONFIG(HEROBRINE_HAS_FINGERPRINT)
#define GPIO_FPMCU_BOOT0	GPIO(77)
#define GPIO_FP_RST_L		GPIO(78)
#define GPIO_EN_FP_RAILS	GPIO(42)
#else
#define GPIO_FPMCU_BOOT0	dead_code_t(gpio_t)
#define GPIO_FP_RST_L		dead_code_t(gpio_t)
#define GPIO_EN_FP_RAILS	dead_code_t(gpio_t)
#endif

void setup_chromeos_gpios(void);

#endif /* _COREBOOT_SRC_MAINBOARD_GOOGLE_HEROBRINE_BOARD_H_ */
