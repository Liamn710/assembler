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
    int i, j;
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
        int binary_words[MAX_BINARY_WORDS];
        int binary_word_count = 0;

        /* Process the current line */
        process_line(str, binary_words, &binary_word_count);

        /* Print the binary words for the current line */
        printf("Line %d: ", i + 1);
        for (j = 0; j < binary_word_count; j++) {
            print_binary(binary_words[j]); /* Print each binary word */
            printf(" ");
        }
        printf("\n"); /* Newline to separate each line's output */
    }

    free_macros();
    free(assembly_code);
    return 0;
}


