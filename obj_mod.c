#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

int is_empty_line(const char *line) {
	int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (!isspace(line[i])) {
            return 0;
        }
    }
    return 1;
}

int find_start_address(FILE *input_file) {
    char line[MAX_LINE_LENGTH];
    int start_address = -1;

    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        if (strstr(line, "START") != NULL) {
            sscanf(line, "%*s %d", &start_address);
            break;
        }
    }
    return start_address;
}

int count_code_lines(FILE *input_file) {
    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        if (!is_empty_line(line)) {
            count++;
        }
    }
    return count;
}

void append_relocation_table(const char *relocation_filename, FILE *output_file) {
    FILE *relocation_file = fopen(relocation_filename, "r");
    if (!relocation_file) {
        perror("Error opening relocation table file");
        exit(EXIT_FAILURE);
    }
    
    char line[MAX_LINE_LENGTH];
    
    fprintf(output_file, "\nRelocation Table:\n");
    while (fgets(line, MAX_LINE_LENGTH, relocation_file)) {
        fprintf(output_file, "%s", line);
    }

    fclose(relocation_file);
}

void append_link_table(const char *symbol_table_filename, FILE *output_file) {
    FILE *symbol_table_file = fopen(symbol_table_filename, "r");
    if (!symbol_table_file) {
        perror("Error opening symbol table file");
        exit(EXIT_FAILURE);
    }
    
    char line[MAX_LINE_LENGTH];
    
    fprintf(output_file, "\nLink Table:\n");
    while (fgets(line, MAX_LINE_LENGTH, symbol_table_file)) {
        fprintf(output_file, "%s", line);
    }

    fclose(symbol_table_file);
}

void append_machine_code(const char *machine_code_filename, FILE *output_file) {
    FILE *machine_code_file = fopen(machine_code_filename, "r");
    if (!machine_code_file) {
        perror("Error opening machine code file");
        exit(EXIT_FAILURE);
    }
    
    char line[MAX_LINE_LENGTH];
    
    fprintf(output_file, "\nMachine Code:\n");
    while (fgets(line, MAX_LINE_LENGTH, machine_code_file)) {
        fprintf(output_file, "%s", line);
    }

    fclose(machine_code_file);
}

int main() {
    FILE *input_file = fopen("inputfile.txt", "r");
    FILE *output_file = fopen("object_module.txt", "w");
    if (!input_file || !output_file) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    int start_address = find_start_address(input_file);
    if (start_address == -1) {
        fprintf(stderr, "START address not found in om.txt\n");
        exit(EXIT_FAILURE);
    }

    rewind(input_file);  
    int code_size = count_code_lines(input_file);

    int relocation_factor = 100;
    int adjusted_start_address = start_address + relocation_factor;

    fprintf(output_file, "Header:\n");
    fprintf(output_file, "Translated Address: %d\n", start_address);
    fprintf(output_file, "Code Size: %d\n", code_size);
    fprintf(output_file, "Start Address: %d\n", adjusted_start_address);

    append_machine_code("machinecode.txt", output_file);
    append_relocation_table("relocation_table.txt", output_file);
    append_link_table("link_table.txt", output_file);

    fclose(input_file);
    fclose(output_file);

    printf("Object module with header, machine code, relocation table, and link table created successfully in object_module.txt.\n");
    return 0;
}
