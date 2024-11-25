#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_ALT_LINES 100

void writeALT(char alt[MAX_ALT_LINES][MAX_LINE_LENGTH], int altCount) {
    FILE *outFile = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\onlyALT.txt", "w");
    int i;
    if (outFile == NULL) {
        printf("Unable to open ALT file for writing.\n");
        return;
    }
    for (i = 0; i < altCount; i++) {
        fprintf(outFile, "%s", alt[i]);
    }
    fclose(outFile);
    printf("ALT written to ALT.txt\n");
}

void processALT(const char *inputFileName) {
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        printf("Unable to open input file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char alt[MAX_ALT_LINES][MAX_LINE_LENGTH];
    int altCount = 0;

    while (fgets(line, sizeof(line), inFile)) {
        if (strstr(line, "MACRO") != NULL) {
            if (fgets(line, sizeof(line), inFile)) {
                char tempLine[MAX_LINE_LENGTH];
                strcpy(tempLine, line);

                char *token = strtok(tempLine, " ,\n");
                int argumentIndex = 1;
                while (token != NULL) {
                    if (token[0] == '&') {
                        snprintf(alt[altCount++], MAX_LINE_LENGTH, "%d   %s\n", argumentIndex++, token);
                    }
                    token = strtok(NULL, " ,\n");
                }
            }
        }
    }
    fclose(inFile);

    writeALT(alt, altCount);
}

int main() {
    const char *inputFileName = "C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\2macro.txt";
    processALT(inputFileName);
    return 0;
}
