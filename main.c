#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu_cycle_recorder.h"


int main ()
{
    int i;
    int N=100;
    double factorial = 1.0;
    long long before;
    long long after;
    cpu_cycle_recorder_fd_t fd;

    fd = open_cpu_cycle_recorder(CPU_ID_ANY);
    reset_cpu_cycle_recorder(fd);

    before = read_cpu_cycles(fd);
    start_cpu_cycle_recorder(fd);
    factorial = 1.0;
    for (i=2; i<=N; i++) {
        factorial *= (double)i;
    }
    pause_cpu_cycle_recorder(fd);
    after = read_cpu_cycles(fd);

    printf("factorial(%d) = %g\n", N, factorial);
    printf("before = %lld\n",before);
    printf("after = %lld\n", after);
    printf("after - before = %lld\n", after - before);
    printf("\n");

    //reset_cpu_cycle_recorder(fd);

    long long before2;
    long long after2;

    before2 = read_cpu_cycles(fd);
    start_cpu_cycle_recorder(fd);
    factorial = 1.0;
    for (i=2; i<=N; i++) {
        factorial *= (double)i;
    }
    pause_cpu_cycle_recorder(fd);
    after2 = read_cpu_cycles(fd);

    printf("before2 = %lld\n",before2);
    printf("after2 = %lld\n", after2);
    printf("after2 - before2 = %lld\n", after2 - before2);
    printf("factorial(%d) = %g\n", N, factorial);

    close_cpu_cycle_recorder(fd);
    return 0;
}

//#if 0
//void example_test2()
//{
//    struct perf_event_attr attr;
//    memset(&attr, 0x00, sizeof(struct perf_event_attr));
//    attr.disabled = 0;
//    attr.type = PERF_TYPE_HARDWARE;
//    attr.exclude_hv = 1;// exclude hypervisor
//    attr.size = sizeof(struct perf_event_attr);
//    attr.enable_on_exec = 1; // auto enable on exec
//    attr.inherit = 1; // count children
//    attr.exclude_kernel = 0;//FIXME
//
//    attr.config = PERF_COUNT_HW_CPU_CYCLES;
//
//    pid_t workerpid = 0;
//
//    workerpid = getpid();
//
//    long long hw_cpu_cycles = 0;
//    int fd = -1;
//
//    // 需要手动syscall调用 Linux 库函数 perf_event_open(), (glibc 暂时没有提供API函数封装)
//    fd = (int) syscall(__NR_perf_event_open, &attr, workerpid, CPU_ID_ANY, -1, 0);
//    if (fd < 0) {
//        perror("ERROR: perf_event_open() failed!");
//        exit(EXIT_FAILURE);
//    }
//    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
//    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
//
//    int i;
//    double factorial = 1.0;
//    for (i=2; i<=10; i++) {
//        factorial *= (double)i;
//    }
//
//    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
//    read(fd, &hw_cpu_cycles, sizeof(long long));
//
//    printf("sum = %g\n", (double)sum);
//    printf("Used %lld CPU cycles\n", hw_cpu_cycles);
//
//    close(fd);
//}
//#endif
