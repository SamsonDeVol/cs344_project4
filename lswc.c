// Samson DeVol, cs344 Project4, 1/27/22
// Pipe one command to another

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  // create pipe file descriptors
  int pfd[2];
  // pipe the parent
  pipe(pfd);

  // fork the parent
  pid_t pid = fork();

  // run the child process
  if (pid == 0) {
    // duplicate the stdin to file descriptor 0
    dup2(pfd[0],0);
    close(pfd[1]);
    // execute the wc -l command
    execlp("wc", "wc", "-l", NULL);
    perror("exec");
    exit(1);

  }
  
  // run the parent process
  
  close(1);
  // dublicate the stdout to file descriptor 1
  dup2(pfd[1],1);
  close(pfd[0]);
  // execute the ls -1a command
  if(argc > 1){
    execlp("ls", "ls", "-1a", argv[1], NULL);
    perror("exec");
    exit(1);
  }
  else{
    execlp("ls", "ls", "-1a", NULL);
    perror("exec");
    exit(1);
    }
  
  }
