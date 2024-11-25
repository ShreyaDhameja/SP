#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int main() {
    // Arrays to store opcodes, literals, and symbols
    char mnemonics[MAX][10], types[MAX][3], literals[MAX][10], symbols[MAX][10];
    int opcodes[MAX], lit_addresses[MAX], sym_addresses[MAX];
    int opcode_count = 0, literal_count = 0, symbol_count = 0;

    // Initialize opcode data
    char *opcodes_data[][3] = {{"START", "AD", "1"}, {"END", "AD", "2"}, {"LTORG", "AD", "5"}, {"DS", "DL", "1"},
                                {"DC", "DL", "2"}, {"MOVER", "IS", "1"}, {"MOVEM", "IS", "2"}, {"ADD", "IS", "3"},
                                {"SUB", "IS", "4"}, {"MULT", "IS", "5"}, {"DIV", "IS", "6"}, {"STOP", "IS", "0"}};

    // Load opcodes into arrays
    for (int i = 0; i < 12; i++) {
        strcpy(mnemonics[opcode_count], opcodes_data[i][0]);
        strcpy(types[opcode_count], opcodes_data[i][1]);
        opcodes[opcode_count] = atoi(opcodes_data[i][2]);
        opcode_count++;
    }

    // Load literals from file
    FILE *lit_file = fopen("LitTab.txt", "r");
    if (!lit_file) {
        printf("Error opening LitTab.txt\n");
        return 1;
    }
    while (fscanf(lit_file, "%d %s %d", &literal_count, literals[literal_count], &lit_addresses[literal_count]) != EOF) {
        lit_addresses[literal_count] = literal_count + 1;
        literal_count++;
    }
    fclose(lit_file);

    // Load symbols from file
    FILE *sym_file = fopen("SymTab.txt", "r");
    if (!sym_file) {
        printf("Error opening SymTab.txt\n");
        return 1;
    }
    while (fscanf(sym_file, "%d %s %d", &symbol_count, symbols[symbol_count], &sym_addresses[symbol_count]) != EOF) {
        symbol_count++;
    }
    fclose(sym_file);

    // Open files for reading and writing
    FILE *file = fopen("myfile.txt", "r");
    FILE *intermediate = fopen("IntermediateCode.txt", "w");
    if (!file || !intermediate) {
        printf("Error opening files\n");
        return 1;
    }

    char line[256], label[10], instruc[10], op1[10], op2[10];
    int lc = 0, start = 0;

    // Process each line
    while (fgets(line, MAX, file)) {
        int tokens = sscanf(line, "%s %s %s %s", label, instruc, op1, op2);

        if (tokens > 0) {
            if (strcmp(label, "START") == 0) { // Handle START
                start = atoi(instruc);
                lc = start;
                fprintf(intermediate, "%d\t(AD, 01)\t(C, %d)\n", lc, start);
                continue;
            }

            // Find opcode index
            int opcode_index = -1;
            for (int i = 0; i < opcode_count; i++) {
                if (strcmp(mnemonics[i], label) == 0 || strcmp(mnemonics[i], instruc) == 0) {
                    opcode_index = i;
                    break;
                }
            }

            if (opcode_index == -1) {
                printf("Unknown instruction: %s\n", instruc);
                continue;
            }

            if (strcmp(types[opcode_index], "IS") == 0) { // Instruction
                if (op1[0] == '=') { // Literal operand
                    int op1_index = -1;
                    for (int i = 0; i < literal_count; i++) {
                        if (strcmp(op1, literals[i]) == 0) {
                            op1_index = i + 1; // Return the 1-based index
                            break;
                        }
                    }
                    fprintf(intermediate, "%d\t(IS, %02d)\t(1)\t(L, %d)\n", lc, opcodes[opcode_index], op1_index);
                } else { // Symbol operand
                    int sym_addr = -1;
                    for (int i = 0; i < symbol_count; i++) {
                        if (strcmp(op1, symbols[i]) == 0) {
                            sym_addr = sym_addresses[i];
                            break;
                        }
                    }

                    if (sym_addr == -1) {
                        strcpy(symbols[symbol_count], op1);
                        sym_addresses[symbol_count] = lc + 1;
                        sym_addr = sym_addresses[symbol_count++];
                    }
                    fprintf(intermediate, "%d\t(IS, %02d)\t(1)\t(S, %d)\n", lc, opcodes[opcode_index], sym_addr);
                }
            } else if (strcmp(types[opcode_index], "DL") == 0) { // Data Literal
                fprintf(intermediate, "%d\t(DL, %02d)\t(C, %s)\n", lc, opcodes[opcode_index], op1);
            } else if (strcmp(types[opcode_index], "AD") == 0) { // Address
                fprintf(intermediate, "%d\t(AD, %02d)\n", lc, opcodes[opcode_index]);
            }
        }
        lc++;
    }

    fclose(file);
    fclose(intermediate);
    return 0;
}
