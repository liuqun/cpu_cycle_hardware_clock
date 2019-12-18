/* C11 headers */
#include <errno.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "stdatomic.h"
//#include "memory.h"

/* POSIX headers */
#include <fcntl.h> // open()
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>

/* Linux headers (which is None-POSIX) */
#include <linux/types.h>

// Fake DPDK headers
#include "rte_config.h"
#include "rte_cycle.h"
//#include "rte_atomic.h"

#ifndef RTE_LOG
#include <stdio.h>
#define RTE_LOG(log_level_, log_which_mod_, ...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
    } while(0)
#endif



/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2017 Intel Corporation
 */

#include <fcntl.h>
#include <unistd.h>
#include <cpuid.h>

//#include <rte_common.h>

//#include "eal_private.h"

static unsigned int
rte_cpu_get_model(uint32_t fam_mod_step)
{
    uint32_t family, model, ext_model;

    family = (fam_mod_step >> 8) & 0xf;
    model = (fam_mod_step >> 4) & 0xf;

    if (family == 6 || family == 15) {
        ext_model = (fam_mod_step >> 16) & 0xf;
        model += (ext_model << 4);
    }

    return model;
}

static int32_t
rdmsr(int msr, uint64_t *val)
{
#ifdef RTE_EXEC_ENV_LINUX
    int fd;
    int ret;

    fd = open("/dev/cpu/0/msr", O_RDONLY);
    if (fd < 0)
        return fd;

    ret = (int)pread(fd, val, sizeof(uint64_t), msr);

    close(fd);

    return ret;
#else
    //#ifndef RTE_SET_USED // RTE_SET_USED() was defined in rte_common.h but here we define again...
    //#define RTE_SET_USED(x) (void)(x)
    //#endif
    RTE_SET_USED(msr);
    RTE_SET_USED(val);

    return -1;
#endif
}


static uint32_t
check_model_wsm_nhm(uint8_t model)
{
    switch (model) {
    /* Westmere */
    case 0x25:
    case 0x2C:
    case 0x2F:
    /* Nehalem */
    case 0x1E:
    case 0x1F:
    case 0x1A:
    case 0x2E:
        return 1;
    }

    return 0;
}

static uint32_t
check_model_gdm_dnv(uint8_t model)
{
    switch (model) {
    /* Goldmont */
    case 0x5C:
    /* Denverton */
    case 0x5F:
        return 1;
    }

    return 0;
}

uint64_t
get_tsc_freq_arch(void)
{
    uint64_t tsc_hz = 0;
    uint32_t a, b, c, d, maxleaf;
    uint8_t mult, model;
    int32_t ret;

    /*
     * Time Stamp Counter and Nominal Core Crystal Clock
     * Information Leaf
     */
    maxleaf = (uint32_t)__get_cpuid_max(0, NULL);

    if (maxleaf >= 0x15) {
        __cpuid(0x15, a, b, c, d);

        /* EBX : TSC/Crystal ratio, ECX : Crystal Hz */
        if (b && c)
            return c * (b / a);
    }

    __cpuid(0x1, a, b, c, d);
    model = (uint8_t)rte_cpu_get_model(a);

    if (check_model_wsm_nhm(model))
        mult = 133;
    else if ((c & bit_AVX) || check_model_gdm_dnv(model))
        mult = 100;
    else
        return 0;

    ret = rdmsr(0xCE, &tsc_hz);
    if (ret < 0)
        return 0;

    return ((tsc_hz >> 8) & 0xff) * mult * 1000*1000;
}

//static inline uint64_t rte_rdtsc(void);

#if defined(__x86_64__) && defined(__amd64__)

