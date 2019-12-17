// high_resolution_clock example
#include <stdio.h>//#include <iostream>
//#include <ctime>
//#include <ratio>

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

#include <unistd.h>//usleep(): microsecond-level sleep

int main ()
{

  high_resolution_clock::time_point t1 = high_resolution_clock::now();


  for (int i=0; i<10; ++i) {
    usleep(1);
  }

  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

  printf("It took me %.6F seconds...\n", time_span.count());

  return 0;
}
