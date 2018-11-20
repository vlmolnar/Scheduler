//Includes taken from starter.c
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
            printf("Child process.\n");
            // execl("./printchars", "./printchars", "a", NULL);       // Print some "a"s
/**/
            Args_node* args_head = current_node->element->args;
            if (args_head == NULL) {
              printf("No args\n");
              execl(current_node->element->exec_path, current_node->element->exec_path);  //TODO convert list into 2D array?

            } else {
              printf("Some args, length:%d\n", current_node->element->args_num);

              // Initialises char pointer array made up of args_num number of
              // 500 char length strings. args_num + 2 because firt arg needs to
              //be the path, while last arg needs to be NULL
              char** argsArray = malloc(sizeof(char*) * 500);
              // char argsArray[current_node->element->args_num + 2][500];
              Args_node* args_current = args_head;
              unsigned int args_counter = 1;

              //Initialises first and last element of array
              strncpy(argsArray[0], current_node->element->exec_path, sizeof(current_node->element->exec_path) - 1);
              argsArray[current_node->element->args_num + 1] = (char*)NULL;

              while (args_current) {
                  // strncpy corruption fix: https://eklitzke.org/beware-of-strncpy-and-strncat
                   strncpy(argsArray[args_counter], args_current->element, sizeof(args_current->element) - 1);

                   fprintf(stderr,"args_current: %s, argsarray: %s\n",args_current->element, argsArray[args_counter]);
                   args_current = args_current->next;
                   args_counter += 1;
              }

/**/

              int i = 0;
              while (*argsArray[i]) {
                 fprintf(stderr,"array:  %s\n", argsArray[i]);
                 i += 1;
               } fprintf(stderr,"0: %s, 1: %s, 2: %s, 3: %s\n", argsArray[0], argsArray[1], argsArray[2], argsArray[3]);
              // execl(current_node->element->exec_path, current_node->element->exec_path, current_node->element->args->element, current_node->element->args->next->element, current_node->element->args->next->next->element, '\0');  //TODO convert list into 2D array?
/**/
              execv(current_node->element->exec_path, argsArray);
            }
            /**/
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
