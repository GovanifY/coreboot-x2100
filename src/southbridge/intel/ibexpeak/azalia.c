/* SPDX-License-Identifier: GPL-2.0-only */

#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <device/pci_ops.h>
#include <device/mmio.h>
#include <delay.h>
#include <device/azalia_device.h>
#include "pch.h"

static int codec_detect(u8 *base)
{
	u8 reg8;

	if (azalia_exit_reset(base) < 0)
		goto no_codec;

	/* Write back the value once reset bit is set. */
	write16(base + HDA_GCAP_REG, read16(base + HDA_GCAP_REG));

	/* Read in Codec location (BAR + 0xe)[2..0] */
	reg8 = read8(base + HDA_STATESTS_REG);
	reg8 &= 0x0f;
	if (!reg8)
		goto no_codec;

	return reg8;

no_codec:
	/* Codec not found, put HDA back in reset */
	azalia_enter_reset(base);
	printk(BIOS_DEBUG, "Azalia: No codec!\n");
	return 0;
}

static void azalia_init(struct device *dev)
{
	u8 *base;
	struct resource *res;
	u32 codec_mask;
	u8 reg8;
	u16 reg16;
	u32 reg32;

	/* Find base address */
	res = probe_resource(dev, PCI_BASE_ADDRESS_0);
	if (!res)
		return;

	// NOTE this will break as soon as the Azalia gets a bar above 4G.
	// Is there anything we can do about it?
	base = res2mmio(res, 0, 0);
	printk(BIOS_DEBUG, "Azalia: base = %p\n", base);

	if (RCBA32(0x2030) & (1 << 31)) {
		reg32 = pci_read_config32(dev, 0x120);
		reg32 &= 0xf8ffff01;
		reg32 |= (1 << 24); // 2 << 24 for server
		reg32 |= RCBA32(0x2030) & 0xfe;
		pci_write_config32(dev, 0x120, reg32);

		reg16 = pci_read_config16(dev, 0x78);
		reg16 |= (1 << 11);
		pci_write_config16(dev, 0x78, reg16);
	} else
		printk(BIOS_DEBUG, "Azalia: V1CTL disabled.\n");

	reg32 = pci_read_config32(dev, 0x114);
	reg32 &= ~0xfe;
	pci_write_config32(dev, 0x114, reg32);

	// Set VCi enable bit
	reg32 = pci_read_config32(dev, 0x120);
	reg32 |= (1 << 31);
	pci_write_config32(dev, 0x120, reg32);

	// Enable HDMI codec:
	reg32 = pci_read_config32(dev, 0xc4);
	reg32 |= (1 << 1);
	pci_write_config32(dev, 0xc4, reg32);

	reg8 = pci_read_config8(dev, 0x43);
	reg8 |= (1 << 6);
	pci_write_config8(dev, 0x43, reg8);

	reg32 = pci_read_config32(dev, 0xd0);
	reg32 &= ~(1 << 31);
	pci_write_config32(dev, 0xd0, reg32);

	/* Set Bus Master */
	pci_or_config16(dev, PCI_COMMAND, PCI_COMMAND_MASTER);

	pci_write_config8(dev, 0x3c, 0x0a); // unused?

	/* Codec Initialization Programming Sequence */

	/* Take controller out of reset */
	reg32 = read32(base + HDA_GCTL_REG);
	reg32 |= HDA_GCTL_CRST;
	write32(base + HDA_GCTL_REG, reg32);
	/* Wait 1ms */
	udelay(1000);

	//
	reg8 = pci_read_config8(dev, 0x40); // Audio Control
	reg8 |= 1; // Select Azalia mode. This needs to be controlled via devicetree.cb
	pci_write_config8(dev, 0x40, reg8);

	reg8 = pci_read_config8(dev, 0x4d); // Docking Status
	reg8 &= ~(1 << 7); // Docking not supported
	pci_write_config8(dev, 0x4d, reg8);

	codec_mask = codec_detect(base);

	if (codec_mask) {
		printk(BIOS_DEBUG, "Azalia: codec_mask = %02x\n", codec_mask);
		azalia_codecs_init(base, codec_mask);
	}

	/* Enable dynamic clock gating */
	reg8 = pci_read_config8(dev, 0x43);
	reg8 &= ~0x7;
	reg8 |= (1 << 2) | (1 << 0);
	pci_write_config8(dev, 0x43, reg8);
}

static struct device_operations azalia_ops = {
	.read_resources		= pci_dev_read_resources,
	.set_resources		= pci_dev_set_resources,
	.enable_resources	= pci_dev_enable_resources,
	.init			= azalia_init,
	.ops_pci		= &pci_dev_ops_pci,
};

static const unsigned short pci_device_ids[] = {
	0x1c20,
	0x1e20,
	PCI_DID_INTEL_IBEXPEAK_AUDIO,
	0
};

static const struct pci_driver pch_azalia __pci_driver = {
	.ops	 = &azalia_ops,
	.vendor	 = PCI_VENDOR_ID_INTEL,
	.devices = pci_device_ids,
};
