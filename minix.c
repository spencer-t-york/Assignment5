#include "traverse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// DEFINE STRUCTS
// superblock struct
typedef struct superBlock {
    unsigned short s_ninodes;
    unsigned short s_nzones;
    unsigned short s_imap_blocks;
    unsigned short s_zmap_blocks;
    unsigned short s_firstdatazone;
    unsigned short s_log_zone_size;
    unsigned int s_max_size;
    unsigned short s_magic;
    unsigned short s_state;
    unsigned int s_zones;
} superBlock;

// inode struct
typedef struct inode {
    unsigned short i_mode;
    unsigned short i_uid;
    unsigned int i_size;
    unsigned int i_time;
    unsigned char i_gid;
    unsigned char i_nlinks;
    unsigned short i_zone[9];
} inode;

// DEFINE FUNCTIONS
// help
void help(); 		// lists all commands

// minimount
int minimount(char *); 	// 0 on success, 1 on error

// miniumount
int miniumount(int); 	// 0 on success, 1 on error

// showsuper
void showsuper(int);	// prints values of properties in superblock

// showzone
// -------- 		// not sure yet

// showfile
// -------- 		// not sure yet

// quit 		// this doesn't actually need a method


int main(int argc, char *argv[]) {
    printf("\n...help()...\n");
    help();

    printf("\n...traverse()...\n");
    //traverse(argc, argv);

    printf("\n...minimount()...\n");
    int fd = minimount(argv[1]);
    //int fd = minimount("imagefile.img");
    if (fd == -1) {
        printf("invalid file descriptor");
    }
    printf("FD: %d", fd);

    printf("\n...showsuper()...\n");
    showsuper(fd);

    printf("\n...miniumount()...\n");
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
void showsuper(int fd) {
    superBlock superblock;

    printf("\nthis is from the superblock.\n");
    lseek(fd, 1024, SEEK_SET);
    if (read(fd, &superblock, sizeof(superblock)) != sizeof(superblock)) {
        perror("Could not read disk image to find superblock");
        return; // exit function
    }

    printf("\ns_ninodes:        %hu\n", superblock.s_ninodes);
    printf("s_nzones:         %hu\n", superblock.s_nzones);
    printf("s_imap_blocks:    %hu\n", superblock.s_imap_blocks);
    printf("s_zmap_blocks:    %hu\n", superblock.s_zmap_blocks);
    printf("s_firstdatazone:  %hu\n", superblock.s_firstdatazone);
    printf("s_log_zone_size:  %hu\n", superblock.s_log_zone_size);
    printf("s_max_size:       %u\n", superblock.s_max_size);
    printf("s_magic:          0x%04x\n", superblock.s_magic);
    printf("s_state:          %hu\n", superblock.s_state);
    printf("s_zones:          %u\n",  superblock.s_zones);
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
