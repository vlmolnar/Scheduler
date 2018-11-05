//Includes taken from starter.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
//#include <string.h>
#include "sched.h"

/*
typedef struct proc {
  // pid_t pid;         //Process ID
  unsigned int pid;         //Process ID
  unsigned int priority;    //Process priority, where 0 is the highest and 20 the lowest
  char *exec_path;         //Path to executable, ex. ./starter.c (?)
  char *p_argv[];           //Arguments to executable
} Proc;


typedef struct pc_node {
  proc *element;
  struct pc_node *next;
} pc_node;

*/


/*
* Allocates memory to a proc struct and fills it with relevant data
*/
static Proc* makeProc(pid_t pid, unsigned int priority, char *exec_path, char *p_argv[]) {
    Proc* new_p = malloc(sizeof(Proc));
    new_p->pid = pid;
    new_p->priority = priority;
    new_p->exec_path = exec_path;
    new_p->p_argv = p_argv;

    return new_p;
}

/*
* Creates a new pc_node struct and adds it to the indicated position in the linked list
*/
static Pc_node* makeProcNode(Proc* proc, Pc_node* previous) {
    // if (proc == NULL) return NULL;
    Pc_node* node = malloc(sizeof(Pc_node));
    node->element = proc;
    node->next = NULL;

    if (previous != NULL) {
  	     previous->next = node;
    }
  	return node;

}

/*
* Inserts proc struct to the end of the list. If the list is empty (head == NULL),
* proc is inserted to head
*/
void addProcToQueue(Proc* proc, Pc_node* head) {
    if (proc == NULL) {
        fprintf(stderr, "%s\n", "Error: No process provided!");
        return;
    }
    if (head == NULL) {
        head = makeProcNode(proc, NULL); //Adds node to head of list
        return;
    }

    Pc_node* current = head;
  while(1) {
    if (current->next == NULL) {  //Adds node to end of list
      makeProcNode(proc, current);
      break;
    }
    else current = current->next;
  }

}

/*
* Recursively frees up all memory allocated to list
*/
void cleanList(Pc_node* node) {
  if (node) {
    free(node->element);
    free(node);
    cleanList(node->next);
  }
}
