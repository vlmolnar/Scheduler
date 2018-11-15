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
    cleanList(head_node);
    fprintf(stderr, "Program finished executing!\n");

    //Run processes
    // Proc* proc = NULL;
    // pid_t pid1 = 0, pid2 = 0;
    //
    // pid1 = fork();
    //
    // if(pid1 < 0) {
    //     fprintf(stderr, "Something went wrong while creating process!\n");
    // } else if(pid1 > 0) {    // We are parent. Immediately stop the new process
    //     printf("Parent process.\n");
    //     kill(pid1,SIGSTOP);
    // } else {  // We are a child process -- overwrite our process space with the new program
    //     printf("Child process.\n");
    //     execl("./printchars", "./printchars", "a", NULL);       // Print some "a"s
    // }
    //
    // kill(pid1, SIGCONT); //Lets process execute
    // usleep(500000); //Takes in microseconds,1 ms is 100 us
    // kill(pid1, SIGTERM);  //Kills process

    /*
    int status;
    pid_t result = waitpid( process_id, &status, WNOHANG );
    if( result == 0) { // process is running }
    else { // process has terminated and we can read its status using waitpid() // }
    */
}
