#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

/*
1st and last processes are edge cases because 
1st: read in normally, but write to a pipe
last: read from a pipe, but output normally
for the pipes in between: read from pipe and write to pipe

pipe()
*/
int main(int argc, char *argv[])
{
    // added because on the spec
    if (argc == 1) {
        errno = EINVAL; // Set errno to EINVAL
        perror("error"); 
        exit(errno); 
    }
    //check for argument size
    if(argc < 2){
        fprintf(stderr, "Usage: %s ENTER the right amount of arguments\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_progs = argc-1;
    int prev_pipe_fds[2];
    int curr_pipe_fds[2];

    // check if inital
    if (pipe(prev_pipe_fds) == -1) {
        perror("pipe creation ERROR");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < num_progs; i++){
        if(i < num_progs - 1) { 
            // create the pipe before FORKING if NOT the last process
            if (pipe(curr_pipe_fds) == -1) {
                perror("pipe creation ERROR");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork ERROR");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){ // child process
            // printf("Child %d: %s\n", getpid(), argv[i + 1]);
            
            // redirect INPUT (change INPUT to #3) if NOT first process
            if (i != 0) {
                dup2(prev_pipe_fds[0], STDIN_FILENO);
                close(prev_pipe_fds[0]);
                close(prev_pipe_fds[1]);
            }

            // redirect output if NOT the last process
            if (i != num_progs - 1) {
                dup2(curr_pipe_fds[1], STDOUT_FILENO);
                close(curr_pipe_fds[0]);
                close(curr_pipe_fds[1]);
            }

            // exec the program
            execlp(argv[i + 1], argv[i + 1], NULL);
            perror("execlp");
            exit(EXIT_FAILURE);
        }

        else { // PARENT process
            // printf("Parent: Waiting for child %d\n", pid);
            if (i > 0) {
                close(prev_pipe_fds[0]);
                close(prev_pipe_fds[1]);
            }
            if (i < num_progs - 1) { // copy over the inputs and output pointers
                prev_pipe_fds[0] = curr_pipe_fds[0];
                prev_pipe_fds[1] = curr_pipe_fds[1];
            }
            
            int status;

            // handling the bogus argument cases
            pid_t child_pid;
            while ((child_pid = waitpid(-1, &status, 0)) > 0) {
                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
                    if (exit_status != 0) {
                        fprintf(stderr, "Child process %d terminated with non-zero exit status: %d\n", child_pid, exit_status);
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Child process %d terminated abnormally\n", child_pid);
                    exit(EXIT_FAILURE);
                }
            }

            if (child_pid == -1 && errno != ECHILD) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            // printf("Parent: Child %d finished\n", pid);
        }
    }
    // Close the final pipe in the parent process
    if (num_progs > 1) {
        close(prev_pipe_fds[0]);
        close(prev_pipe_fds[1]);
    }

    return 0;
}
