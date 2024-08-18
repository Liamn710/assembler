#include "front.h"
#include "macro_handle.h"
#include <stdlib.h>
#include <stdio.h>

char* convert_assembly_to_string(const char *filename) {
    FILE *file = fopen(filename, "r");
    long length;
    char *buffer;

    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
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
    const char *filename = "ps.as";
    char *assembly_code = convert_assembly_to_string(filename);
    struct string_sep_result ssr;
    FILE *output_file;

    if (assembly_code == NULL) {
        fprintf(stderr, "Error reading assembly code.\n");
        return 1;
    }

    output_file = fopen("output.ob", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        free(assembly_code);
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
        int label_value = is_label(str);

        if (instr_index >= 0) {
            fprintf(output_file, "Instruction found: %s\n", str);
        } else if (label_value >= 0) {
            fprintf(output_file, "Label found: %s at address %i\n", str, label_value);
        } else if (opcode_index >= 0) {
            fprintf(output_file, "Opcode found: %s\n", str);
        } else if (reg_index >= 0) {
            fprintf(output_file, "Register found: %s\n", str);
        } else if (is_macro(str)) {
            fprintf(output_file, "Macro found: %s\n", str);
        } else if (check_legal_label(str)) {
            fprintf(output_file, "Label found: %s\n", str);
        } else {
            int var_address = find_variable(str);
            if (var_address == -1) {
                var_address = add_variable(str);
                fprintf(output_file, "New variable found: %s at address %04X\n", str, var_address);
            } else {
                fprintf(output_file, "Variable found: %s at address %04X\n", str, var_address);
            }
        }
    }

    fclose(output_file);

    free_macros();
    free(assembly_code);
    return 0;
}