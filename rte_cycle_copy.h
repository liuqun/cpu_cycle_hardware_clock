#pragma once

#define MS_PER_S 1000
#define US_PER_S 1000000
#define NS_PER_S 1000000000

enum timer_source {
    EAL_TIMER_TSC = 0,
    EAL_TIMER_HPET
};
extern enum timer_source eal_timer_source;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
