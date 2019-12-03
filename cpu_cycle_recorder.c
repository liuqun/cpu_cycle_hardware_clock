#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include "cpu_cycle_recorder.h"

#if defined(__x86_64__) || defined(__i386__)
# include <asm/unistd.h>
#elif defined(__aarch64__) && (defined(__ARM_ARCH) && __ARM_ARCH==8) && defined(__ARM_64BIT_STATE)
# error "TODO: Will support ARM 32/64bit CPU, but later..."
#elif defined(__arm__) && (defined(__ARM_ARCH) && __ARM_ARCH<=7) && defined(__ARM_32BIT_STATE)
# error "TODO: Will support ARM 32/64bit CPU, but later..."
#endif // End of gcc cross compiler check...


/**
 * @brief reset_cpu_cycle_recorder
 */
void reset_cpu_cycle_recorder(cpu_cycle_recorder_fd_t recorder_fd)
{
    ioctl(recorder_fd, PERF_EVENT_IOC_RESET, 0);
}

/**
 * @brief start_cpu_cycle_recorder
 */
void start_cpu_cycle_recorder(cpu_cycle_recorder_fd_t recorder_fd)
{
    ioctl(recorder_fd, PERF_EVENT_IOC_ENABLE, 0);
}

/**
 * @brief pause_cpu_cycle_recorder
 */
void pause_cpu_cycle_recorder(cpu_cycle_recorder_fd_t recorder_fd)
{
    ioctl(recorder_fd, PERF_EVENT_IOC_DISABLE, 0);
}

/**
 * @brief open_cpu_cycle_recorder
 */
cpu_cycle_recorder_fd_t open_cpu_cycle_recorder(int cpu_id)
{
    int recorder_fd = -1;
    pid_t pid = 0; // default process id 0 for self
    const int event_group_fd_none = -1;
    unsigned long flags = 0;

    struct perf_event_attr attr;

    memset(&attr, 0x00, sizeof(struct perf_event_attr));
    attr.type = PERF_TYPE_HARDWARE;
    attr.size = sizeof(struct perf_event_attr);
    attr.config = PERF_COUNT_HW_CPU_CYCLES;
    attr.disabled = 1;
    attr.inherit = 1; // count children
    attr.exclude_kernel = 0;
    attr.exclude_hv = 1;// exclude hypervisor
    attr.enable_on_exec = 1; // auto enable on exec
    flags |= PERF_FLAG_FD_CLOEXEC;//since Linux 3.14
    if (cpu_id < 0) {
        cpu_id = CPU_ID_ANY;
    }
    recorder_fd = (int) syscall(
                __NR_perf_event_open,/* perf_event_open() */
                &attr, pid, cpu_id, event_group_fd_none, flags);
    if (recorder_fd < 0) {
        return -1;
    }
    ioctl(recorder_fd, PERF_EVENT_IOC_RESET, 0);
    return ((cpu_cycle_recorder_fd_t)recorder_fd);
}

/**
 * @brief open_cpu_cycle_recorder
 */
void close_cpu_cycle_recorder(cpu_cycle_recorder_fd_t recorder_fd)
{
    if (recorder_fd < 0) {
        return;
    }
    close((int)recorder_fd);
}
