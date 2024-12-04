#include "traverse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// DEFINE FUNCTIONS
// help
void help(); 		// lists all commands

// minimount
int minimount(char *); 	// 0 on success, 1 on error

// miniumount
int miniumount(int); 	// 0 on success, 1 on error

// showsuper
void showsuper();	// prints values of properties in superblock

// showzone
// -------- 		// not sure yet

// showfile
// -------- 		// not sure yet

// quit 		// this doesn't actually need a method

// make the superblock struct
// make the inode struct

int main(int argc, char *argv[]) {
    printf("\n...help()...\n");
    help();
    printf("\n...traverse()...\n");
    //traverse(argc, argv);
    printf("\n...minimount()...\n");
    //int fd = minimount(argv[0]);
    int fd = minimount("imagefile.img");
    printf("FD: %d", fd);
    printf("\n...miniumount()...\n");
    if (fd == -1) {
        printf("invalid file descriptor");
    }
    if (miniumount(fd) == 1) {
        printf("miniumount() failed");
    } else {
        printf("miniumount() success! 👍");
    }
}


// help
void help() {
  printf("These are the minix commands that can used:\n");
  printf("\n  help          show the supported commands in this minix console");
  printf("\n  minimount     mount a local minix disk (opens the disk image file for reading)");
  printf("\n  miniumount    Unmounts the mounted Minix disk");
  printf("\n  showsuper     Displays information about the superblock");
  printf("\n  traverse      Lists the contents of the root directory");
  printf("\n  showzone      Displays the ASCII content of the specified zone number. Non-ASCII values are displayed as spaces");
  printf("\n  showfile      Displays the content of a specified file (in hex) located in the root directory. Assumes the file size is less than 7KB");
  printf("\n  quit          Quits the minix console");
}

// minimount
// this opens the disk image file for reading with open()
int minimount(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("failed to open disk image");
        return -1;
    }
    return fd; // success
}

//  miniumount
//  this closes the disk image file using close(fd)
int miniumount(int fd) {
    if (close(fd) == -1) {
        return 1;
    }
    return 0; // success
}

// showsuper
//     this just calls read(fd, superblock, sizeof(struct superblock));
//     and then a bunch of print statements to print out all of the properties of the superblock
void showsuper() {
    printf("\nthis is from the superblock.\n");
}

// traverse
//     I think this is just ls command which I've done already in CIS 340
//     this is linked already in the traverse.c and .h file
// showzone
//     Not sure what this is yet...
// showfile
//     Not sure what this is yet...
// quit
//     just break out of program loop