#elif defined(__aarch__) && defined(__ARM_ARCH) && (__ARM_ARCH >= 8)
    #warning "Later implement me please"
    static inline uint64_t __attribute((unused)) rte_rdtsc_arm_7_32bit(void)
    {
        return 0;
    }

    #define dsb(opt) asm volatile("dsb " #opt : : : "memory")
    #define dmb(opt) asm volatile("dmb " #opt : : : "memory")

    #define rte_mb() dsb(sy)
    #define rte_wmb() dsb(st)
    #define rte_rmb() dsb(ld)
    #define rte_smp_mb() dmb(ish)
    #define rte_smp_wmb() dmb(ishst)
    #define rte_smp_rmb() dmb(ishld)
    #define rte_io_mb() rte_mb()
    #define rte_io_wmb() rte_wmb()
    #define rte_io_rmb() rte_rmb()
    #define rte_cio_wmb() dmb(oshst)
    #define rte_cio_rmb() dmb(oshld)

#elif defined(__arm__) && defined(__ARM_ARCH) && (__ARM_ARCH == 7)
    #warning "Later implement me please"
    static inline uint64_t __attribute((unused)) rte_rdtsc_arm_8_64bit(void)
    {
        return 0;
    }
#endif

//#include <stdbool.h>
struct xfs__internal_config_ {
    _Bool no_hpet;
};

static struct xfs__internal_config_ internal_config = {
    .no_hpet = 0,
};

enum timer_source eal_timer_source = EAL_TIMER_HPET;

#define DEV_HPET "/dev/hpet"

/**
 * Macro to align a value to the multiple of given value. The resultant
 * value will be of the same type as the first parameter and will be no lower
 * than the first parameter.
 */
#define RTE_ALIGN_MUL_CEIL(v, mul) \
    (((v + (__typeof__(v))(mul) - 1) / ((__typeof__(v))(mul))) * (__typeof__(v))(mul))

/**
 * Macro to align a value to the multiple of given value. The resultant
 * value will be of the same type as the first parameter and will be no higher
 * than the first parameter.
 */
#define RTE_ALIGN_MUL_FLOOR(v, mul) \
    ((v / ((__typeof__(v))(mul))) * (__typeof__(v))(mul))

/**
 * Macro to align value to the nearest multiple of the given value.
 * The resultant value might be greater than or less than the first parameter
 * whichever difference is the lowest.
 */
#define RTE_ALIGN_MUL_NEAR(v, mul)                         \
        ({                                                 \
          __typeof__(v) ceil = RTE_ALIGN_MUL_CEIL(v, mul);   \
          __typeof__(v) floor = RTE_ALIGN_MUL_FLOOR(v, mul); \
          (ceil - v) > (v - floor) ? floor : ceil;         \
        })

/* Maximum number of counters. */
#define HPET_TIMER_NUM 3

/* General capabilities register */
#define CLK_PERIOD_SHIFT     32 /* Clock period shift. */
#define CLK_PERIOD_MASK      0xffffffff00000000ULL /* Clock period mask. */

/**
 * HPET timer registers. From the Intel IA-PC HPET (High Precision Event
 * Timers) Specification.
 */
struct xfs__eal_hpet_regs_ {
    /* Memory-mapped, software visible registers */
    uint64_t capabilities;      /**< RO General Capabilities Register. */
    uint64_t reserved0;         /**< Reserved for future use. */
    uint64_t config;            /**< RW General Configuration Register. */
    uint64_t reserved1;         /**< Reserved for future use. */
    uint64_t isr;               /**< RW Clear General Interrupt Status. */
    uint64_t reserved2[25];     /**< Reserved for future use. */
    union {
        uint64_t counter;   /**< RW Main Counter Value Register. */
        struct {
            uint32_t counter_l; /**< RW Main Counter Low. */
            uint32_t counter_h; /**< RW Main Counter High. */
        };
    };
    uint64_t reserved3;         /**< Reserved for future use. */
    struct {
        uint64_t config;    /**< RW Timer Config and Capability Reg. */
        uint64_t comp;      /**< RW Timer Comparator Value Register. */
        uint64_t fsb;       /**< RW FSB Interrupt Route Register. */
        uint64_t reserved4; /**< Reserved for future use. */
    } timers[HPET_TIMER_NUM]; /**< Set of HPET timers. */
};

