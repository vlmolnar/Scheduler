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
    //Read line
    char* line;
    Pc_node* head = NULL;
    Proc* proc = NULL;
    size_t len = 0;

    //While there is a new line
    while (getline(&line, &len, stdin) != -1) {
      

    }

    //Run processes
    pid_t pid1 = 0, pid2 = 0;

    pid1 = fork();

    if(pid1 < 0) {
        fprintf(stderr, "Something went wrong while creating process!\n");
    } else if(pid1 > 0) {    // We are parent. Immediately stop the new process
        printf("Parent process.\n");
        kill(pid1,SIGSTOP);
    } else {  // We are a child process -- overwrite our process space with the new program
        printf("Child process.\n");
        execl("./printchars", "./printchars", "a", NULL);       // Print some "a"s
    }

    kill(pid1, SIGCONT); //Lets process execute
    usleep(500000); //Takes in microseconds,1 ms is 100 us
    kill(pid1, SIGTERM);  //Kills process

    /*
    int status;
    pid_t result = waitpid( process_id, &status, WNOHANG );
    if( result == 0) { // process is running }
    else { // process has terminated and we can read its status using waitpid() // }
    */
}
