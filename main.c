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
    struct string_sep_result ssr;  /* Move the declaration to the top */

    if (assembly_code == NULL) {
        fprintf(stderr, "Error reading assembly code.\n");
        return 1;
    }

    /* Initialize macros and process them from the assembly code */
    initialize_macros();
    process_macros_from_string(assembly_code);

    /* Now that macros are processed, we can continue with further parsing and processing */
    ssr = string_sep(assembly_code);  /* Now this line is fine */

    /* Example of processing the separated strings */
    for (i = 0; i < ssr.strings_count; i++) {
        char *str = ssr.strings[i];
        if (is_instruction(str) >= 0) {
            printf("Instruction found: %s\n", str);
        } else if (is_opcode(str) >= 0) {
            printf("Opcode found: %s\n", str);
        } else if (is_reg(str) >= 0) {
            printf("Register found: %s\n", str);
        } else if (check_legal_label(str)) {
            printf("Label found: %s\n", str);
        } else {
            printf("Unrecognized string: %s\n", str);
        }
    }

    free_macros(); /* Clean up allocated memory for macros */
    free(assembly_code); /* Clean up allocated memory for assembly_code */
    return 0;
}
