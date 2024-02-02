#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
1st and last processes are edge cases because 
1st: read in normally, but write to a pipe
last: read from a pipe, but output normally
for the pipes in between: read from pipe and write to pipe

pipe()
*/
int main(int argc, char *argv[])
{
    //check for argument size
    if(argc < 2){
        fprintf(stderr, "Usage: %s ENTER the right amount of arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_progs = argc-1;
    int pipe_fds[2];
    
    for(int i = 0; i < num_progs; i++){
        if(i < c - 1) { // create the pipe before FORKING if NOT the last process
            if(pipe(pipe_fds) == -1){
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        // this is NOT the last process
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){ // child process
            // redirect INPUT (change std in to #3) if NOT first process
            printf("Child %d: %s\n", getpid(), argv[i + 1]);
            if (i != 0) {
                dup2(pipe_fds[0], STDIN_FILENO);
                close(pipe_fds[0]);
            }

            // redirect output if NOT the last process
            if (i != num_progs - 1) {
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            // exec the program
            execlp(argv[i + 1], argv[i + 1], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);

        }
        else { // PARENT process
            printf("Parent: Waiting for child %d\n", pid);
            if (i < num_progs - 1) {
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            wait(NULL);
            printf("Parent: Child %d finished\n", pid);
        }
    }
    return 0;
}

// DISCUSSIOn
// // TODO: it's all yours
// 	// return 0;

// 	int pipe_fds[2];
// 	pipe(pipe_fds); // create a pipe of two ends
// 	fork();

// 	close(pipe_fds);
// 	dup2(pipe_fds[1])
// 	close(pipe_fds[0])
// 	pipe(pipe_fds)
// 	fork()

// 	//for loop
// 	dup2(pipe_fds[1], 1);
// 	close(pipe_fds[1])
// 	close(pipe_fds[0])

// 	if (1 <= i <= argc) {
// 		if (i =1)
// 			//1st program
// 		else if(i == argc-1){
// 			//last program
// 		}
// 	}
