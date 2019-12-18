#pragma once

#include <stdint.h>
#include <unistd.h>
#include <rte_config.h>
//#include "rte_cycle.h"

#if defined(__x86_64__) ||  defined(__amd64__)


#endif // defined(__x86_64__) ||  defined(__amd64__)


#ifdef RTE_LIBRTE_EAL_VMWARE_TSC_MAP_SUPPORT
/* Global switch to use VMWARE mapping of TSC instead of RDTSC */
extern int rte_cycles_vmware_tsc_map;
#include <rte_branch_prediction.h>
#endif

static inline uint64_t rte_rdtsc_intel__x86_64bit(void)
{
    union {
        uint64_t tsc_64;
        struct {
            uint32_t lo_32;
            uint32_t hi_32;
        };
    } tsc;

#ifdef RTE_LIBRTE_EAL_VMWARE_TSC_MAP_SUPPORT
    if (unlikely(rte_cycles_vmware_tsc_map)) {
        /* ecx = 0x10000 corresponds to the physical TSC for VMware */
        asm volatile("rdpmc" :
                     "=a" (tsc.lo_32),
                     "=d" (tsc.hi_32) :
                     "c"(0x10000));
        return tsc.tsc_64;
    }
#endif

    __asm volatile("rdtsc" :
                 "=a" (tsc.lo_32),
                 "=d" (tsc.hi_32));
    return tsc.tsc_64;
}

/* Intel standard headers (Warning: None-portable to ARM arch) */
#include <immintrin.h>
#define	rte_mb_intel__x86_64bit() _mm_mfence() // General memory barrier, see gcc header file emmintrin.h ---> /usr/lib/gcc/x86_64-linux-gnu/7/include/emmintrin.h
#define	rte_rmb_intel__x86_64bit() _mm_lfence() // emmintrin.h
#define	rte_wmb_intel__x86_64bit() _mm_sfence() // xmmintrin.h

static inline uint64_t rte_rdtsc_precise__x86_64bit(void)
{
    rte_mb_intel__x86_64bit();
    return rte_rdtsc_intel__x86_64bit();
}


static inline uint64_t rte_get_tsc_cycles__x86_64bit(void)
{
    return rte_rdtsc_intel__x86_64bit();
}




    __attribute((unused))
    static inline uint64_t rte_rdtsc(void)
    {
        return rte_rdtsc_intel__x86_64bit();
    }

    __attribute((unused))
    static inline uint64_t rte_rdtsc_precise(void)
    {
        return rte_rdtsc_precise__x86_64bit();
    }

    __attribute((unused))
    static inline uint64_t rte_get_tsc_cycles(void)
    {
        return rte_get_tsc_cycles__x86_64bit();
    }



