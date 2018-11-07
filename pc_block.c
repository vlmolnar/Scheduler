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
static Proc* makeProc(pid_t pid, unsigned int priority, char *exec_path, char **p_argv) {
    Proc* new_p = malloc(sizeof(Proc));
    new_p->pid = pid;
    new_p->priority = priority;
    new_p->exec_path = exec_path;
    // new_p->p_argv = p_argv;  //May need memcpy

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
* Processes line form input to a Proc struct
*/
Proc* lineToProc(char* line, Pc_node* head) {
    pid_t pid; unsigned int priority; char *exec_path; char** p_argv;
    Proc* proc = NULL;
    unsigned int counter = 0;
    char* token = strtok(line, " \n");


    while(token != NULL) {
        if (counter == 0) priority = strdup(token);
      	else if (counter == 1) exec_path = strdup(token);
      	else if (counter >= 2) p_argv = strdup(token); //Each one added to a new array?

        token = strtok(NULL, " \n");
        counter += 1;
    }

    if (priority >= 0 && priority <= 20 && exec_path) { //Quick validity check
        proc = makeProc(-1, priority, exec_path, p_argv);
        return proc;
    } else {
      free(priority); free(exec_path); free(p_argv);
    }
    return proc;
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
