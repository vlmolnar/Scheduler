#ifndef SCHED_H
#define SCHED_H

#include <signal.h>

#define QUANT_VAL 200000  //Quantum value in microseconds, 200 milliseconds

/*
* A basic Process Control Block struct to contain relevant details of all processes
*/
typedef struct proc {
  pid_t pid;                //Process ID
  unsigned int priority;    //Process priority, where 0 is the highest and 20 the lowest
  char** args_array;        // An array of char pointers to contain all process arguments
  unsigned int args_num;    //Number of arguments
  unsigned int wait_time;   //Waiting time of process
} Proc;

/*
* Linked list node to contain a pc_block struct element and a pointer to the
* next node
*/
typedef struct pc_node {
  Proc *element;
  struct pc_node *next;
} Pc_node;

void changeProcPid(Proc* proc, pid_t new_pid);

void lineToProc(char* line, Pc_node** head_node);

void cleanList(Pc_node* node);

void printList(Pc_node* head_node);

void incrementAllWait(Pc_node* head_node, pid_t pid_ex);

void averageWaitTime(Pc_node* head_node);

#endif
