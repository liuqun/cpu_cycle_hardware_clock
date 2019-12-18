/* 例子 */
#include <pthread.h>
#include <time.h>
#include <signal.h>

void *my_thrd_routine(void *arg)
{
    pthread_t self_thrd_id = pthread_self();
    clockid_t self_clk_id;
    int err = 0;
    struct timespec tp1;
    struct timespec tp2;

    (void) arg;

    err =  pthread_getcpuclockid(self_thrd_id, &self_clk_id);
    if (err) {
        return NULL;
    }


    clock_gettime(self_clk_id, &tp1);
    for (int i=1; i<=100; i++) {
        static double floatnum = 1.0;
        floatnum *= (double) i;
    }
    clock_gettime(self_clk_id, &tp2);

    return NULL;
}

#include <string.h>
#include <stdio.h>

#include <syscall.h>
int main(void)
{

syscall(SYS_clock_gettime, CLOCK_MONOTONIC_RAW, &monotonic_time);

//struct itimerspec it = {{0,0},{0,0},};
//it.it_interval.tv_sec = 0;
//it.it_interval.tv_nsec = 1000;
//it.it_value.tv_sec = 0;
//it.it_value.tv_nsec = 0;

    timer_t timerid = 0;
    sigevent_t event;
    event.sigev_value.sival_int = 0;
    event.sigev_signo = 0;
    event.sigev_notify = 0;
    memset(&(event._sigev_un), 0x00, sizeof(event._sigev_un));
    /*
     *
     *
     *
     */


    /* Create the timer */
    event.sigev_value.sival_ptr = &timerid;
    event.sigev_signo = SIGRTMIN;
    event.sigev_notify = SIGEV_SIGNAL;
    memset(&(event._sigev_un), 0x00, sizeof(event._sigev_un));

    int err2 = 0;

    err2 = timer_create(CLOCK_MONOTONIC, &event, &timerid);
    if (err2) {
        fprintf(stderr, "Error: timer_create() failed: %s\n", strerror(err2));
    }

    timer_delete(timerid);



    pthread_t thrd_id = 0;

    int err1 = 0;

    err1 = pthread_create(&thrd_id, NULL, my_thrd_routine, NULL);
    if (err1) {
        fprintf(stderr, "Error: pthread failed: %s\n", strerror(err1));
    }
    else {
        pthread_join(thrd_id, NULL);
    }

    return 0;
}
