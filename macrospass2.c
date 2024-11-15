#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_MDT_LINES 100
#define MAX_MNT_LINES 100
#define MAX_ALT_LINES 10 
#define MAX_EXPANDED_LINES 500

void readFileToArray(const char *fileName, char array[][MAX_LINE_LENGTH], int *count) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Unable to open file: %s\n", fileName);
        return;
    }
    char line[MAX_LINE_LENGTH];
    *count = 0;
    while (fgets(line, sizeof(line), file)) {
        strcpy(array[(*count)++], line);
    }
    fclose(file);
}

void writeExpandedCodeFile(char expanded[MAX_EXPANDED_LINES][MAX_LINE_LENGTH], int expandedCount) {
    FILE *outFile = fopen("ExpandedCode.txt", "w");
    int i;
    if (outFile == NULL) {
        printf("Unable to open ExpandedCode file for writing.\n");
        return;
    }
    for (i = 0; i < expandedCount; i++) {
        fprintf(outFile, "%s", expanded[i]);
    }
    fclose(outFile);
    printf("Expanded code written to 2ExpandedCode.txt\n");
}

int findMacroDefinition(char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH], int mntCount, const char *macroName) {
    int i;
    for (i = 0; i < mntCount; i++) {
        char name[MAX_LINE_LENGTH];
        int mdtIndex;
        sscanf(mnt[i], "%d %s", &mdtIndex, name);
        if (strcmp(name, macroName) == 0) {
            return mdtIndex;
        }
    }
    return -1;
}

void substituteArguments(char *line, char actualArgs[MAX_ALT_LINES][MAX_LINE_LENGTH], int actualArgCount) {
    char placeholder[MAX_LINE_LENGTH];
    int i;
    
    for (i = 0; i < actualArgCount; i++) {
        sprintf(placeholder, "#%d", i + 1);  
        char *pos = strstr(line, placeholder);
        while (pos != NULL) {
            char temp[MAX_LINE_LENGTH];
            strncpy(temp, line, pos - line);
            temp[pos - line] = '\0';
            strcat(temp, actualArgs[i]);
            strcat(temp, pos + strlen(placeholder));
            strcpy(line, temp);
            pos = strstr(line, placeholder);  
        }
    }
}

void expandMacro(char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH], int mdtIndex, char actualArgs[MAX_ALT_LINES][MAX_LINE_LENGTH], int actualArgCount, char expanded[MAX_EXPANDED_LINES][MAX_LINE_LENGTH], int *expandedCount) {
    while (strstr(mdt[mdtIndex], "MEND") == NULL) {
        char line[MAX_LINE_LENGTH];
        strcpy(line, mdt[mdtIndex]);

        char *instructionStart = strchr(line, ' ');
        if (instructionStart != NULL) {
            strcpy(line, instructionStart + 1);  
        }

        substituteArguments(line, actualArgs, actualArgCount);

        strcpy(expanded[(*expandedCount)++], line);  
        mdtIndex++;  
    }
}

void processPass2(const char *intermediateFileName, char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH], int mntCount, char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH]) {
    FILE *intermediateFile = fopen(intermediateFileName, "r");
    if (intermediateFile == NULL) {
        printf("Unable to open intermediate file.\n");
        return;
    }

    char expanded[MAX_EXPANDED_LINES][MAX_LINE_LENGTH];
    int expandedCount = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), intermediateFile)) {
        char macroName[MAX_LINE_LENGTH];
        int foundMacro = 0;

        sscanf(line, "%s", macroName);
        int mdtIndex = findMacroDefinition(mnt, mntCount, macroName);
        if (mdtIndex != -1) {
            foundMacro = 1;

            char *start = strchr(line, ' ');
            char actualArgs[MAX_ALT_LINES][MAX_LINE_LENGTH];
            int actualArgCount = 0;
            if (start != NULL) {
                start++;
                char *token = strtok(start, " ,\n");
                while (token != NULL) {
                    strcpy(actualArgs[actualArgCount++], token);
                    token = strtok(NULL, " ,\n");
                }
            }

            expandMacro(mdt, mdtIndex, actualArgs, actualArgCount, expanded, &expandedCount);
        }

        if (!foundMacro) {
            strcpy(expanded[expandedCount++], line);
        }
    }

    fclose(intermediateFile);

    writeExpandedCodeFile(expanded, expandedCount);
}

int main() {
    char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH];
    char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH];

    int mntCount, mdtCount;

    readFileToArray("MNT.txt", mnt, &mntCount);
    readFileToArray("MDT.txt", mdt, &mdtCount);

    processPass2("Intermediatecodemacro.txt", mnt, mntCount, mdt);

    return 0;
}
