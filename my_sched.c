//Includes taken from starter.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
//#include <string.h>

#define QUANT = 2;  //Quantum value in milliseconds

/*
* A basic Process Control Block struct to contain relevant details of all processes
*/
typedef struct pc_block {
  pid_t pid;         //Process ID
  unsigned int priority;    //Process priority, where 0 is the highest and 20 the lowest
  char *exec_path;         //Path to executable, ex. ./starter.c (?)
  char *p_argv[];           //Arguments to executable
} pc_block;

/*
* Linked list node to contain a pc_block struct element and a pointer to the
* next node
*/
typedef struct pc_node {
  pc_block *element;
  struct pc_node *next;
} pc_node;


int main(int argc, char *argv[]) {
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
}
