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

#define BUFF_SIZE 255

int main(int argc, char *argv[]) {
    //Checks for a minimum of 1 argument
    if (argc < 1) {
      fprintf(stderr, "Program usage: ./sched <config_file_path>\n");
      exit(0);
    }

    //Ref: https://www.tutorialspoint.com/cprogramming/c_file_io.htm
    FILE *fp;
    fp = fopen(argv[0], "r");  //Opens file for reading

    //Read line
    char *line;
    Pc_node* head_node = NULL;
    Proc* proc = NULL;
    // size_t len = 0;

    //While there is a new line
    while ((getline(&line, (size_t) BUFF_SIZE, fp)) != -1) { //Ref: https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
        lineToProc(line, head_node);
    }

    fclose(fp); //Closes file

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
