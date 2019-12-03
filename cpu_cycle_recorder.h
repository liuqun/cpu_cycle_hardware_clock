#pragma once
#include <unistd.h>

/**
 * data type cpu_cycle_recorder_fd_t
 */
typedef int cpu_cycle_recorder_fd_t;

#ifdef __cplusplus
extern "C" {
#endif

#define CPU_ID_ANY -1
/**
 * @brief 函数open_cpu_cycle_recorder()
 * @param cpu_id==-1 => any CPU; 0 ~ 2^N-1 => cpu_id
 * @return
 */
cpu_cycle_recorder_fd_t open_cpu_cycle_recorder(int cpu_id);

/**
 * @brief 函数close_cpu_cycle_recorder()
 */
void close_cpu_cycle_recorder(cpu_cycle_recorder_fd_t);


/**
 * @name start_cpu_cycle_recorder
 * @brief 函数start_cpu_cycle_recorder(): 启动CPU运行计数器
 *
 * @name pause_cpu_cycle_recorder
 * @brief 函数pause_cpu_cycle_recorder(): 暂停
 *
 * @name reset_cpu_cycle_recorder
 * @brief 函数reset_cpu_cycle_recorder(): 清零重置CPU运行计数器
 */
void start_cpu_cycle_recorder(cpu_cycle_recorder_fd_t);
void pause_cpu_cycle_recorder(cpu_cycle_recorder_fd_t);
void reset_cpu_cycle_recorder(cpu_cycle_recorder_fd_t);


/**
 * @brief 函数read_cpu_cycles()
 * @return 返回值为int64整数, 当前CPU运行计数器
 */
static inline __attribute__((unused))
long long read_cpu_cycles(cpu_cycle_recorder_fd_t recorder_fd)
{
    long long result = 0;
    if (read((int)recorder_fd, &result, sizeof(result)) < (int)sizeof(long long)) {
        return 0;
    }
    return result;
}

#ifdef __cplusplus
}
#endif
