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
    /*
    What is happening
    1. Parent forks a child to execute ls
    2. parent waits for LS to finish before proceeding and forking another child
    3. parent waits again for the cat child process to complete before completition
    */
    int return_code_ls = fork();
    if (return_code_ls == 0) {
        printf("This is the child process for ls!\n");
        execlp("ls", "ls", "-a", "-l", NULL);
        perror("execlp ls");
        exit(EXIT_FAILURE);
    } else if (return_code_ls > 0) {// Parent process
        printf("LS PARENT\n");
        int pid_ls = return_code_ls;
        int status_ls = 0;
        waitpid(pid_ls, &status_ls, 0);
        printf("Child process for ls exits with code: %d\n", WEXITSTATUS(status_ls));

        // Fork for cat
        int return_code_cat = fork();
        if (return_code_cat == 0) {
            // Child process for cat
            printf("This is the child process for cat!\n");
            execlp("cat", "cat", NULL);
            // If execlp returns, an error occurred
            perror("execlp cat");
            exit(EXIT_FAILURE);
        } else if (return_code_cat > 0) {
            // Parent process
            printf("CAT parent\n");
            int pid_cat = return_code_cat;
            int status_cat = 0;
            waitpid(pid_cat, &status_cat, 0);
            printf("Child process for cat exits with code: %d\n", WEXITSTATUS(status_cat));
        } else {
            // Error handling for cat
            perror("fork cat");
            exit(EXIT_FAILURE);
        }
    } else {
        // Error handling for ls
        perror("fork ls");
        exit(EXIT_FAILURE);
    }
    printf("They finished; Done!\n");
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
