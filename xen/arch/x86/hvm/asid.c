/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * asid.c: ASID management
 * Copyright (c) 2007, Advanced Micro Devices, Inc.
 * Copyright (c) 2009, Citrix Systems, Inc.
 */

#include <xen/init.h>
#include <xen/lib.h>
#include <xen/param.h>
#include <xen/sched.h>
#include <xen/smp.h>
#include <xen/percpu.h>
#include <asm/hvm/asid.h>

/* Xen command-line option to enable ASIDs */
static bool __read_mostly opt_asid_enabled = true;
boolean_param("asid", opt_asid_enabled);

/*
 * ASIDs partition the physical TLB.  In the current implementation ASIDs are
 * introduced to reduce the number of TLB flushes.  Each time the guest's
 * virtual address space changes (e.g. due to an INVLPG, MOV-TO-{CR3, CR4}
 * operation), instead of flushing the TLB, a new ASID is assigned.  This
 * reduces the number of TLB flushes to at most 1/#ASIDs.  The biggest
 * advantage is that hot parts of the hypervisor's code and data retain in
 * the TLB.
 *
 * Sketch of the Implementation:
 * TODO(vaishali): Update this comment
 * ASIDs are Xen-wide resource.  As preemption of ASIDs is not possible,
 * ASIDs are assigned in a round-robin scheme.  To minimize the overhead of
 * ASID invalidation, at the time of a TLB flush,  ASIDs are tagged with a
 * 64-bit generation.  Only on a generation overflow the code needs to
 * invalidate all ASID information stored at the VCPUs with are run on the
 * specific physical processor.  This overflow appears after about 2^80
 * host processor cycles, so we do not optimize this case, but simply disable
 * ASID useage to retain correctness.
 */

/* Xen-wide ASID management */
struct hvm_asid_data {
   uint32_t next_asid;
   uint32_t max_asid;
   uint32_t min_asid;
   bool disabled;
};

static struct hvm_asid_data asid_data = {
    .next_asid = 1 // We start the count at 1 because ASID 0 is reserved
};

void hvm_asid_init(int nasids)
{
    static int8_t g_disabled = -1;
    struct hvm_asid_data *data = &asid_data;

    data->max_asid = nasids - 1;
    data->disabled = !opt_asid_enabled || (nasids <= 1);

    if ( g_disabled != data->disabled )
    {
        printk("HVM: ASIDs %sabled.\n", data->disabled ? "dis" : "en");
        if ( g_disabled < 0 )
            g_disabled = data->disabled;
    }
}

void hvm_asid_flush_domain_asid(struct hvm_domain_asid *asid)
{
    write_atomic(&asid->generation, 0);
}

void hvm_asid_flush_domain(struct domain *d)
{
    hvm_asid_flush_domain_asid(&d->arch.hvm.n1asid);
    hvm_asid_flush_domain_asid(&d->arch.hvm.nv_n2asid);
}

/* This function is called only when first vmenter happens after creating a new domain */
bool hvm_asid_domain_create(struct hvm_domain_asid *asid)
{
    struct hvm_asid_data *data = &asid_data;

    /* On erratum #170 systems we must flush the TLB. 
     * Generation overruns are taken here, too. */
    if ( data->disabled )
        goto disabled;

    /* TODO(vaishali): Test if domain has valid ASID. */

    /* If there are no free ASIDs, check in the reclaimable pool */
    if ( unlikely(data->next_asid > data->max_asid) )
    {
        // TODO(vaishali): Add a check to pick the asid from the reclaimable asids if any
        data->next_asid = 1;
        if ( data->disabled )
            goto disabled;
    }

    /* Now guaranteed to be a free ASID. Only assign a new asid if the ASID is 1 */
    if (asid->asid == 1)
    {
        asid->asid = data->next_asid++;
    }

    /*
     * When we assign ASID 1, flush all TLB entries as we are starting a new
     * generation, and all old ASID allocations are now stale. 
     */
    return (asid->asid == 1);

 disabled:
    asid->asid = 0;
    return 0;
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
