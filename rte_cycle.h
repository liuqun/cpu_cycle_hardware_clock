#pragma once

#include <stdint.h>
#include "rte_config.h"
#ifndef RTE_LOG
#include <stdio.h>
#define RTE_LOG(log_level_, log_which_mod_, ...) \
    do { \
        fprintf(stderr, __VA_ARGS__); \
    } while(0)
#endif


#define MS_PER_S 1000
#define US_PER_S 1000000
#define NS_PER_S 1000000000

enum timer_source {
    EAL_TIMER_TSC = 0,
    EAL_TIMER_HPET
};
extern enum timer_source eal_timer_source;



#if defined(__x86_64__) && defined(__amd64__)
#include "rte_cycle_x86_64.h"
#endif // defined(__x86_64__) && defined(__amd64__)


#ifdef __cplusplus
extern "C" {
#endif








/**
 * Get the measured frequency of the RDTSC counter
 *
 * @return
 *   The TSC frequency for this lcore
 */
uint64_t
rte_get_tsc_hz(void);

/**
 * Return the number of TSC cycles since boot
 *
  * @return
 *   the number of cycles
 */
static inline uint64_t
rte_get_tsc_cycles(void);

/**
 * Return the number of HPET cycles since boot
 *
 * This counter is global for all execution units. The number of
 * cycles in one second can be retrieved using rte_get_hpet_hz().
 *
 * @return
 *   the number of cycles
 */
uint64_t
rte_get_hpet_cycles(void);

/**
 * Get the number of HPET cycles in one second.
 *
 * @return
 *   The number of cycles in one second.
 */
uint64_t
rte_get_hpet_hz(void);

/**
 * Initialise the HPET for use. This must be called before the rte_get_hpet_hz
 * and rte_get_hpet_cycles APIs are called. If this function does not succeed,
 * then the HPET functions are unavailable and should not be called.
 *
 * @param make_default
 *	If set, the hpet timer becomes the default timer whose values are
 *	returned by the rte_get_timer_hz/cycles API calls
 *
 * @return
 *	0 on success,
 *	-1 on error, and the make_default parameter is ignored.
 */
int rte_eal_hpet_init(int make_default);

/**
 * Get the number of cycles since boot from the default timer.
 *
 * @return
 *   The number of cycles
 */
__attribute__((unused))
static inline uint64_t
rte_get_timer_cycles(void)
{
    switch(eal_timer_source) {
    case EAL_TIMER_TSC:
        return rte_get_tsc_cycles();
    case EAL_TIMER_HPET:
        return rte_get_hpet_cycles();
    }
    exit(EXIT_FAILURE);//rte_panic("Invalid timer source specified\n");
}

/**
 * Get the number of cycles in one second for the default timer.
 *
 * @return
 *   The number of cycles in one second.
 */
__attribute__((unused))
static inline uint64_t
rte_get_timer_hz(void)
{
    switch(eal_timer_source) {
    case EAL_TIMER_TSC:
        return rte_get_tsc_hz();
    case EAL_TIMER_HPET:
        return rte_get_hpet_hz();
    }

    RTE_LOG(ERR, EAL, "rte_panic(\"Invalid timer source specified\n\");");
    exit(EXIT_FAILURE);//rte_panic("Invalid timer source specified\n");
}
/**
 * Wait at least us microseconds.
 * This function can be replaced with user-defined function.
 * @see rte_delay_us_callback_register
 *
 * @param us
 *   The number of microseconds to wait.
 */
extern void
(*rte_delay_us)(unsigned int us);

/**
 * Wait at least ms milliseconds.
 *
 * @param ms
 *   The number of milliseconds to wait.
 */
__attribute__((unused))
static inline void
rte_delay_ms(unsigned ms)
{
    rte_delay_us(ms * 1000);
}

/**
 * Blocking delay function.
 *
 * @param us
 *   Number of microseconds to wait.
 */
void rte_delay_us_block(unsigned int us);

/**
 * Delay function that uses system sleep.
 * Does not block the CPU core.
 *
 * @param us
 *   Number of microseconds to wait.
 */
//__rte_experimental
void
rte_delay_us_sleep(unsigned int us);

/**
 * Replace rte_delay_us with user defined function.
 *
 * @param userfunc
 *   User function which replaces rte_delay_us. rte_delay_us_block restores
 *   builtin block delay function.
 */
void rte_delay_us_callback_register(void(*userfunc)(unsigned int));

#ifdef __cplusplus
}
#endif
