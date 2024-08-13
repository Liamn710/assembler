#include "macro_handle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Declare macro_list as a global variable */
static struct macro *macro_list = NULL;

char *my_strdup(const char *str) {
    char *dup;
    size_t len;

    if (str == NULL) return NULL;
    
    len = strlen(str) + 1;  /* Declare and initialize len before executable code */

    dup = (char *)malloc(len);
    if (dup == NULL) return NULL; /* Handle memory allocation failure */
    
    memcpy(dup, str, len);
    return dup;
}

void initialize_macros() {
    /* Initialize macro_list if needed */
    macro_list = NULL;
}

void add_macro(const char *name, const char *definition) {
    struct macro *new_macro = (struct macro *)malloc(sizeof(struct macro));
    if (new_macro == NULL) {
        fprintf(stderr, "Memory allocation error in add_macro.\n");
        exit(1);
    }

    new_macro->name = my_strdup(name); /* Use custom strdup function */
    new_macro->definition = my_strdup(definition);
    new_macro->next = macro_list; /* Use the global macro_list */
    macro_list = new_macro;
}

void process_macros_from_string(const char *assembly_code) {
    /* Implementation */
}

void free_macros() {
    struct macro *current = macro_list;
    while (current != NULL) {
        struct macro *next = current->next;
        free(current->name);
        free(current->definition);
        free(current);
        current = next;
    }
    macro_list = NULL;
}
