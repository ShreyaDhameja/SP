#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_LINES 1000
#define MAX_MDT_LINES 100
#define MAX_MNT_LINES 100

void writeMDTFile(char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH], int mdtCount) {
    FILE *outFile = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\onlyMDT.txt", "w");
    if (outFile == NULL) {
        printf("Unable to open MDT file for writing.\n");
        return;
    }
    int i;
    for (i = 0; i < mdtCount; i++) {
        fprintf(outFile, "%s", mdt[i]);
    }

    fclose(outFile);
    printf("MDT written to onlyMDT.txt\n");
}

void writeMNTFile(char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH], int mntCount) {
    FILE *outFile = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\onlyMNT.txt", "w");
    if (outFile == NULL) {
        printf("Unable to open MNT file for writing.\n");
        return;
    }
    int i;
    for (i = 0; i < mntCount; i++) {
        fprintf(outFile, "%s", mnt[i]);
    }

    fclose(outFile);
    printf("MNT written to onlyMNT.txt\n");
}

void processMacros(const char *inputFileName) {
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        printf("Unable to open input file.\n");
        return;
    }

    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int lineCount = 0;

    // Read the entire file into the 2D array
    while (fgets(lines[lineCount], MAX_LINE_LENGTH, inFile)) {
        lineCount++;
        if (lineCount >= MAX_LINES) {
            printf("File too large to process.\n");
            fclose(inFile);
            return;
        }
    }
    fclose(inFile);

    char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH];
    char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH];

    int mdtIndex = 1;
    int mdtCount = 0;
    int mntCount = 0;
    int i;

    for (i = 0; i < lineCount; i++) {
        if (strstr(lines[i], "MACRO") != NULL) {
            // Extract macro name and add to MNT
            if (i + 1 < lineCount) {
                char macroName[MAX_LINE_LENGTH];
                sscanf(lines[i + 1], "%s", macroName);
                snprintf(mnt[mntCount++], MAX_LINE_LENGTH, "%d   %s\n", mdtIndex, macroName);
                snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, lines[i + 1]);

                i += 2; // Skip MACRO and macro name lines

                // Process macro body until MEND
                while (i < lineCount && strstr(lines[i], "MEND") == NULL) {
                    snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, lines[i]);
                    i++;
                }

                if (i < lineCount && strstr(lines[i], "MEND") != NULL) {
                    snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, lines[i]);
                }
            }
        }
    }

    writeMNTFile(mnt, mntCount);
    writeMDTFile(mdt, mdtCount);
}

int main() {
    const char *inputFileName = "C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 5,6\\2macro.txt";
    processMacros(inputFileName);
    return 0;
}

