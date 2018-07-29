
#include <stdio.h>          /* perror */
#include <stdlib.h>         /* exit */
#include <sys/types.h>      /* fork */
#include <unistd.h>         /* fork, pipe, close, execlp */

int main(void) {
    int fd[2];
    pid_t pid;
    
    //define pipe
    if (pipe(fd) < 0){
        perror("pipe");
        exit(1);
    }

    //create child process
    pid = fork();
    if (pid == -1){
        perror("fork");
        exit(1);
    }
    //child process
    else if (pid == 0){
        //close unused fd[0] 
        if (close(fd[0]) == -1){
            perror("close child fd[0]");
            exit(1);
        }
        
        //replace stdout with fd[1]
        if (dup2(fd[1],1) == -1){
            perror("dup2");
            exit(1);
        }
        
        //close unused fd[1]
        if (close(fd[1]) == -1){
            perror("close child fd[1]");
            exit(1);
        }
        
        //execute "uniq < tmp.txt"
        if (execlp("/bin/sh", "bin/sh", "-c", "uniq < tmp.txt", NULL) == -1){
            perror("execlp 'uniq < tmp.txt'");
            exit(1);
        }

    //parent process
    } else {
        //close unused fd[1] 
        if (close(fd[1]) == -1){
            perror("close parent fd[1]");
            exit(1);
        }
        
        //replace stdin with fd[0]
        if (dup2(fd[0],0) == -1){
            perror("dup2");
            exit(1);
        }
        
        //close unused fd[0]
        if (close(fd[0]) == -1){
            perror("close parent fd[0]");
            exit(1);
        }
        
        //execute "wc -l"
        if (execlp("wc", "wc", "-l", NULL) == -1){
            perror("execlp 'wc -l'");
            exit(1);
        }
    }

    return 0;
}
