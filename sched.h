#ifndef SCHED_H
#define SCHED_H

#define QUANT = 2;  //Quantum value in milliseconds

/*
* A linked list to store the process variables as strings
*/
typedef struct args_node {
    char* element;
    struct args_node* next;
} Args_node;

/*
* A basic Process Control Block struct to contain relevant details of all processes
*/
typedef struct proc {
  // pid_t pid;             //Process ID
  unsigned int pid;         //Process ID
  unsigned int priority;    //Process priority, where 0 is the highest and 20 the lowest
  char *exec_path;          //Path to executable, ex. ./starter.c (?)
  Args_node* args;          //Arguments to executable
} Proc;

/*
* Linked list node to contain a pc_block struct element and a pointer to the
* next node
*/
typedef struct pc_node {
  Proc *element;
  struct pc_node *next;
} Pc_node;



#endif
