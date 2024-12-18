#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LITERALS 100

int main() {
    FILE *file = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 2,3\\asinput.txt", "r");
    if (file == NULL) {
        perror("Error opening input file");
        return 1;
    }
    FILE *sym = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 2,3\\asymbol.txt", "w");
    if (sym == NULL) {
        perror("Error opening symbol file");
        fclose(file);
        return 1;
    }
    fprintf(sym, "%-15s %-22s %s\n", "Opcode", "Symbol", "Address");

    FILE *lit = fopen("C:\\Users\\DELL\\OneDrive\\Desktop\\SP PRACTICAL\\Assignments 2,3\\aliteral.txt", "w");
    if (lit == NULL) {
        perror("Error opening literal file");
        fclose(file);
        fclose(sym);
        return 1;
    }
    fprintf(lit, "%-15s %-10s %s\n", "Opcode", "Literal", "Address");

    char line[256];
    int location_counter = 0;
    int startAdd = 0;

    char literals[MAX_LITERALS][50];   // Array to store literal values
    int literal_addresses[MAX_LITERALS]; // Array to store addresses for each literal
    int literal_count = 0;               // Counter for literals

    // Initialize literal addresses to -1 to indicate "unassigned"
    // for (int i = 0; i < MAX_LITERALS; i++) {
    //     literal_addresses[i] = -1;
    // }

    int symbol_index = 1;  // Symbol numbering
    int literal_index = 1; // Literal numbering

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        char label[50] = "";
        char instruction[50] = "";
        char operand1[50] = "";
        char operand2[50] = "";
        int i;

        // Parse the line for label, instruction, and operands
        int tokens = sscanf(line, "%s %s %s %s", label, instruction, operand1, operand2);

        if (tokens > 0) {
            if (strcmp(label, "START") == 0) {
                startAdd = atoi(instruction);
                location_counter = startAdd;
                printf("START : %d \n", startAdd);
                printf("Location Counter initialized to : %d \n", location_counter);
                continue;
            }

            if (strcmp(instruction, "DS") == 0 || strcmp(instruction, "DC") == 0) {
                printf("Symbol: %s at Location Counter: %d\n", label, location_counter);
                fprintf(sym, "%d %s\t %d\n", symbol_index++, label, location_counter);
            }

            if (operand1[0] == '=') {
                // Store literal in the array but don't assign address yet
                printf("Encountered Literal on line %d: %s\n", location_counter, operand1);
                strcpy(literals[literal_count], operand1);
                literal_addresses[literal_count] = -1; // Mark as unassigned
                literal_count++;
            }

            // Handle LTORG or END: assign addresses to stored literals
            if (strcmp(label, "LTORG") == 0 || strcmp(label, "END") == 0) {
                printf("Processing literals at LTORG/END at Location Counter: %d...\n", location_counter);
                for (i = 0; i < literal_count; i++) {
                    if (literal_addresses[i] == -1) { // Unassigned literal
                        literal_addresses[i] = location_counter;
                        printf("Assigning address %d to Literal: %s\n", location_counter, literals[i]);
                        fprintf(lit, "%-16d %-10s %d\n", literal_index++, literals[i], location_counter);
                        location_counter++; // Increment location counter for each literal
                    }
                }
            }
        } else {
            printf("Line %d is not in expected format.\n", location_counter);
        }

        location_counter++; // Increment location counter for each line read
        printf("Completed Line, Location Counter: %d \n", location_counter);
    }

    fclose(file);
    fclose(sym);
    fclose(lit);
    return 0;
}
