/* SPDX-License-Identifier: GPL-2.0-only */

/* Global Variables */

Field (GNVS, ByteAcc, NoLock, Preserve)
{
	/* Miscellaneous */
	,	16,	// 0x00 - Operating System
	SMIF,	8,	// 0x02 - SMI function
	,	8,	// 0x03 - SMI function parameter
	,	8,	// 0x04 - SMI function parameter
	SCIF,	8,	// 0x05 - SCI function
	,	8,	// 0x06 - SCI function parameter
	,	8,	// 0x07 - SCI function parameter
	,	8,	// 0x08 - Global Lock function for EC
	,	8,	// 0x09 - Lock function parameter
	,	8,	// 0x0a - Lock function parameter
	,	8,	// 0x0b - Processor Count
	PPCM,	8,	// 0x0c - Max PPC State
	TMPS,	8,	// 0x0d - Temperature Sensor ID
	TLVL,	8,	// 0x0e - Throttle Level Limit
	FLVL,	8,	// 0x0f - Current FAN Level
	TCRT,	8,	// 0x10 - Critical Threshold
	TPSV,	8,	// 0x11 - Passive Threshold
	TMAX,	8,	// 0x12 - CPU Tj_max
	S5U0,	8,	// 0x13 - Enable USB in S5
	S3U0,	8,	// 0x14 - Enable USB in S3
	S33G,	8,	// 0x15 - Enable 3G in S3
	LIDS,	8,	// 0x16 - LID State
	,	8,	// 0x17 - AC Power State
	,	32,	// 0x18 - 0x1b - CBMEM TOC
	CBMC,	32,	// 0x1c - 0x1f - coreboot Memory Console
	PM1I,	64,	// 0x20 - 0x27 - PM1 wake status bit
	GPEI,	64,	// 0x28 - 0x2f - GPE wake status bit
}
