#include "front.c"
#include "macro_handle.c" 
#include <stdlib.h>
#include <stdio.h>

/* Function to convert the assembly file to a string */
char* convert_assembly_to_string(const char *filename) {
    FILE *file = fopen(filename, "r");
    long length;  /* Move the declaration to the top */
    char *buffer;

    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);  /* Now this line is fine */
    fseek(file, 0, SEEK_SET);

    buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

int main() {
    int i;
    const char *filename = "ps.as"; /* The input assembly file */
    char *assembly_code = convert_assembly_to_string(filename);
    struct string_sep_result ssr;

    if (assembly_code == NULL) {
        fprintf(stderr, "Error reading assembly code.\n");
        return 1;
    }

    initialize_macros();
    process_macros_from_string(assembly_code); 

    ssr = string_sep(assembly_code);

    for (i = 0; i < ssr.strings_count; i++) {
    char *str = trim_and_remove_commas(ssr.strings[i]);
    int instr_index = is_instruction(str);
    int opcode_index = is_opcode(str);
    int reg_index = is_reg(str);

    if (instr_index >= 0) {
        printf("Instruction found: %s\n", str);
    } else if (opcode_index >= 0) {
        printf("Opcode found: %s\n", str);
    } else if (reg_index >= 0) {
        printf("Register found: %s\n", str);
    } else if (is_macro(str)) {  /* Check if it's a macro */
        printf("Macro found: %s\n", str);
    } else if (check_legal_label(str)) {
        printf("Label found: %s\n", str);
    } else {
        int var_address = find_variable(str);
        if (var_address == -1) {
            var_address = add_variable(str);
            printf("New variable found: %s at address %04X\n", str, var_address);
        } else {
            printf("Variable found: %s at address %04X\n", str, var_address);
        }
    }
}

    free_macros();
    free(assembly_code);
    return 0;
}


