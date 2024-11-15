#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 100
#define MAX_SYMBOLS 100
#define MAX_ADDRESS_SENSITIVE_INSTRUCTIONS 100

typedef struct {
    int original_address;
    int relocation_factor;
    int relocated_address;
    int is_address_sensitive; 
    char instruction[MAX_LINE_LENGTH]; 
} RelocationEntry;

typedef struct {
    char name[MAX_LINE_LENGTH];
    int address;
    int linked_address; 
    char type[3]; 
} Symbol;

void extract_symbols(char assembly_code[MAX_LINES][MAX_LINE_LENGTH], int line_count, Symbol symbol_table[MAX_SYMBOLS], int *symbol_count, char address_sensitive_instructions[MAX_ADDRESS_SENSITIVE_INSTRUCTIONS][MAX_LINE_LENGTH], int *address_sensitive_count) {
    int address = 100; 
    int i;

    for (i = 0; i < line_count; i++) {
        char *line = assembly_code[i];

        if (strlen(line) == 0 || line[0] == ';') {
            continue;
        }

        if (strncmp(line, "START", 5) == 0) {
            sscanf(line, "START %d", &address);  
            continue;
        }

        if (strncmp(line, "ENTRY", 5) == 0) {
            char symbol_name[MAX_LINE_LENGTH];
            sscanf(line, "ENTRY %s", symbol_name); 
            
            strncpy(symbol_table[*symbol_count].name, symbol_name, MAX_LINE_LENGTH);
            symbol_table[*symbol_count].address = address;
            strncpy(symbol_table[*symbol_count].type, "PD", 3);
            (*symbol_count)++;
            continue; 
        }

        else if (strstr(line, "DS") || strstr(line, "DC")) {
            char symbol_name[MAX_LINE_LENGTH];
            sscanf(line, "%s", symbol_name); 
            strncpy(symbol_table[*symbol_count].name, symbol_name, MAX_LINE_LENGTH);
            symbol_table[*symbol_count].address = address;

            strncpy(symbol_table[*symbol_count].type, "PD", 3);
            (*symbol_count)++;
        }

        if (strstr(line, "LOAD") || strstr(line, "MOVEM") || strstr(line, "SUB") || strstr(line, "CMP") || strstr(line, "ADD")) {
            strncpy(address_sensitive_instructions[*address_sensitive_count], line, MAX_LINE_LENGTH);
            (*address_sensitive_count)++;
        }

        address++;
    }
}

void calculate_relocation(int link_origin, int start_origin, char assembly_code[MAX_LINES][MAX_LINE_LENGTH], int line_count, Symbol symbol_table[MAX_SYMBOLS], int symbol_count, char address_sensitive_instructions[MAX_ADDRESS_SENSITIVE_INSTRUCTIONS][MAX_LINE_LENGTH], int address_sensitive_count, FILE *output_file) {
    int relocation_factor = link_origin - start_origin;

    int address = start_origin;
    RelocationEntry relocation_table[MAX_LINES];
    int entry_count = 0;
    int i, j;

    for (i = 0; i < line_count; i++) {
        char *line = assembly_code[i];

        if (strlen(line) == 0 || line[0] == ';') {
            continue;
        }

        if (strncmp(line, "START", 5) == 0) {
            sscanf(line, "START %d", &address);  
            continue;
        }

        int is_address_sensitive = 0;

        for (j = 0; j < address_sensitive_count; j++) {
            if (strstr(line, address_sensitive_instructions[j]) != NULL) {
                is_address_sensitive = 1;
                break;
            }
        }

        if (is_address_sensitive) {
            relocation_table[entry_count].original_address = address;
            relocation_table[entry_count].relocation_factor = relocation_factor;
            relocation_table[entry_count].relocated_address = address + relocation_factor;
            relocation_table[entry_count].is_address_sensitive = is_address_sensitive;
            entry_count++;
        }
        
        address++;
    }

    fprintf(output_file, "%-20s\n", "Relocated Address");
    fprintf(output_file, "%s\n", "--------------------");
    for (j = 0; j < entry_count; j++) {
        fprintf(output_file, "%-20d\n", relocation_table[j].relocated_address);
    }
}

int main() {
    int link_origin, start_origin = 100;
    char assembly_code[MAX_LINES][MAX_LINE_LENGTH];
    Symbol symbol_table[MAX_SYMBOLS];
    char address_sensitive_instructions[MAX_ADDRESS_SENSITIVE_INSTRUCTIONS][MAX_LINE_LENGTH];
    int line_count = 0, symbol_count = 0, address_sensitive_count = 0;
    int j;

    printf("Enter the link origin: ");
    scanf("%d", &link_origin);

    getchar(); 
    
    FILE *file = fopen("inputfile.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while (line_count < MAX_LINES && fgets(assembly_code[line_count], MAX_LINE_LENGTH, file)) {
        assembly_code[line_count][strcspn(assembly_code[line_count], "\n")] = 0;
        line_count++;
    }

    fclose(file);

    extract_symbols(assembly_code, line_count, symbol_table, &symbol_count, address_sensitive_instructions, &address_sensitive_count);
    
	for (j = 0; j < symbol_count; j++) {
        symbol_table[j].linked_address = symbol_table[j].address + (link_origin - start_origin);
    }

    FILE *symbol_file = fopen("linktable.txt", "w");
    if (symbol_file == NULL) {
        perror("Error opening symbol output file");
        return EXIT_FAILURE;
    }

    fprintf(symbol_file, "%-20s %-20s %-20s\n", "Symbol", "Address", "Type");
fprintf(symbol_file, "%s\n", "----------------------------------------------");
for (j = 0; j < symbol_count; j++) {
    fprintf(symbol_file, "%-20s %-20d %-20s\n", symbol_table[j].name, symbol_table[j].address, symbol_table[j].type);
}

    fclose(symbol_file);

    FILE *output_file = fopen("relocationtable.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    calculate_relocation(link_origin, start_origin, assembly_code, line_count, symbol_table, symbol_count, address_sensitive_instructions, address_sensitive_count, output_file);

    fclose(output_file);

    return 0;
}
