#include "macro_handle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Declare macro_list as a static global variable */
static struct macro *macro_list = NULL;
static int macro_count = 0; /* Keep track of the number of macros */

void initialize_macros() {
    macro_list = NULL;
    macro_count = 0;
}

void add_macro(const char *name, const char *text) {
    struct macro *new_macro = (struct macro *)malloc(sizeof(struct macro));
    if (new_macro == NULL) {
        fprintf(stderr, "Memory allocation error in add_macro.\n");
        exit(1);
    }

    strcpy(new_macro->name, name);
    strcpy(new_macro->text, text);
    new_macro->next = macro_list;
    macro_list = new_macro;
    macro_count++;
}

struct macro* get_macro_list() {
    return macro_list;  /* Return the pointer to the macro list */
}

void free_macros() {
    struct macro *current = macro_list;
    while (current != NULL) {
        struct macro *next = current->next;
        free(current);
        current = next;
    }
    macro_list = NULL;
    macro_count = 0;
}

enum line_type determine_line_type(const char *line) {
    if (strncmp(line, "macro", 5) == 0) {
        return macro_definition;
    } else if (strncmp(line, "endmacr", 7) == 0) {
        return end_macro_definition;
    } else {
        struct macro *m = macro_list;
        while (m != NULL) {
            if (strcmp(line, m->name) == 0) {
                return macro_call;
            }
            m = m->next;
        }
        return any_other_line;
    }
}

void process_macros_from_string(char *assembly_code) {
    char line_buffer[LINE_BUFFER_SIZE];
    char macro_name[32];
    char macro_body[1024];
    int inside_macro = 0;
    struct macro *m;
    char expanded_code[4096] = "";  /* Initialize expanded_code */
    char *current_line = assembly_code;
    enum line_type type;
    int i;

    macro_body[0] = '\0';

    while (*current_line) {
        sscanf(current_line, "%[^\n]\n", line_buffer);
        type = determine_line_type(line_buffer);

        if (type == macro_definition) {
            inside_macro = 1;
            sscanf(line_buffer + 6, "%s", macro_name); /* Capture macro name */
            macro_body[0] = '\0'; /* Clear previous macro body */
        } else if (type == end_macro_definition) {
            inside_macro = 0;
            add_macro(macro_name, macro_body); /* Store the macro */
        } else if (inside_macro) {
            strcat(macro_body, line_buffer);
            strcat(macro_body, "\n");
        } else if (type == macro_call) {
            /* Replace with macro text */
            for (i = 0, m = macro_list; m != NULL; i++, m = m->next) {
                if (strcmp(line_buffer, m->name) == 0) {
                    strcat(expanded_code, m->text);
                    strcat(expanded_code, "\n");
                    break;
                }
            }
        } else {
            strcat(expanded_code, line_buffer);
            strcat(expanded_code, "\n");
        }

        /* Move to the next line */
        while (*current_line && *current_line++ != '\n');
    }

    strcpy(assembly_code, expanded_code);
}


char *trim_and_remove_commas(char *str) {
    char *out = str;  /* Pointer for the output string */
    char *in = str;   /* Pointer for the input string */

    /* Skip leading whitespace */
    while (*in == ' ' || *in == '\t') {
        in++;
    }

    /* Iterate through the string */
    while (*in != '\0') {
        if (*in != ' ' && *in != '\t' && *in != ',' && *in != '\n') {
            *out++ = *in;  /* Copy character if it's not a space, tab, comma, or newline */
        }
        in++;
    }

    /* Null-terminate the string */
    *out = '\0';

    /* Remove trailing whitespace */
    while (--out >= str && (*out == ' ' || *out == '\t')) {
        *out = '\0';
    }

    return str;
}
