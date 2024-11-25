#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_MDT_LINES 100

void writeMDT(char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH], int mdtCount) {
    FILE *outFile = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\onlyMDT.txt", "w");
    int i;
    if (outFile == NULL) {
        printf("Unable to open MDT file for writing.\n");
        return;
    }
    for (i = 0; i < mdtCount; i++) {
        fprintf(outFile, "%s", mdt[i]);
    }
    fclose(outFile);
    printf("MDT written to MDT.txt\n");
}

void processMacros(const char *inputFileName) {
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        printf("Unable to open input file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH];
    int mdtCount = 0;
    int mdtIndex = 1; // MDT line indexing

    while (fgets(line, sizeof(line), inFile)) {
        if (strstr(line, "MACRO") != NULL) {
            if (fgets(line, sizeof(line), inFile)) {
                snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);

                while (fgets(line, sizeof(line), inFile)) {
                    if (strstr(line, "MEND") != NULL) {
                        snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);
                        break;
                    }
                    snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);
                }
            }
        }
    }
    fclose(inFile);

    writeMDT(mdt, mdtCount);
}

int main() {
    const char *inputFileName = "C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\2macro.txt";
    processMacros(inputFileName);
    return 0;
}
