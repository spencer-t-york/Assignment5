//#include "traverse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// DEFINE STRUCTS
typedef struct superBlock {
    unsigned short s_ninodes;        // number of nodes
    unsigned short s_nzones;         // number of zones
    unsigned short s_imap_blocks;    // number of i-node bit map blocks
    unsigned short s_zmap_blocks;    // number of zone bit map blocks
    unsigned short s_firstdatazone;  // first data zone
    unsigned short s_log_zone_size;  // log 2(zonesize/blocksize)
    unsigned int s_max_size;         // max file size
    unsigned short s_magic;          // magic number (indicates which minix version i think?)
    unsigned short s_state;          // clean unmount or no (clean or dirty)
    unsigned int s_zones;            // total number of zones (data blocks) in the filesystem
} superBlock;

// inode struct
typedef struct inode {
    unsigned short i_mode;           // file type and rwx bit
    unsigned short i_uid;            // identifies the use who owns the file
    unsigned int i_size;             // number of bytes in the file (file size)
    unsigned int i_time;             // in seconds, since Jan 1, 1970
    unsigned char i_gid;             // Directories lisiting this i-node/owner's group
    unsigned char i_nlinks;          // number of hard links attached to the inode
    unsigned short i_zone[9];        // pointers to data blocks
} inode;

// help
void help(); 		// lists all commands

// minimount
int minimount(char *); 	// 0 on success, 1 on error

// miniumount
int miniumount(int); 	// 0 on success, 1 on error

// showsuper
void showsuper(int);	// prints values of properties in superblock

// traverse
void traverse(int);

// showzone
// -------- 		// not sure yet

// showfile
// -------- 		// not sure yet

// quit 		// this doesn't actually need a method


int main(int argc, char *argv[]) {
    printf("\n...help()...\n");
    help();

    printf("\n...minimount()...\n");
    int fd = minimount(argv[1]);
    //int fd = minimount("imagefile.img");
    if (fd == -1) {
        printf("invalid file descriptor");
    }
    printf("FD: %d", fd);

    printf("\n...showsuper()...\n");
    showsuper(fd);

    printf("\n...traverse()...\n");
    traverse(fd);

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
void traverse(int fd) {
    // goal is to read root inode
    inode inode;
    superBlock superblock;
    
    // read superblock and use its properties to calculate position of inode table
    lseek(fd, 1024, SEEK_SET);
    if (read(fd, &superblock, sizeof(superblock)) != sizeof(superblock)) {
        perror("Could not read disk image to find superblock (from traverse)");
        return; // exit function
    }
    
    // calculate position of inode table
    //int block_size = superblock.s_log_zone_size;
    //int first_inode_index = block_size * 4; // in bytes
    lseek(fd, 1024 * superblock.s_firstdatazone, SEEK_SET); // based on diagram, inodes start at the 5th block
    
    // read the root inode
    if (read(fd, &inode, sizeof(inode)) != sizeof(inode)) {
        perror("Could not read disk image to find inode (from traverse)");
        return; // exit function
    }
    
    // print inode data
    printf("\ninode data:\n");
    printf("i_mode:   %hu\n", inode.i_mode);
    printf("i_uid:    %hu\n", inode.i_uid);
    printf("i_size:   %u\n", inode.i_size);
    printf("i_time:   %u\n", inode.i_time);
    printf("i_gid:    %u\n", inode.i_gid);
    printf("i_nlinks: %u\n", inode.i_nlinks);
    printf("i_zone(%d):   \n");
    for (int i = 0; i < sizeof(inode.i_zone)/sizeof(inode.i_zone[0]); i++) {
        printf("i_mode[%d]:   %hu\n", i, inode.i_zone[i]); 
    }

    // do this recursively maybe?
}

// showzone
//     Not sure what this is yet...
// showfile
//     Not sure what this is yet...
// quit
//     just break out of program loop
