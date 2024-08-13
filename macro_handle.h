#ifndef MACRO_HANDLE_H
#define MACRO_HANDLE_H

/* Include necessary headers */
#include <stddef.h> /* For size_t */

/* Define the macro structure */
struct macro {
    char *name;
    char *definition;
    struct macro *next;
};

/* Function declarations */
void initialize_macros();
void add_macro(const char *name, const char *definition);
void process_macros_from_string(const char *assembly_code);
void free_macros();

#endif /* MACRO_HANDLE_H */
