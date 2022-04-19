#define _BSD_SOURCE //to use usleep (might go in header file)
#include "buffer_mon.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
//#include <semaphore.h>
//#include <linux/semaphore.h>


//Forward Declerations
void* producer_function(void* arg);
void* consumer_function(void* arg);

//Driver File for Test
int main(void) {

  //Create Threads and initialize buffer
  int init_code = init_buffer_421();
  
  pthread_t producer_thread;
  pthread_t consumer_thread;


  pthread_create(&producer_thread,NULL, producer_function,NULL);
  pthread_create(&consumer_thread,NULL, consumer_function,NULL);

  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  delete_buffer_421();
  printf("done");
  return 0;
}
