//Includes taken from starter.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "sched.h"


/*
* Allocates memory to a proc struct and fills it with relevant data
*/
static Proc* makeProc(pid_t pid, unsigned int priority, char **args_array, unsigned int args_num) {
    Proc* new_p = malloc(sizeof(Proc));
    new_p->pid = pid;
    new_p->priority = priority;
    new_p->args_array = args_array;
    new_p->args_num = args_num;
    new_p->wait_time = 0;

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
static void addProcToEnd(Proc* proc, Pc_node** head) {
    if (proc == NULL) {
        fprintf(stderr, "%s\n", "Error: No process provided!");
        return;
    }
    if (*head == NULL) {
        *head = makeProcNode(proc, NULL); //Adds node to head of list
        fprintf(stderr, "2 Head: %d\n", *head == NULL);
        return;
    }

    Pc_node* current = *head;
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
    free(node->element->args_array);
    free(node->element);
    free(node);
    cleanList(node->next);
  }
}

/*
* Processes line form input to a Proc struct
*/
void lineToProc(char* line, Pc_node** head_node) {
    unsigned int priority;
    Proc* proc = NULL;

    //Memory allocated to array depends on line length
    unsigned int lineLength = strlen(line);
    char** args_array = malloc(sizeof(char*) * lineLength);

    unsigned int counter = 0;
    char* token = strtok(line, " \n");  //Tokenises string when reaching a space or newline character
    unsigned int args_num = 0;

    while(token != NULL) {
        if (counter == 0) {
            char* pri_str = strdup(token);
            priority = atoi(pri_str); //Converts string to int
            free(pri_str);
        }

      	else if (counter >= 1) {

            args_array[counter - 1] = strdup(token);
            args_num += 1;
        }
        args_array[counter] = (char*) NULL;


        token = strtok(NULL, " \n");
        counter += 1;
    }

    if (priority <= 20 && args_array[0]) { //Quick validity check
        proc = makeProc(0, priority, args_array, args_num);  //Process id 0 by default, given correct value when process is created
        fprintf(stderr, "LineProc pid: %d, priority: %d\n", proc->pid, proc->priority);
        addProcToEnd(proc, head_node);
    } else { //Invalid input
      fprintf(stderr, "Tokenising error\n");
    }
}


/*
* Prints all elements of a pc_node list, used for debugging
*/
void printList(Pc_node* head_node) {
    fprintf(stderr, "Printing list...\n");
    Pc_node* node = head_node;
    Proc* proc;
    char** argsArray = node->element->args_array;
    while (node != NULL) {
        proc = node->element;


        argsArray = proc->args_array;
        fprintf(stderr, "Print pid: %d, priority: %d", proc->pid, proc->priority);
        unsigned int i = 0;
        while (i < proc->args_num) {
          if (argsArray[i] != (char*) NULL)
            fprintf(stderr,", array:  %s", argsArray[i]);
          i += 1;
        }
        fprintf(stderr, "\n");
        node = node->next;
    }
}
