#include "buffer_sem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bb_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;

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
      printf("Buffer does not exist, can not equeue");
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
      printf("Buffer does not exist, can not dequeue");
      break;
    }
  } //end of for loop

  return NULL;
}

long init_buffer_421(void) {
	// Write your code to initialize buffer

  //Determine if buffer exists
  //If it does exist exit
  if(buffer.read != NULL){
    printf("BUFFER ALREADY EXISTS, EXIT PROGRAM");
    return -1;
  }
  //If buffer does not exist, create it
  else{
    struct bb_node_421 *pointer_current = NULL;
    struct bb_node_421 *pointer_next = NULL;
    struct bb_node_421 *first_node = NULL;

    //Creat circular buffer of length 20
    int i = 0;
    for(i = 0; i < SIZE_OF_BUFFER; i++){

      //If this is first node save the address
      if(i == 0){      
        pointer_current = malloc(sizeof(struct bb_node_421));
        first_node = pointer_current;
      }
      //If it is not first node but not last node add it to circular list
      else{
        pointer_next = malloc(sizeof(struct bb_node_421));
        pointer_current -> next = pointer_next;
        pointer_current = pointer_current -> next;
        //pointer_current -> data = 0;
      
      }//end of else   
    } //end of for loop

    //Connect last node to first node
    pointer_current -> next = first_node;

    //Initialize buffer structure
    buffer.length = 0;
    buffer.read = first_node; //Set to first node in list
    buffer.write = first_node; //Set to first node in list    
  } //end of else
  
  /////////////////////////////////////////
	// Initialize your semaphores here.
  int sem1_init = sem_init(&mutex,0,1); //Shared between threads with value 1 (boolean)
  if(sem1_init == -1){
    printf("Mutex semaphore initiliazation failure");
  }
  
  int sem2_init = sem_init(&fill_count,0,buffer.length); //Initialize count to 0
  if(sem2_init == -1){
    printf("Mutex semaphore initiliazation failure");
  }
  
  int sem3_init = sem_init(&empty_count,0,SIZE_OF_BUFFER); //Initialize to SIZE_OF_BUFFER 
  if(sem3_init == -1){
    printf("Mutex semaphore initiliazation failure");
  }
	return 0; //Succesfull Initialation of semaphore and mutex
}


long enqueue_buffer_421(char * data) {
	// Write your code to enqueue data into the buffer

  //Random amount of time before enqueue between 0 and 10 milliseconds
  //srand(1);
  int  m_seconds = rand() % 10000;
  usleep(m_seconds); //suspends execution of the calling thread(0-10,000 microseconds)

  //Make sure buffer is initialized
  //buffer is initialized
  if(buffer.read){

    sem_wait(&empty_count); //decrement
    print_semaphores();
    sem_wait(&mutex);

    //Critical Section
    strcpy(buffer.write -> data, data);
    buffer.write = buffer.write -> next; //update write pointer 
    
    // end of critical
    
    buffer.length = buffer.length + 1; //increment length
    sem_post(&fill_count); //increment
    sem_post(&mutex); //increment to 1
    
    return 0;
  }

  //buffer not initialized
  else{
    printf("Buffer is not initialized. Can not enqueue data.");
    return -1;  
  }
  
} //end of function

long dequeue_buffer_421(char * data) {

	// Write your code to dequeue data from the buffer

// Write your code to enqueue data into the buffer

  //Random amount of time before enqueue between 0 and 10 milliseconds
  srand(2); //different seed
  int m_seconds = (rand() % 10000) + 2000; // more balanced version 
  //switch line to int  m_seconds = (rand() % 10000) + 2000;
  usleep(m_seconds); //suspends execution of the calling thread(0-10,000 microseconds)

  //Make sure buffer is initialized
  //buffer is initialized
  if(buffer.read){

    sem_wait(&fill_count); //decrement
    print_semaphores();
    sem_wait(&mutex);

    //Critical Section
    strcpy(data, buffer.read -> data);
    buffer.read = buffer.read -> next; //update read pointer 

    // end of critical
    
    buffer.length = buffer.length - 1; //decrement length

    sem_post(&empty_count); //increment
    sem_post(&mutex);
    
    return 0;
  }

  //buffer not initialized
  else{
    printf("Buffer is not initialized. Can not enqueue data.");
    return -1;  
  }
  
} //end of function


long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	// write your code to delete buffer and other unwanted components

  //Determine if buffer exists
  //BUffer exists  
  if(buffer.read != NULL){  
    //Delete nodes of buffer
    struct bb_node_421  *current = buffer.read;
    struct bb_node_421  *next;

    //Itterate through buffer and delete
    for(int w = 0; w < ((buffer.length) - 1); w++){
      next = current -> next;
      free(current);
      current = next;
    }

    //delete the final node
    free (current);

    //Delete semaphore and reset buffer struct
    sem_destroy(&mutex);
    sem_destroy(&fill_count);
    sem_destroy(&empty_count);
    buffer.read = NULL;
    buffer.write = NULL;
    buffer.length = 0;
    
    return 0;
        
  }
  //BUffer does not exist, can not delete
  else{
    printf("Can not delete a buffer that does not exist");
    return -1;
  }
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int value;
	sem_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	return;
}
