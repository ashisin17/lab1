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
	// int return_code = fork();
    // if (return_code == 0) {
    //     printf("This is the child process!\n");
    //     execlp("ls", "ls", "-a", "-l", NULL);
    //     // sleep(5);
    //     // exit(0); // this is not necessary
    // } else if (return_code > 0) {
    //     printf("I am lazy parent, letting my child to ls the directory\n");
    //     printf("I will just wait for their report\n");
    //     int pid = return_code;
    //     int status = 0;
    //     waitpid(pid, &status, 0);
    //     printf("Child process exits with code: %d\n", WEXITSTATUS(status));
    // } else {
    //     printf("Child process creation error! \n");
    // }
    // printf("They finished; Done!\n");
    // return 0;
    printf("J Running ./pipe ls\n");    
    // Execute ls command
    execlp("ls", "ls", "-a", "-l", NULL);
    
    // If execlp returns, an error occurred
    perror("execlp");
    return EXIT_FAILURE;

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
