/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * asid.h: ASID management
 * Copyright (c) 2007, Advanced Micro Devices, Inc.
 * Copyright (c) 2009, Citrix Systems, Inc.
 */

#ifndef __ASM_X86_HVM_ASID_H__
#define __ASM_X86_HVM_ASID_H__

struct hvm_domain;
struct hvm_domain_asid;

/* Initialise ASID management distributed across all CPUs. */
void hvm_asid_init(int nasids);

/* Invalidate a particular ASID allocation: forces re-allocation. */
void hvm_asid_flush_domain_asid(struct hvm_domain_asid *asid);

/* Invalidate all ASID allocations for specified domain */
void hvm_asid_flush_domain(struct domain *d);

/* Called while creating a domain. Checks if the domain has a valid asid, if not
then assigns a new one */
bool hvm_asid_domain_create(struct hvm_domain_asid *asid);

#endif /* __ASM_X86_HVM_ASID_H__ */

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
