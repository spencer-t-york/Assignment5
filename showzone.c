#include <stdio.h>
#include <ctype.h>

int main() {
    int zone_number;
    printf("Enter the zone number: ");
    scanf("%d", &zone_number);

    int zone_start = zone_number * 1024;
    char zone_data[1024];

    // Still need to put in data reading mechanism

    for (int i = 0; i < 1024; i++) {
        if (isprint(zone_data[i])) {
            printf("%c", zone_data[i]);
        }
        else {
            printf(" ");
        }
    }

    printf("\n");
    return 0;
}