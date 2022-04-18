#define _BSD_SOURCE //to use usleep (might go in header file)
#include "buffer_sem.h"
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "buffer_sem.h"


//Producer helper function for producer thread
void* producer_function(void* arg){
  char character[DATA_LENGTH];
  for(int i = 0; i < 1000; i++){

    memset(character, (i % 10),DATA_LENGTH);
    /*for(int j = 0; j < DATA_LENGTH; j++){
      character[j] = i % 10; //Get ones place
    } //end of inside for
  */
    
    int equeue_code = enqueue_buffer_421(character);  
    //print_semaphores();

    
     if(equeue_code == -1){
      printk("Buffer does not exist, can not equeue");
      break;
    }
    
  } //end of second for loop
  return NULL; 
}

//Consumer helper function for consumer thread
void* consumer_function(void* arg){
  char character[DATA_LENGTH];
  
  for(int i = 0; i < 1000; i++){
    int dequeue_code = dequeue_buffer_421(character);  

    if(dequeue_code == -1){
      printk("Buffer does not exist, can not dequeue");
      break;
    }
  } //end of for loop

  return NULL;
}

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

  return 0;
}
