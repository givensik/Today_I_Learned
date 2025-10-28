#include <stdio.h>

int main(int argc, char *argv[]) {
    int lines = 0;
    int words = 0;
    int characters = 0;
    int i;
    char *buf;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    while(1){
        char line[1024];
        buf = fgets(line, sizeof(line), file);
        if (buf == NULL) break;
        lines++;
    }
    fgets()

    printf("Line Count : %d\n", lines);
    printf("Word Count : %d\n", words);
    printf("Character Count : %d\n", characters);

    return 0;

}