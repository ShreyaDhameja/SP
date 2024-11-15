#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_MDT_LINES 100
#define MAX_MNT_LINES 100

void writeMDTFile(char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH], int mdtCount) {
    FILE *outFile = fopen("MDT.txt", "w");
    int i;
    if (outFile == NULL) {
        printf("Unable to open MDT file for writing.\n");
        return;
    }
    for (i = 0; i < mdtCount; i++) {
        fprintf(outFile, "%s", mdt[i]);
    }
    fclose(outFile);
    printf("MDT written to 2MDT.txt\n");
}

void writeMNTFile(char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH], int mntCount) {
    FILE *outFile = fopen("MNT.txt", "w");
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

void writeIntermediateFile(char lines[][MAX_LINE_LENGTH], int startLine, int totalLines) {
    FILE *intermediateFile = fopen("Intermediatecodemacro.txt", "w");
    int i;
    if (intermediateFile == NULL) {
        printf("Unable to open Intermediate file for writing.\n");
        return;
    }
    for (i = startLine; i < totalLines; i++) {
        fprintf(intermediateFile, "%s", lines[i]);
    }
    fclose(intermediateFile);
    printf("Intermediate code written to 2Intermediatecodemacro.txt\n");
}

void writeALTFile(char alt[MAX_MNT_LINES][MAX_LINE_LENGTH], int altCount) {
    FILE *altFile = fopen("ALT.txt", "w");
    int i;
    if (altFile == NULL) {
        printf("Unable to open ALT file for writing.\n");
        return;
    }
    for (i = 0; i < altCount; i++) {
        fprintf(altFile, "%s", alt[i]);
    }
    fclose(altFile);
    printf("ALT written to 2ALT.txt\n");
}

void processMacros(const char *inputFileName) {
    FILE *inFile = fopen(inputFileName, "r");
    if (inFile == NULL) {
        printf("Unable to open input file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char mdt[MAX_MDT_LINES][MAX_LINE_LENGTH];
    char mnt[MAX_MNT_LINES][MAX_LINE_LENGTH];
    char alt[MAX_MNT_LINES][MAX_LINE_LENGTH];
    char allLines[MAX_MNT_LINES][MAX_LINE_LENGTH];
    
    int mdtIndex = 1;
    int mdtCount = 0;
    int mntCount = 0;
    int altCount = 0;
    int lineCount = 0;
    int lastMendLine = -1;

    while (fgets(line, sizeof(line), inFile)) {
        snprintf(allLines[lineCount++], MAX_LINE_LENGTH, "%s", line);

        if (strstr(line, "MACRO") != NULL) {
            int argumentIndex = 1;
            if (fgets(line, sizeof(line), inFile)) {
                snprintf(allLines[lineCount++], MAX_LINE_LENGTH, "%s", line);

                char macroName[MAX_LINE_LENGTH];
                sscanf(line, "%s", macroName);
                snprintf(mnt[mntCount++], MAX_LINE_LENGTH, "%d   %s\n", mdtIndex, macroName);
                snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);

                while (fgets(line, sizeof(line), inFile)) {
                    snprintf(allLines[lineCount++], MAX_LINE_LENGTH, "%s", line);
                    if (strstr(line, "MEND") != NULL) {
                        snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);
                        lastMendLine = lineCount;
                        break;
                    }

                    char tempLine[MAX_LINE_LENGTH];
                    strcpy(tempLine, line);
                    char *token = strtok(tempLine, " ,");
                    while (token != NULL) {
                        if (token[0] == '&') {
                            char *equalPos = strchr(token, '=');
                            if (equalPos != NULL) {
                                *equalPos = '\0'; 
                                char *paramName = token;
                                char *paramValue = equalPos + 1;

                                snprintf(alt[altCount++], MAX_LINE_LENGTH, "%d   %s = %s\n", argumentIndex++, paramName, paramValue);

                                char *pos = strstr(line, paramName);
                                if (pos != NULL) {
                                    strncpy(pos, paramValue, strlen(paramValue));
                                }
                            } else {
                                snprintf(alt[altCount++], MAX_LINE_LENGTH, "%d   %s\n", argumentIndex++, token);
                                char indexedParam[MAX_LINE_LENGTH];
                                snprintf(indexedParam, sizeof(indexedParam), "#%d", argumentIndex - 1);
                                char *pos = strstr(line, token);
                                if (pos != NULL) {
                                    strncpy(pos, indexedParam, strlen(indexedParam));
                                }
                            }
                        }
                        token = strtok(NULL, " ,");
                    }
                    snprintf(mdt[mdtCount++], MAX_LINE_LENGTH, "%d   %s", mdtIndex++, line);
                }
            }
        }
    }
    fclose(inFile);

    writeMDTFile(mdt, mdtCount);
    writeMNTFile(mnt, mntCount);
    writeALTFile(alt, altCount);

    if (lastMendLine != -1 && lastMendLine < lineCount) {
        writeIntermediateFile(allLines, lastMendLine, lineCount);
    } else {
        printf("No intermediate code found after the last MEND.\n");
    }
}

int main() {
    const char *inputFileName = "macro.txt";
    processMacros(inputFileName);
    return 0;
}
