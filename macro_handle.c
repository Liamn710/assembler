#include "macro_handle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Declare macro_list as a static global variable */
/* Pointer to the head of the macro list */
static struct macro *macro_list = NULL;

/* Variable to keep track of the number of macros */
static int macro_count = 0;

/* Function to initialize the macro system */
/* Resets the macro list and macro count */
void initialize_macros() {
    macro_list = NULL;
    macro_count = 0;
}

/* Function to add a new macro */
/* Allocates memory for the new macro, copies its name and text, and adds it to the list */
void add_macro(const char *name, const char *text) {
    struct macro *new_macro = (struct macro *)malloc(sizeof(struct macro));
    if (new_macro == NULL) {
        /* Handle memory allocation error */
        fprintf(stderr, "Memory allocation error in add_macro.\n");
        exit(1);
    }

    /* Copy the macro name and text into the new macro structure */
    strcpy(new_macro->name, name);
    strcpy(new_macro->text, text);

    /* Insert the new macro at the beginning of the list */
    new_macro->next = macro_list;
    macro_list = new_macro;
    macro_count++;
}

/* Function to get the current macro list */
/* Returns a pointer to the head of the macro list */
struct macro* get_macro_list() {
    return macro_list;
}

/* Function to free all allocated memory for macros */
/* Iterates through the macro list and deallocates each macro */
void free_macros() {
    struct macro *current = macro_list;
    while (current != NULL) {
        struct macro *next = current->next;
        free(current);
        current = next;
    }
    /* Reset the macro list and macro count */
    macro_list = NULL;
    macro_count = 0;
}

/* Function to determine the type of a line */
/* Returns the type of the line (macro definition, macro call, etc.) */
enum line_type determine_line_type(const char *line) {
    if (strncmp(line, "macro", 5) == 0) {
        return macro_definition;
    } else if (strncmp(line, "endmacr", 7) == 0) {
        return end_macro_definition;
    } else {
        /* Check if the line matches any macro name in the list */
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

/* Function to process macros within the assembly code */
/* Expands macro calls and replaces them with the macro text */
void process_macros_from_string(char *assembly_code) {
    char line_buffer[LINE_BUFFER_SIZE];
    char macro_name[32];
    char macro_body[1024];
    int inside_macro = 0; /* Flag to indicate if inside a macro definition */
    struct macro *m;
    char expanded_code[4096] = "";  /* Buffer to store the expanded assembly code */
    char *current_line = assembly_code;
    enum line_type type;
    int i;

    /* Initialize the macro body buffer */
    macro_body[0] = '\0';

    /* Process each line of the assembly code */
    while (*current_line) {
        /* Read the current line into the buffer */
        sscanf(current_line, "%[^\n]\n", line_buffer);
        /* Determine the type of the current line */
        type = determine_line_type(line_buffer);

        if (type == macro_definition) {
            /* Start a new macro definition */
            inside_macro = 1;
            sscanf(line_buffer + 6, "%s", macro_name); /* Extract macro name */
            macro_body[0] = '\0'; /* Clear previous macro body */
        } else if (type == end_macro_definition) {
            /* End the macro definition and store it */
            inside_macro = 0;
            add_macro(macro_name, macro_body); /* Add the macro to the list */
        } else if (inside_macro) {
            /* Append the line to the current macro body if inside a macro */
            strcat(macro_body, line_buffer);
            strcat(macro_body, "\n");
        } else if (type == macro_call) {
            /* Replace the macro call with the macro text */
            for (i = 0, m = macro_list; m != NULL; i++, m = m->next) {
                if (strcmp(line_buffer, m->name) == 0) {
                    strcat(expanded_code, m->text);
                    strcat(expanded_code, "\n");
                    break;
                }
            }
        } else {
            /* Copy the line as is if it is not a macro-related line */
            strcat(expanded_code, line_buffer);
            strcat(expanded_code, "\n");
        }

        /* Move to the next line */
        while (*current_line && *current_line++ != '\n');
    }

    /* Copy the expanded code back into the original assembly code buffer */
    strcpy(assembly_code, expanded_code);
}

/* Function to trim leading and trailing spaces and remove commas from a string */
/* Returns the cleaned-up string */
char *trim_and_remove_commas(char *str) {
    char *start = str;
    char *end = str;
    char *dst = str;

    /* Skip leading spaces and tabs */
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    /* Find the end of the string */
    while (*end && *end != '\n') {
        end++;
    }

    /* Move back from the end, skipping trailing spaces, tabs, and commas */
    end--;
    while (end > start && (*end == ' ' || *end == '\t' || *end == ',')) {
        end--;
    }

    /* Copy the trimmed and cleaned-up string to the destination */
    while (start <= end) {
        if (*start != ',') {
            *dst++ = *start;
        }
        start++;
    }
    /* Null-terminate the destination string */
    *dst = '\0';  

    return str;
}