/* Mmap'd hpet registers */
static volatile struct xfs__eal_hpet_regs_ *eal_hpet = NULL;

/* Period at which the HPET counter increments in
 * femtoseconds (10^-15 seconds). */
static uint32_t eal_hpet_resolution_fs = 0;

/* Frequency of the HPET counter in Hz */
static uint64_t eal_hpet_resolution_hz = 0;

/* Incremented 4 times during one 32bits hpet full count */
static uint32_t eal_hpet_msb;

static pthread_t msb_inc_thread_id;

/*
 * This function runs on a specific thread to update a global variable
 * containing used to process MSB of the HPET (unfortunately, we need
 * this because hpet is 32 bits by default under linux).
 */
static void *
hpet_msb_inc(__attribute__((unused)) void *arg)
{
    uint32_t t;

    while (1) {
        t = (eal_hpet->counter_l >> 30);
        if (t != (eal_hpet_msb & 3))
            eal_hpet_msb ++;
        sleep(10);
    }
    return NULL;
}

uint64_t
rte_get_hpet_hz(void)
{
    if(internal_config.no_hpet) {
        RTE_LOG(ERR, EAL, "Error, HPET called, but no HPET present\n");
        //rte_panic("Error, HPET called, but no HPET present\n");
        exit(EXIT_FAILURE);
    }
    return eal_hpet_resolution_hz;
}

uint64_t
rte_get_hpet_cycles(void)
{
    uint32_t t, msb;
    uint64_t ret;

    if(internal_config.no_hpet) {
        RTE_LOG(ERR, EAL, "Error, HPET called, but no HPET present\n");
        //rte_panic("Error, HPET called, but no HPET present\n");
    }
    t = eal_hpet->counter_l;
    msb = eal_hpet_msb;
    ret = (msb + 2 - (t >> 30)) / 4;
    ret <<= 32;
    ret += t;
    return ret;
}

/*
 * Open and mmap /dev/hpet (high precision event timer) that will
 * provide our time reference.
 */
int
rte_eal_hpet_init(int make_default)
{
    int fd, ret;

    if (internal_config.no_hpet) {
        RTE_LOG(NOTICE, EAL, "HPET is disabled\n");
        return -1;
    }

    fd = open(DEV_HPET, O_RDONLY);
    if (fd < 0) {
        RTE_LOG(ERR, EAL, "ERROR: Cannot open "DEV_HPET": %s!\n",
            strerror(errno));
        internal_config.no_hpet = 1;
        return -1;
    }
    eal_hpet = mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
    if (eal_hpet == MAP_FAILED) {
        RTE_LOG(ERR, EAL, "ERROR: Cannot mmap "DEV_HPET"!\n"
                "Please enable CONFIG_HPET_MMAP in your kernel configuration "
                "to allow HPET support.\n"
                "To run without using HPET, set CONFIG_RTE_LIBEAL_USE_HPET=n "
                "in your build configuration or use '--no-hpet' EAL flag.\n");
        close(fd);
        internal_config.no_hpet = 1;
        return -1;
    }
    close(fd);

    eal_hpet_resolution_fs = (uint32_t)((eal_hpet->capabilities &
                    CLK_PERIOD_MASK) >>
                    CLK_PERIOD_SHIFT);

    eal_hpet_resolution_hz = (1000ULL*1000ULL*1000ULL*1000ULL*1000ULL) /
        (uint64_t)eal_hpet_resolution_fs;

    RTE_LOG(INFO, EAL, "HPET frequency is ~%"PRIu64" kHz\n",
            eal_hpet_resolution_hz/1000);

    eal_hpet_msb = (eal_hpet->counter_l >> 30);

    /* create a thread that will increment a global variable for
     * msb (hpet is 32 bits by default under linux) */
    ret = pthread_create(&msb_inc_thread_id, NULL,
                     hpet_msb_inc, NULL);
    if (ret != 0) {
        RTE_LOG(ERR, EAL, "ERROR: Cannot create HPET timer thread!\n");
        internal_config.no_hpet = 1;
        return -1;
    }

    if (make_default)
        eal_timer_source = EAL_TIMER_HPET;
    return 0;
}

