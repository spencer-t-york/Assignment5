#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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
