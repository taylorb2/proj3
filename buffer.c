#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>


ring_buffer_421_t* buffer;
int initialize = 0; //0 for not intialized, 1 for intialized

int main(void) {
  int initialize_code = init_buffer_421();
  int print_code = print_buffer_421();
  int insert_code = 0; //-1 if error
  
  //Test Insert
  insert_code = insert_buffer_421(1);
  insert_code = insert_buffer_421(2);
  insert_code = insert_buffer_421(3);
  insert_code = insert_buffer_421(4);
  insert_code = insert_buffer_421(5);
  insert_code = insert_buffer_421(6);
  insert_code = insert_buffer_421(7);
  insert_code = insert_buffer_421(8);
  insert_code = insert_buffer_421(9);
  insert_code = insert_buffer_421(10);
  insert_code = insert_buffer_421(11);
  insert_code = insert_buffer_421(12);
  insert_code = insert_buffer_421(13);
  insert_code = insert_buffer_421(14);
  insert_code = insert_buffer_421(15);
  insert_code = insert_buffer_421(16);
  insert_code = insert_buffer_421(17);
  insert_code = insert_buffer_421(18);
  insert_code = insert_buffer_421(19);
  insert_code = insert_buffer_421(20);
  insert_code = insert_buffer_421(21);
  print_code = print_buffer_421();

  int delter_code = delete_buffer_421();
  
  return 0;
}



//Initialize buffer
//Note: change malloc to kmalloc for kernal space implementation
long init_buffer_421(void){

  int exist = 0; //0 for doesnt exist, 1 for already exists
  //Initialize data strructure
  buffer = malloc(sizeof(struct ring_buffer_421));
  buffer -> read = NULL;
  buffer -> write = NULL;
  buffer -> length = 0;
  //printf("%d", buffer -> length);
  //printf("Got in here!");
  
  //Determine if data structure already exists or not
  if(initialize == 0){
    struct node_421 *pointer_current = NULL;
    struct node_421 *pointer_next = NULL;
    struct node_421 *first_node = NULL;
  
    //Creat circular buffer of length 20
    int i = 0;
    for(i = 0; i < SIZE_OF_BUFFER; i++){

      //If this is first node save the address
      if(i == 0){      
        pointer_current = malloc(sizeof(struct node_421));
        first_node = pointer_current;
        //Insert dummy data
        pointer_current -> data = 0;
      }
      //If it is not first node but not last node add it to circular list
      else{
        pointer_next = malloc(sizeof(struct node_421));
        pointer_current -> next = pointer_next;
        pointer_current = pointer_current -> next;
        pointer_current -> data = 0;
      
      }//end of else   
    } //end of for loop

    
    //Connect last node to first node
    pointer_current -> next = first_node;

    //Initialize buffer structure
    buffer -> length = 0;
    buffer -> read = first_node; //Set to first node in list
    buffer -> write = NULL; //Set blank

    initialize = 1; //change global
    
    //Return Status
    return 0; //Circular buffer succesfully made
  } //end of if

  //If data structure already exists
  else{
    return -1; //failed to initialize or linked list already exists
  } 
} //end of function

////////////////////////////////////////////////////////////////////////////
//Print the linked list buffer 
long print_buffer_421(void){
  //If the buffer exists and has been initialized, then print
  if(initialize == 1){

     //Save the head of the list to know when it repeats  
    struct node_421 *temp_head = buffer -> read;
    int start_print = 0; // 0 if not started, 1 if have 
    int counter = 0;

    //Determine if the list exists and can be printed
    //If the list exists, print
    while((buffer -> read != temp_head) || (start_print == 0)){
      //If we have not started itterating through linked list yet, itterate first pointer
      if(start_print == 0){        
        //Print Content
        printf("\n Node Number: %d ", counter);
        printf("\nData: %d", buffer -> read -> data);
        
        buffer -> read = buffer -> read -> next;
        start_print = 1;
        counter ++;
      }
      //Stop when we have reached front of list
      else if(buffer -> read != temp_head){
        //Print Content
        printf("\n Node Number: %d ", counter);
        printf("\nData: %d", buffer -> read -> data);
        
        buffer -> read = buffer -> read -> next;
        counter ++;
      }  
    }
    buffer -> read = temp_head;
    return 0;
  }
  //If the buffer has not been initialized, return error code
  else{
    return -1;
  }
}

////////////////////////////////////////////////////////////////////////
//Insert Data (integer i) into next available buffer
long insert_buffer_421(int i){

  //Check to see if buffer is initialized. ALso check to see if buffer is full
  //Buffer exists and data can be inserted
  if(initialize == 1 && buffer -> length != SIZE_OF_BUFFER){
    struct node_421 *temp_head = buffer -> read;
    buffer -> write = temp_head;

    //Itterate through to open node
    for(int j = 0; j < buffer -> length; j++){
      buffer -> write = buffer -> write -> next;
    }
    //Insert
    buffer -> write -> data = i;
    buffer -> length = (buffer -> length) + 1; //increment length by one
    buffer -> write = temp_head; //set write back to start of list
    return 0;
  }
  //Data can not be inserted succesfully
  else{
    return -1; 
  }
} //end of function


/////////////////////////////////////////////////////////////
//Function to delete buffer and dynamic memory associated
long delete_buffer_421(){
  //Determine if the buffer exists
  //The buffer Does exist
  
  if(initialize == 1){
    struct node_421 *current = buffer -> read;
    struct node_421 *next;

    //Itterate through and delete the whole list
    for(int w = 0; w < ((buffer -> length) - 1); w++){
      next = current -> next;
      free(current);
      current = next;      
    }
    //delete the final node
    free(current);
  }
  //If it does not exist
  else{
    return -1;
  }
}






