static void
check_tsc_flags(void)
{
    char line[512];
    FILE *stream;

    stream = fopen("/proc/cpuinfo", "r");
    if (!stream) {
        RTE_LOG(WARNING, EAL, "WARNING: Unable to open /proc/cpuinfo\n");
        return;
    }

    while (fgets(line, sizeof line, stream)) {
        char *constant_tsc;
        char *nonstop_tsc;

        if (strncmp(line, "flags", 5) != 0)
            continue;

        constant_tsc = strstr(line, "constant_tsc");
        nonstop_tsc = strstr(line, "nonstop_tsc");
        if (!constant_tsc || !nonstop_tsc)
            RTE_LOG(WARNING, EAL,
                "WARNING: cpu flags "
                "constant_tsc=%s "
                "nonstop_tsc=%s "
                "-> using unreliable clock cycles !\n",
                constant_tsc ? "yes":"no",
                nonstop_tsc ? "yes":"no");
        break;
    }

    fclose(stream);
}


/**
 * Set TSC frequency from precise value or estimation
 *
 * This function is private to the EAL.
 */
void set_tsc_freq(void);//#include "eal_private.h"

/**
 * Get precise TSC frequency from system
 *
 * This function is private to the EAL.
 */
uint64_t get_tsc_freq(void);//#include "eal_private.h"

/**
 * Get TSC frequency if the architecture supports.
 *
 * This function is private to the EAL.
 *
 * @return
 *   The number of TSC cycles in one second.
 *   Returns zero if the architecture support is not available.
 */
uint64_t get_tsc_freq_arch(void);//#include "eal_private.h"



/* The frequency of the RDTSC timer resolution */
static uint64_t eal_tsc_resolution_hz;

/* Pointer to user delay function */
extern void (*rte_delay_us)(unsigned int);
void (*rte_delay_us)(unsigned int) = NULL;

void
rte_delay_us_block(unsigned int us)
{
    (void) us;
//	const uint64_t start = rte_get_timer_cycles();
//	const uint64_t ticks = (uint64_t)us * rte_get_timer_hz() / 1E6;
//	while ((rte_get_timer_cycles() - start) < ticks)
//		rte_pause();
}

void
rte_delay_us_sleep(unsigned int us)
{
    struct timespec wait[2];
    int ind = 0;

    wait[0].tv_sec = 0;
    if (us >= US_PER_S) {
        wait[0].tv_sec = us / US_PER_S;
        us -= wait[0].tv_sec * US_PER_S;
    }
    wait[0].tv_nsec = 1000 * us;

    while (nanosleep(&wait[ind], &wait[1 - ind]) && errno == EINTR) {
        /*
         * Sleep was interrupted. Flip the index, so the 'remainder'
         * will become the 'request' for a next call.
         */
        ind = 1 - ind;
    }
}

uint64_t
rte_get_tsc_hz(void)
{
    return eal_tsc_resolution_hz;
}

static uint64_t
estimate_tsc_freq(void)
{
    RTE_LOG(WARNING, EAL, "WARNING: TSC frequency estimated roughly"
        " - clock timings may be less accurate.\n");
    /* assume that the sleep(1) will sleep for 1 second */
    uint64_t start = rte_rdtsc();
    sleep(1);
    /* Round up to 10Mhz. 1E7 ~ 10Mhz */
    const uint64_t CYC_PER_10MHZ = 10*1000*1000ULL;
    return RTE_ALIGN_MUL_NEAR(rte_rdtsc() - start, CYC_PER_10MHZ);
}

void
set_tsc_freq(void)
{
    uint64_t freq;

    freq = get_tsc_freq_arch();
    if (!freq)
        freq = get_tsc_freq();
    if (!freq)
        freq = estimate_tsc_freq();

    RTE_LOG(DEBUG, EAL, "TSC frequency is ~%" PRIu64 " KHz\n", freq / 1000);
    eal_tsc_resolution_hz = freq;
}

