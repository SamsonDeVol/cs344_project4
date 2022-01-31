//Samson DeVol, cs344, 1/30/22
//c program to re direct outputs into a file

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

  // check for correct number of arguments
  if (argc < 3){
    printf("not enough arguments, correct format is: ./redirout (output file) (commands)");
    exit(1);
  }

  else{
    // open the output file
    int ofd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(ofd == -1){
      perror("open file");
    }

    // create pipe file descriptors
    int pfd[2];
    // pipe the parent
    pipe(pfd);

    // fork the parent
    pid_t pid = fork();

    // run the child process
    if(pid == 0){
      // duplicate the stdout to file descriptor 1
      dup2(pfd[1], 1);
      close(pfd[0]);
      // get the third element in argv (the commands)
      char * const * s = argv + 2;
      // execute the commands given at command line
      execvp(argv[2], s);
      perror("exec");
      exit(1);
    }

    // run the parent program
    close(pfd[1]);
    // set up the buffer
    char buf[248];
    int count = 1; 

    // keep reading until out of commands and read returns 0
    while (count > 0){
      count = read(pfd[0], buf, sizeof buf);
      // write for each read
      int wfd = write(ofd, buf, count);
      if(wfd == -1){
        perror("open write file");
      }
    }

    // close open file descriptors
    close(pfd[0]);
    close(ofd);
  }

}
