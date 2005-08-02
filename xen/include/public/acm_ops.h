/******************************************************************************
 * acm_ops.h
 *
 * Copyright (C) 2005 IBM Corporation
 *
 * Author:
 * Reiner Sailer <sailer@watson.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * Process acm policy command requests from guest OS.
 * access checked by policy; not restricted to DOM0
 *
 */

#ifndef __XEN_PUBLIC_ACM_OPS_H__
#define __XEN_PUBLIC_ACM_OPS_H__

#include "xen.h"
#include "sched_ctl.h"

/*
 * Make sure you increment the interface version whenever you modify this file!
 * This makes sure that old versions of acm tools will stop working in a
 * well-defined way (rather than crashing the machine, for instance).
 */
#define ACM_INTERFACE_VERSION   0xAAAA0003

/************************************************************************/

#define ACM_SETPOLICY        	4
typedef struct acm_setpolicy {
    /* OUT variables */
    void *pushcache;
    u16 pushcache_size;
} acm_setpolicy_t;


#define ACM_GETPOLICY        	5
typedef struct acm_getpolicy {
    /* OUT variables */
    void *pullcache;
    u16 pullcache_size;
} acm_getpolicy_t;

#define ACM_DUMPSTATS        	6
typedef struct acm_dumpstats {
    void *pullcache;
    u16 pullcache_size;
} acm_dumpstats_t;


typedef struct acm_op {
    u32 cmd;
    u32 interface_version;      /* ACM_INTERFACE_VERSION */
    union {
        acm_setpolicy_t setpolicy;
        acm_getpolicy_t getpolicy;
        acm_dumpstats_t dumpstats;
    } u;
} acm_op_t;

#endif                          /* __XEN_PUBLIC_ACM_OPS_H__ */
