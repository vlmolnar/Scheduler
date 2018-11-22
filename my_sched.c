#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "sched.h"


int main(int argc, char *argv[]) {
    /***************
    * FILE READING *
    ***************/

    //Check for a minimum of 1 argument
    if (argc <= 1) {
      fprintf(stderr, "Program usage: ./sched <config_file_path>\n");
      exit(0);
    }

    //Ref: https://www.tutorialspoint.com/cprogramming/c_file_io.htm
    FILE *fp;
    fp = fopen(argv[1], "r");  //Opens file for reading

    if (fp == NULL) {
      fprintf(stderr, "Error: Invalid FILE path \"%s\"\n", argv[1]);
      exit(0);
    }

    //Read line
    char line[225];
    Pc_node* head_node = NULL;

    //While there is a new line
    while ((fgets(line, sizeof(line), fp))) { //Ref: https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c
      // fprintf(stderr, "In while, line is: %s\n", line);
        lineToProc(line, &head_node);
    }
    fclose(fp); //Closes file

    // printList(head_node);


    /***************************
    * PROCESSES INITIALISATION *
    ***************************/

    //Run processes
    Pc_node* current_node = head_node;
    pid_t pid1;
    unsigned int prog_count = 0;
    unsigned int kill_count = 0;

    while (current_node) {
        pid1 = fork();  //New process

        if(pid1 < 0) {
            fprintf(stderr, "Something went wrong while creating process!\n");
        } else if(pid1 > 0) {    //Parent process
            kill(pid1,SIGSTOP);
            // printf("Parent process.\n");
            prog_count+= 1;
            current_node->element->pid = pid1;  //Sets child's pid
        } else {  //Child process
            // printf("Child process.\n");
              execv(current_node->element->args_array[0], current_node->element->args_array);
              fprintf(stderr, "Error: Invalid process path \"%s\"\n", current_node->element->args_array[0]);
              exit(0);
        }

        //FIFO
        // pid_t result = 0;
        // while (!result) {
        //     int status = 0;
        //     // fprintf(stderr, "Inside status while...\n");
        //     result = waitpid( pid1, &status, WNOHANG );
        //     if (result == 0) {
        //       // fprintf(stderr, "Exit condition NOT met\n");
        //     }
        //     if( result != 0) {  //Child process finished executing
        //       // fprintf(stderr, "Exit condition met\n");
        //         kill(pid1, SIGTERM);  //Kills process
        //         break;
        //     }
        // }
        //END OF FIFO

        current_node = current_node->next;
    }


    /*************
    * SCHEDULING *
    *************/

    Pc_node* queue1_head = NULL;  //Priority 0-6
    // Pc_node* queue2_head = NULL; //Priority 7-13
    // Pc_node* queue3_head = NULL; //Priority 14-20
    replicateList(head_node, &queue1_head);
    fprintf(stderr, "Printing original\n");
    printList(head_node);
    fprintf(stderr, "Printing copy\n");
    printList(queue1_head);

    Pc_node* sched_node = queue1_head;

    while (prog_count > kill_count) {
          // if (current_node == NULL) current_node = head_node; //Makes list loop
          if (sched_node == NULL) sched_node = queue1_head; //Makes list loop
          pid1 = sched_node->element->pid;
          // fprintf(stderr, "pid1: %d\n", pid1);

          if (pid1 != -1) { //Skips terminated processes

          kill(pid1, SIGCONT); //Lets process execute
          usleep(QUANT_VAL); //Takes in microseconds

          id_t result = 0;
          int status = 0;
          result = waitpid( pid1, &status, WNOHANG );

          if (result == 0) {
            // fprintf(stderr, "Child %d still going stong\n", pid1);
              kill(pid1,SIGSTOP); //Pauses process
          }
          else {  //Child process finished executing
            // fprintf(stderr, "Child %d terminated\n", pid1);
              kill(pid1, SIGTERM);  //Terminates process
              sched_node->element->pid = -1; //Sets pid to indicate termination
              kill_count += 1;
          }
          incrementAllWait(head_node, pid1);
      }
      sched_node = sched_node->next;
    }
    fprintf(stderr, "Out of loops\n");
    // printList(head_node);
    // averageWaitTime(head_node);
    averageWaitTime(queue1_head);
    // cleanList(queue1_head);
    cleanList(head_node);
    fprintf(stderr, "\n--Program finished executing!--\n");

}
