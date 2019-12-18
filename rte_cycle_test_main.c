#include <stdio.h>
#include "rte_cycle.h"

extern int rte_eal_timer_init(void);

int main(void)
{
    uint64_t delta_tsc= 0;
    uint64_t delta_hpet = 0;
    uint64_t before;
    uint64_t after;
    uint64_t hz_tsc = 0;
    uint64_t hz_hpet = 0;

    printf("===========\n"
           "TSC 时钟测试\n");
    rte_eal_timer_init();

    //usleep(1000*1000);

    before = rte_get_tsc_cycles();
    usleep(1000);
    after = rte_get_tsc_cycles();
    delta_tsc = after - before;
    printf("CPU Cycles: delta_tsc = after - before = %llu\n", (unsigned long long) delta_tsc);


    printf("===================\n"
           "HPET 时钟测试:\n");
    if (rte_eal_hpet_init(1) < 0) {
        fprintf(stderr, "Warning: HPET can not be enabled, using TSC as default timer...\n");
    }
    before = rte_get_hpet_cycles();
    usleep(1000);
    after = rte_get_hpet_cycles();
    delta_hpet = after - before;
    printf("CPU Cycles: delta_hpet = after - before = %llu\n", (unsigned long long) delta_hpet);


    printf("===================\n");
    hz_tsc = rte_get_tsc_hz();
    hz_hpet = rte_get_timer_hz();
    printf("TSC 时钟频率 = %10llu Hz\n", (unsigned long long) hz_tsc);
    printf("HPET时钟频率 = %10llu Hz\n", (unsigned long long) hz_hpet);

    return 0;
}
