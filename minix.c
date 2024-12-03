#include <stdio.h>
#include <unistd.h>

// DEFINE FUNCTIONS
// help
void help();
// minimount
int minimount
// miniumount
// showsuper
// traverse
// showzone
// showfile
// quit

// make the superblock struct
// make the inode struct

int main(int argc, char *argv[]) {
  help();
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
  printf("\n   quit          Quits the minix console");

}
// minimount
//     this opens the disk image file for reading with open()
// miniumount
//     this closes the disk image file using close(fd)
// showsuper
//     this just calls read(fd, superblock, sizeof(struct superblock));
//     and then a bunch of print statements to print out all of the properties of the superblock
// traverse
//     I think this is just ls command which I've done already in CIS 340
// showzone
//     Not sure what this is yet...
// showfile
//     Not sure what this is yet...
// quit
//     just break out of program loop
