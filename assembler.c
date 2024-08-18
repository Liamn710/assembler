#include "front.h"
#include "macro_handle.h"
#include <stdlib.h>
#include <stdio.h>

/* Function to read the contents of a file into a dynamically allocated string */
/* Returns a pointer to the string containing the file contents, or NULL on error */
char* convert_assembly_to_string(const char *filename) {
    FILE *file = fopen(filename, "r");
    long length;
    char *buffer;

    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    /* Move to the end of the file to determine its length */
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* Allocate memory for the file contents plus a null terminator */
    buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    /* Read the file contents into the buffer */
    fread(buffer, 1, length, file);
    buffer[length] = '\0';  /* Null-terminate the string */

    fclose(file);
    return buffer;
}

/* Main function for processing the assembly code */
/* Reads assembly code from a file, processes macros, and identifies instructions, labels, opcodes, registers, and variables */
int main() {
    int i;
    const char *filename = "ps.as";  /* Input assembly file */
    char *assembly_code = convert_assembly_to_string(filename);
    struct string_sep_result ssr;

    if (assembly_code == NULL) {
        fprintf(stderr, "Error reading assembly code.\n");
        return 1;
    }

    /* Initialize macro handling */
    initialize_macros();
    /* Process macros in the assembly code */
    process_macros_from_string(assembly_code);

    /* Separate the assembly code into individual strings */
    ssr = string_sep(assembly_code);

    /* Process each separated string */
    for (i = 0; i < ssr.strings_count; i++) {
        char *str = trim_and_remove_commas(ssr.strings[i]);  /* Clean up the string */
        int instr_index = is_instruction(str);
        int opcode_index = is_opcode(str);
        int reg_index = is_reg(str);
        int label_value = is_label(str);

        if (instr_index >= 0) {
            /* Print the found instruction */
            printf("Instruction found: %s\n", str);
        } else if (label_value >= 0) {
            /* Print the found label and its address */
            printf("Label found: %s at address %i\n", str, label_value);
        } else if (opcode_index >= 0) {
            /* Print the found opcode */
            printf("Opcode found: %s\n", str);
        } else if (reg_index >= 0) {
            /* Print the found register */
            printf("Register found: %s\n", str);
        } else if (is_macro(str)) {
            /* Print the found macro */
            printf("Macro found: %s\n", str);
        } else if (check_legal_label(str)) {
            /* Print the label if it's a legal label */
            printf("Label found: %s\n", str);
        } else {
            /* Handle variables */
            int var_address = find_variable(str);
            if (var_address == -1) {
                /* Add new variable and print its address */
                var_address = add_variable(str);
                printf("New variable found: %s at address %04X\n", str, var_address);
            } else {
                /* Print the address of the existing variable */
                printf("Variable found: %s at address %04X\n", str, var_address);
            }
        }
    }

    /* Free allocated memory for macros and assembly code */
    free_macros();
    free(assembly_code);
    return 0;
}
