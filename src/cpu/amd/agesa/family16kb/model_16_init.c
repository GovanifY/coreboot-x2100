/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <cpu/x86/msr.h>
#include <cpu/amd/msr.h>
#include <cpu/x86/mtrr.h>
#include <cpu/amd/mtrr.h>
#include <device/device.h>
#include <cpu/x86/pae.h>
#include <cpu/x86/lapic.h>
#include <cpu/cpu.h>
#include <cpu/x86/cache.h>
#include <acpi/acpi.h>
#include <northbridge/amd/agesa/agesa_helper.h>

static void model_16_init(struct device *dev)
{
	printk(BIOS_DEBUG, "Model 16 Init.\n");

	msr_t msr;
	int msrno;
#if CONFIG(LOGICAL_CPUS)
	u32 siblings;
#endif

	//enable_cache();
	//amd_setup_mtrrs();
	//x86_mtrr_check();
	disable_cache();
	/* Enable access to AMD RdDram and WrDram extension bits */
	msr = rdmsr(SYSCFG_MSR);
	msr.lo |= SYSCFG_MSR_MtrrFixDramModEn;
	msr.lo &= ~SYSCFG_MSR_MtrrFixDramEn;
	wrmsr(SYSCFG_MSR, msr);

	// BSP: make a0000-bffff UC, c0000-fffff WB, same as OntarioApMtrrSettingsList for APs
	msr.lo = msr.hi = 0;
	wrmsr(MTRR_FIX_16K_A0000, msr);
	msr.lo = msr.hi = 0x1e1e1e1e;
	wrmsr(MTRR_FIX_64K_00000, msr);
	wrmsr(MTRR_FIX_16K_80000, msr);
	for (msrno = MTRR_FIX_4K_C0000; msrno <= MTRR_FIX_4K_F8000; msrno++)
		wrmsr(msrno, msr);

	msr = rdmsr(SYSCFG_MSR);
	msr.lo &= ~SYSCFG_MSR_MtrrFixDramModEn;
	msr.lo |= SYSCFG_MSR_MtrrFixDramEn;
	wrmsr(SYSCFG_MSR, msr);

	if (acpi_is_wakeup_s3())
		restore_mtrr();

	x86_mtrr_check();
	enable_cache();

	/* zero the machine check error status registers */
	mca_clear_status();

	/* Enable the local CPU APICs */
	setup_lapic();

#if CONFIG(LOGICAL_CPUS)
	siblings = cpuid_ecx(0x80000008) & 0xff;

	if (siblings > 0) {
		msr = rdmsr_amd(CPU_ID_FEATURES_MSR);
		msr.lo |= 1 << 28;
		wrmsr_amd(CPU_ID_FEATURES_MSR, msr);

		msr = rdmsr_amd(CPU_ID_EXT_FEATURES_MSR);
		msr.hi |= 1 << (33 - 32);
		wrmsr_amd(CPU_ID_EXT_FEATURES_MSR, msr);
	}
	printk(BIOS_DEBUG, "siblings = %02d, ", siblings);
#endif

	/* DisableCf8ExtCfg */
	msr = rdmsr(NB_CFG_MSR);
	msr.hi &= ~(1 << (46 - 32));
	wrmsr(NB_CFG_MSR, msr);

	/* Write protect SMM space with SMMLOCK. */
	msr = rdmsr(HWCR_MSR);
	msr.lo |= (1 << 0);
	wrmsr(HWCR_MSR, msr);
}

static struct device_operations cpu_dev_ops = {
	.init = model_16_init,
};

static const struct cpu_device_id cpu_table[] = {
	{ X86_VENDOR_AMD, 0x700f00 },	/* KB-A0 */
	{ 0, 0 },
};

static const struct cpu_driver model_15 __cpu_driver = {
	.ops      = &cpu_dev_ops,
	.id_table = cpu_table,
};
