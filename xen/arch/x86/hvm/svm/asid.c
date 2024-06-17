/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * asid.c: handling ASIDs in SVM.
 * Copyright (c) 2007, Advanced Micro Devices, Inc.
 */

#include <asm/amd.h>
#include <asm/hvm/nestedhvm.h>
#include <asm/hvm/svm/svm.h>

#include <asm/processor.h>
#include "svm.h"
#include "xen/cpumask.h"

void svm_asid_init(const struct cpuinfo_x86 *c)
{
    //unsigned int cpu = smp_processor_id();
    // const struct cpuinfo_x86 *c;
    int nasids = 0;

   // for_each_online_cpu( cpu ) {
     //   c = &cpu_data[cpu];
        /* Check for erratum #170, and leave ASIDs disabled if it's present. */
        if ( !cpu_has_amd_erratum(c, AMD_ERRATUM_170) )
            nasids = cpuid_ebx(0x8000000aU);
   // }
    hvm_asid_init(nasids);
}

/*
 * Called directly after vcpus are initialized. Assigns asid to all
 * vcpus of the same domain.
 */
void svm_vcpu_assign_asid(struct vcpu *v)
{
    struct vmcb_struct *vmcb = v->arch.hvm.svm.vmcb;
    struct hvm_domain_asid *p_asid = &v->domain->arch.hvm.n1asid;

    // hvm_asid_domain_create(p_asid);

    /* ASID 0 indicates that ASIDs are disabled. */
    printk(XENLOG_INFO "Entered svm_vcpu_assign_asid and asid is %u", p_asid->asid);
    if ( p_asid->asid == 0 )
    {
        vmcb_set_asid(vmcb, true);
        vmcb->tlb_control =
            cpu_has_svm_flushbyasid ? TLB_CTRL_FLUSH_ASID : TLB_CTRL_FLUSH_ALL;
        return;
    }

    if ( vmcb_get_asid(vmcb) != p_asid->asid ) {
        vmcb_set_asid(vmcb, p_asid->asid);
        printk(XENLOG_INFO "assigned asid %u in this vcpu's vmcb field", p_asid->asid);
    }
    /* vmcb->tlb_control =
        !need_flush ? TLB_CTRL_NO_FLUSH :
        cpu_has_svm_flushbyasid ? TLB_CTRL_FLUSH_ASID : TLB_CTRL_FLUSH_ALL;*/
}

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
