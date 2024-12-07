#include "traverse.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

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

// directory struct
typedef struct directoryEntry {
    unsigned short inode;	     // holds inode number (i think)
    char name[0];                  // holds file or directory name (i think)
} directoryEntry;

// help
void help(); 		// lists all commands

// minimount
int minimount(char *); 	// 0 on success, 1 on error

// miniumount
int miniumount(int); 	// 0 on success, 1 on error

// showsuper
void showsuper(int);	// prints values of properties in superblock

// traverse
void traverse(int, int);

    // convert mode
    void mode_to_str(unsigned short, char *);

    // convert number number to letters
    const char* numberToMonth(int);

// showzone
void showzone(int);	// not sure yet

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
    traverse(fd, 0); // l flag unset
    traverse(fd, 1); // l flag set

    printf("\n...showzone()...\n");
    showzone(fd);

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
void traverse(int fd, int l_flag) {
    superBlock superblock;
    inode inode;

    // read superblock and use its properties to calculate position of inode table
    lseek(fd, 1024, SEEK_SET);
    if (read(fd, &superblock, sizeof(superblock)) != sizeof(superblock)) {
        perror("Could not read disk image to find superblock (from traverse)");
        return; // exit function
    }
    
    // calculate position of inode table
    int block_size = 1024 << superblock.s_log_zone_size;
    int inode_bitmap_size = superblock.s_imap_blocks * block_size;
    int zone_bitmap_size  = superblock.s_zmap_blocks * block_size;
    int inode_table_pos   = 1024 + block_size + inode_bitmap_size + zone_bitmap_size; 

    lseek(fd, inode_table_pos, SEEK_SET); // based on diagram, first inode starts at inode table
    
    // read the root inode
    if (read(fd, &inode, sizeof(inode)) != sizeof(inode)) {
        perror("Could not read disk image to find inode (from traverse)");
        return; // exit function
    }
    
    // print inode data
    printf("\ninode data:\n");
    printf("i_mode:   %hu\n", inode.i_mode);
    printf("i_uid:    %hu\n", inode.i_uid);
    printf("i_size:   %u\n",  inode.i_size);
    printf("i_time:   %u\n",  inode.i_time);
    printf("i_gid:    %u\n",  inode.i_gid);
    printf("i_nlinks: %u\n",  inode.i_nlinks);
    printf("i_zone:   \n");
    for (int i = 0; i < 9; i++) {
        printf("i_zone[%d]:   %hu\n", i, inode.i_zone[i]); 
    }
    
    int zone_pos = inode.i_zone[0] * block_size; // positon of first zone
    lseek(fd, zone_pos, SEEK_SET);

    char zone_data[block_size]; // will hold the content of data zone
    if (read(fd, zone_data, block_size) != block_size) {
        perror("Could not read disk image to store directory entries");
        return; // exit function
    }

    int offset = 0;
    while (offset < inode.i_size) {
        directoryEntry *entry = (directoryEntry *)(zone_data + offset);

	if (entry->inode != 0) {
	    if (l_flag) {
	        // find and go to inode position
		int inode_pos = inode_table_pos + (entry->inode-1) * sizeof(inode);
		
		if (lseek(fd, inode_pos, SEEK_SET) == -1) {
                    perror("ERROR: lseek failed");
                    continue; // Skip this entry
                }  
		
		if (read(fd, &inode, sizeof(inode)) != sizeof(inode)) {
                   perror("Could not read disk image to find inode for -l flag");
                   return; // exit function
                }

		// convert mode to readable string
		char mode[11];
		mode_to_str(inode.i_mode, mode);

		// convert time
		time_t time = inode.i_time;
		struct tm *timestruct;
		timestruct = localtime(&time);

		int year  = timestruct->tm_year + 1900;
		const char* month = numberToMonth(timestruct->tm_mon + 1);
	        int day   = timestruct->tm_mday;	
                
		printf("%s %hu %u %s %d %d %s\n",
                    mode,               // permissions
                    inode.i_uid,        // owner name
                    inode.i_size,       // file size in bytes
                    month, day, year,   // last modified date
                    entry->name         // file or path name
                );

	    } else {
	        printf("%s\n", entry->name);
	    }
	}

    // adds size of directory entry properties and null termiator
	offset += sizeof(entry->inode) + strlen(entry->name) + 1;
        
	// account for minix padding of 16 bytes
	if (offset % 16 != 0) {
	    offset += 16 - (offset % 16);
	}    
    }
}

void mode_to_str(unsigned short mode, char *str) {
    str[0] = (mode & 0x4000) ? 'd' : '-';  // directory indicator
    str[1] = (mode & 0x0100) ? 'r' : '-';  // read  for owner
    str[2] = (mode & 0x0080) ? 'w' : '-';  // write for owner
    str[3] = (mode & 0x0040) ? 'x' : '-';  // exec  for owner
    str[4] = (mode & 0x0020) ? 'r' : '-';  // read  for group
    str[5] = (mode & 0x0010) ? 'w' : '-';  // write for group
    str[6] = (mode & 0x0008) ? 'x' : '-';  // exec  for group
    str[7] = (mode & 0x0004) ? 'r' : '-';  // read  for others
    str[8] = (mode & 0x0002) ? 'w' : '-';  // write for others
    str[9] = (mode & 0x0001) ? 'x' : '-';  // exec  for others
    str[10] = '\0';                        // null terminator
}

const char* numberToMonth(int monthNumber) {
    const char* months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    // Return the corresponding month abbreviation
    return months[monthNumber - 1];
}

// showzone
void showzone(int fd) {
    int zone_number;
    printf("Enter the zone number: ");
    scanf("%d", &zone_number);

    int zone_start = zone_number * 1024;
    char zone_data[1024];

    // Still need to put in data reading mechanism
    if (read(fd, zone_data, sizeof(zone_data)) != sizeof(zone_data)) {
        perror("Could not read disk image for zone_data");
        return; // exit function
    }
    for (int i = 0; i < 1024; i++) {
        if (isprint(zone_data[i])) {
            printf("%c", zone_data[i]);
        }
        else {
            printf(" ");
        }
    }

    printf("\n");
}

// showfile


// quit
//     just break out of program loop
