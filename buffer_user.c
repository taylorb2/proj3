#include <linux/string.h>
#include <linux/unistd.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>

static bb_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;

//long init_buffer_421(void) {
SYSCALL_DEFINE0(init_buffer_sem_421){
	// Write your code to initialize buffer

  //Determine if buffer exists
  //If it does exist exit
  if(buffer.read != NULL){
    printk("BUFFER ALREADY EXISTS, EXIT PROGRAM");
    return -1;
  }
  //If buffer does not exist, create it
  else{
    struct bb_node_sem_421 *pointer_current = NULL;
    struct bb_node_sem_421 *pointer_next = NULL;
    struct bb_node_sem_421 *first_node = NULL;

    //Creat circular buffer of length 20
    int i = 0;
    for(i = 0; i < SIZE_OF_BUFFER; i++){

      //If this is first node save the address
      if(i == 0){      
        pointer_current = kmalloc((sizeof(struct bb_node_sem_421)), GFP_KERNEL);
        first_node = pointer_current;
      }
      //If it is not first node but not last node add it to circular list
      else{
        pointer_next = kmalloc((sizeof(struct bb_node_sem_421)), GFP_KERNEL);
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
    printk("Mutex semaphore initiliazation failure");
  }
  
  int sem2_init = sem_init(&fill_count,0,buffer.length); //Initialize count to 0
  if(sem2_init == -1){
    printk("Mutex semaphore initiliazation failure");
  }
  
  int sem3_init = sem_init(&empty_count,0,SIZE_OF_BUFFER); //Initialize to SIZE_OF_BUFFER 
  if(sem3_init == -1){
    printk("Mutex semaphore initiliazation failure");
  }
	return 0; //Succesfull Initialation of semaphore and mutex
}


//long enqueue_buffer_421(char * data) {
SYSCALL_DEFINE1(enqueue_buffer_sem_421, char*, data){
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
    printk("Buffer is not initialized. Can not enqueue data.");
    return -1;  
  }
  
} //end of function

//long dequeue_buffer_421(char * data) {
SYSCALL_DEFINE1(dequeue_buffer_sem_421, char* , data){
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
    printk("Buffer is not initialized. Can not enqueue data.");
    return -1;  
  }
  
} //end of function


//long delete_buffer_421(void) {
SYSCALL_DEFINE0(delete_buffer_421){
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
      kfree(current);
      current = next;
    }

    //delete the final node
    kfree (current);

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
    printk("Can not delete a buffer that does not exist");
    return -1;
  }
}