void rte_delay_us_callback_register(void (*userfunc)(unsigned int))
{
    rte_delay_us = userfunc;
}

#if 0
#define RTE_INIT(func) \
static void __attribute__((constructor, used)) func(void)
#endif

static void __attribute__((constructor, used)) rte_timer_init(void)
{
    /* set rte_delay_us_block as a delay function */
    rte_delay_us_callback_register(rte_delay_us_block);
}

#ifndef CLOCK_MONOTONIC_RAW
uint64_t
get_tsc_freq(void)
{
    return 0;
}
#else // defined(CLOCK_MONOTONIC_RAW)
uint64_t
get_tsc_freq(void)
{
    const uint64_t NS_PER_SEC = 1000*1000*1000ULL;
    struct timespec sleeptime = {.tv_nsec = NS_PER_SEC / 10 }; /* 1/10 second */

    struct timespec t_start, t_end;
    uint64_t tsc_hz;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &t_start) == 0) {
        uint64_t ns=0, end=0; uint64_t start = rte_rdtsc();
        nanosleep(&sleeptime,NULL);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
        end = rte_rdtsc();
        ns = ((uint64_t)(t_end.tv_sec - t_start.tv_sec) * NS_PER_SEC);
        ns += (uint64_t)(t_end.tv_nsec - t_start.tv_nsec);

        double secs = (double)ns/NS_PER_SEC;
        tsc_hz = (uint64_t)((end - start)/secs);
        /* Round up to 10Mhz. 1E7 ~ 10Mhz */
        const uint64_t CYC_PER_10MHZ = 10*1000*1000ULL;
        uint64_t result = RTE_ALIGN_MUL_NEAR(tsc_hz, CYC_PER_10MHZ);
        return result;
    }
    return 0;
}
#endif // defined(CLOCK_MONOTONIC_RAW)


int
rte_eal_timer_init(void)
{

    eal_timer_source = EAL_TIMER_TSC;

    set_tsc_freq();
    check_tsc_flags();
    return 0;
}


#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L
#error "Please try gcc -std=gnu11 (or gcc -std=c11)!"
#endif



/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no
 * bigger than the first parameter. Second parameter must be a
 * power-of-two value.
 */
#if 0
#define RTE_ALIGN_FLOOR(val, align) \
    (typeof(val))((val) & (~((typeof(val))((align) - 1))))
#endif // if 0

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 */
#if 0
#define RTE_PTR_ALIGN_CEIL(ptr, align) \
    RTE_PTR_ALIGN_FLOOR((typeof(ptr))RTE_PTR_ADD(ptr, (align) - 1), align)
#endif // if 0

/**
 * Macro to align a value to a given power-of-two. The resultant value
 * will be of the same type as the first parameter, and will be no lower
 * than the first parameter. Second parameter must be a power-of-two
 * value.
 */
#if 0
#define RTE_ALIGN_CEIL(val, align) \
    RTE_ALIGN_FLOOR(((val) + ((typeof(val)) (align) - 1)), align)
#endif // if 0

/**
 * Macro to align a pointer to a given power-of-two. The resultant
 * pointer will be a pointer of the same type as the first parameter, and
 * point to an address no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RTE_PTR_ALIGN_CEIL
 */
#if 0
#define RTE_PTR_ALIGN(ptr, align) RTE_PTR_ALIGN_CEIL(ptr, align)
#endif // if 0

/**
 * Macro to align a value to a given power-of-two. The resultant
 * value will be of the same type as the first parameter, and
 * will be no lower than the first parameter. Second parameter
 * must be a power-of-two value.
 * This function is the same as RTE_ALIGN_CEIL
 */
#if 0
#define RTE_ALIGN(val, align) RTE_ALIGN_CEIL(val, align)
#endif // if 0


