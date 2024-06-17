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

// static unsigned long *hvm_asid_bitmap;
// static unsigned long *hvm_reclaim_asid_bitmap;

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
 * TODO(vaishali): Add a comment about the new TLB flushing scheme here
 */

/* Xen-wide ASID management */
struct hvm_asid_data {
   uint32_t next_asid;
   uint32_t max_asid;
   uint32_t min_asid;
   bool disabled;
};

static struct hvm_asid_data asid_data = {
    .next_asid = 1 /* We start the count at 1 because ASID 0 is reserved */
};

void hvm_asid_init(int nasids)
{
    static int8_t g_disabled = -1;
    struct hvm_asid_data *data = &asid_data;

    data->max_asid = nasids - 1;
    data->disabled = !opt_asid_enabled || (nasids <= 1);

    /* hvm_asid_bitmap = xzalloc_array(unsigned long,
                                    BITS_TO_LONGS(data->max_asid));

    if ( !hvm_asid_bitmap )
        return -ENOMEM;*/

    if ( g_disabled != data->disabled )
    {
        printk("HVM: ASIDs %sabled.\n", data->disabled ? "dis" : "en");
        if ( g_disabled < 0 )
            g_disabled = data->disabled;
    }

    data->next_asid = 1;
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

/* We still allow flushing on vcpu level for non-SEV domain */
void hvm_asid_flush_vcpu(struct vcpu *v)
{
    hvm_asid_flush_domain_asid(&v->domain->arch.hvm.n1asid);
    hvm_asid_flush_domain_asid(&v->domain->arch.hvm.nv_n2asid);
}

/* This function is called while creating a new domain */
bool hvm_asid_domain_create(struct hvm_domain_asid *asid)
{
    struct hvm_asid_data *data = &asid_data;

    /* On erratum #170 systems we must flush the TLB. 
     * Generation overruns are taken here, too. */
    if ( data->disabled )
        goto disabled;

    /* If there are no free ASIDs, check in the reclaimable pool */
    if ( unlikely(data->next_asid > data->max_asid) )
    {
        // TODO(vaishali): Add a check to pick the asid from the reclaimable asids if any
        data->next_asid = 1;
        if ( data->disabled )
            goto disabled;
    }

    printk(XENLOG_INFO "Current asid before assigning the new one is %u" , asid->asid);
    /* Assign a new asid */
    asid->asid = data->next_asid++;
    printk(XENLOG_INFO "assign asid %u" , asid->asid);

    return 0;

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
