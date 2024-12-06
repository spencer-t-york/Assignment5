#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    char filename[100];
    printf("Enter the filename: ");
    scanf("%s", filename);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    unsigned char byte;
    int count = 0;

    while (read(fd, &byte, 1) == 1) {
        printf("%02X ", byte);
        count++;
        if (count % 16 == 0) {
            printf("\n");
        }
    }
    if (count % 16 != 0) {
        for (int i = 0; i < 16 - (count % 16); i++) {
            printf("   ");
        }
        printf("\n");
    }

    close(fd);
    return 0;
}