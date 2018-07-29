
#include <stdio.h>      /* perror */
#include <stdlib.h>     /* exit */
#include <sys/types.h>  /* open, write, read */
#include <sys/stat.h>   /* open */
#include <fcntl.h>      /* open */
#include <sys/uio.h>    /* write */
#include <unistd.h>     /* close, write, read */

#define OUTFILE         "concat.txt"    //default output file
#define BUFSIZE         1024


int main(int argc, char *argv[]) {

    int ifd, ofd;         //input file descriptor and output file descriptor
    ssize_t cc;           //the number of characters by read-systemcall
    char buf[BUFSIZE];    //buffer to store characters by read-systemcall
    
    //tell when lack of the comannd line arguments
    if (argc < 2){
        fprintf(stderr, "argc error: Please input more than 2 INFILE\n");
        exit(1);
    }
    
    /*open concat.txt with delete all contents.
        If not exist, create without create-systemcall*/
    if ((ofd = open(OUTFILE, O_WRONLY|O_TRUNC|O_CREAT, 0666)) == -1) {
        perror(OUTFILE);
        exit(1); 
    }
    
    /* open, read and write one by one*/
    for (int i=1; i<argc; i++){
        //open input file
        if ((ifd = open(argv[i], O_RDONLY)) ==-1){
            perror(argv[i]);
            exit(1);
        }
        
        //loop until there're no charactors to read in ifd
        while ((cc = read(ifd, buf, BUFSIZE)) > 0){
            //write buf on ofd
            if (write(ofd, buf, cc) == -1){
                perror("wirte");
                exit(1);
            }
        }
        
        //check whether or not read correctly 
        if (cc == -1){
            perror("read");
            exit(1);
        }
        
        //close input file
        if (close(ifd) == -1){
            perror("close INFILE");
            exit(1);
        }
    }

    //close concat.txt
    if (close(ofd) == -1){
        perror("close OUTFILE");
        exit(1);
    }
    
    printf("Done! Check %s!\n", OUTFILE);
    
    return 0;
}
