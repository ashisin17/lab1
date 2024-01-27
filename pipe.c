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
	if(argc < 2) { // need pipe SMTHG at least
		fprintf(stderr, "add at least 1 argument \n", argv[0]);
        return EXIT_FAILURE;
	}

	//create a pipe
	int pipe_fds[2];
	int pipe_return = pipe(pipe_fds);
    if (pipe_return == -1) {
        perror("pipe error");
        return EXIT_FAILURE;
    }
	
	pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (pid == 0) { // Child process
        dup2(pipe_fds[1], 1);// Redirect stdout to the write end of the pipe
        close(pipe_fds[0]); // Close read end of the pipe
		close(pipe_fds[1]); // Close the original write end

        // Execute the command
        if (execlp(argv[1], argv[1], NULL) == -1) {
            perror("execlp");
            return errno;
        }

    } else { // Parent process
        close(pipe_fds[1]); // Close write end of the pipe
        wait(NULL); // Wait for the child to finish
        close(pipe_fds[0]); // Close the read end of the pipe

        return EXIT_SUCCESS;
    }


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
