/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _CPU_AMD_COMMON_H
#define _CPU_AMD_COMMON_H

#include <types.h>
#include <acpi/acpigen.h>

struct cppc_config;
void cpu_init_cppc_config(struct cppc_config *config, u32 version);
void generate_cppc_entries(unsigned int core_id);

#endif
