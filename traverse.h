#ifndef TRAVERSE_H
#define TRAVERSE_H

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

// traverse
void traverse(int, int);

// convert mode
void mode_to_str(unsigned short, char *);

// convert number number to letters
const char* numberToMonth(int);

#endif
