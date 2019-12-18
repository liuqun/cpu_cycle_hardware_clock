#include "rte_cycle.h"

int main(void)
{
    if (rte_eal_hpet_init(1) < 0) {
        RTE_LOG(INFO, APP, "HPET is not enabled, using TSC as default timer\n");
    }
    return 0;
}
