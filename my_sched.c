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




int main(int argc, char *argv[]) {
    //Checfor a minimum of 1 argument
    if (argc <= 1) {
      fprintf(stderr, "Program usage: ./sched <config_file_path>\n");
      exit(0);
    }

    //Ref: https://www.tutorialspoint.com/cprogramming/c_file_io.htm
    FILE *fp;
    fp = fopen(argv[1], "r");  //Opens file for reading
    fprintf(stderr, "File opened\n");

    //Read line
    char line[225];
    Pc_node* head_node = NULL;
    // size_t len = 225;

    //While there is a new line
    while ((fgets(line, sizeof(line), fp))) { //Ref: https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c
      fprintf(stderr, "In while, line is: %s\n", line);
        lineToProc(line, &head_node);
    }
    fclose(fp); //Closes file


    printList(head_node);



    //Run processes
    Pc_node* current_node = head_node;
    Proc* proc = NULL;
    pid_t pid1;
    unsigned int prog_count = 0;
    unsigned int kill_count = 0;
    while (current_node) {
        pid1 = fork();  //New process

        if(pid1 < 0) {
            fprintf(stderr, "Something went wrong while creating process!\n");
        } else if(pid1 > 0) {    // We are parent. Immediately stop the new process
            kill(pid1,SIGSTOP);
            printf("Parent process.\n");
            prog_count+= 1;
            current_node->element->pid = pid1;  //Sets child's pid
        } else {  // We are a child process -- overwrite our process space with the new program
            // printf("Child process.\n");
            // execl("./printchars", "./printchars", "a", NULL);       // Print some "a"s
            char* ptArray[5];
            execl(current_node->element->exec_path, current_node->element->exec_path, current_node->element->args->element, NULL);  //TODO convert list into 2D array?

        }

        // kill(pid1, SIGCONT); //Lets process execute
        // usleep(500000); //Takes in microseconds


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

    while (prog_count > kill_count) {
          if (current_node == NULL) current_node = head_node; //Makes list loop
          pid1 = current_node->element->pid;
          // fprintf(stderr, "pid1: %d\n", pid1);

          if (pid1 != -1) { //Skips terminated processes

          kill(pid1, SIGCONT); //Lets process execute
          usleep(QUANT_VAL); //Takes in microseconds

          id_t result = 0;
          int status = 0;
          result = waitpid( pid1, &status, WNOHANG );
          if (result == 0) {
              // fprintf(stderr, "Child lives on\n");
              kill(pid1,SIGSTOP); //Pauses process
          }
          if( result != 0) {  //Child process finished executing
              // fprintf(stderr, "Child is dead\n");
              kill(pid1, SIGTERM);  //Terminates process
              current_node->element->pid = -1; //Sets pid to indicate termination
              kill_count += 1;
          }
      }
      current_node = current_node->next;
    }





    cleanList(head_node);
    fprintf(stderr, "Program finished executing!\n");

    /*
    int status;
    pid_t result = waitpid( process_id, &status, WNOHANG );
    if( result == 0) { // process is running }
    else { // process has terminated and we can read its status using waitpid() // }
    */
}
