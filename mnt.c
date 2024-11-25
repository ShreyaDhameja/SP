#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_MNT_LINES 100

void writeMNTFile(char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH], int mntCount) {
    FILE *outFile = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\onlyMNT.txt", "w");
    int i;
    if (outFile == NULL) {
        printf("Unable to open MNT file for writing.\n");
        return;
    }
    for (i = 0; i < mntCount; i++) {
        fprintf(outFile, "%s", mnt[i]);
    }
    fclose(outFile);
    printf("MNT written to 2MNT.txt\n");
}

void processMacros(const char *inputFileName) {
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        printf("Unable to open input file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH];
    int mdtIndex = 1;
    int mntCount = 0;

    while (fgets(line, sizeof(line), inFile)) {
        if (strstr(line, "MACRO") != NULL) {
            if (fgets(line, sizeof(line), inFile)) {
                char macroName[MAX_LINE_LENGTH];
                sscanf(line, "%s", macroName);
                snprintf(mnt[mntCount++], MAX_LINE_LENGTH, "%d   %s\n", mdtIndex, macroName);
            }
        }
    }
    fclose(inFile);

    writeMNTFile(mnt, mntCount);
}

int main() {
    const char *inputFileName = "C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\2macro.txt";
    processMacros(inputFileName);
    return 0;
}

