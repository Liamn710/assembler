#ifndef MACRO_HANDLE_H
#define MACRO_HANDLE_H

/* Define the maximum number of macros and line buffer size */
#define MAX_MACROS 100
#define LINE_BUFFER_SIZE 80

/* Structure to hold macro information */
/* 
 * name: The name of the macro.
 * text: The body/content of the macro.
 * next: Pointer to the next macro in the linked list.
 */
struct macro {
    char name[32];
    char text[1024];
    struct macro *next;  /* To support linked list */
};

/* Enum to represent the type of line in the assembly code */
enum line_type { 
    macro_call,             /* A line that calls a macro */
    macro_definition,       /* The start of a macro definition */
    any_other_line,         /* Any line that is not related to macros */
    end_macro_definition    /* The end of a macro definition */
};

/* Function declarations */

/* Initialize the macro list and reset the macro count */
void initialize_macros();

/* Process macros from a string containing assembly code */
void process_macros_from_string(char *assembly_code);

/* Add a macro with the given name and text to the macro list */
void add_macro(const char *name, const char *text);

/* Determine the type of a line in the assembly code */
enum line_type determine_line_type(const char *line);

/* Function to access the macro list */
struct macro* get_macro_list();

/* Free the allocated memory for all macros in the list */
void free_macros();

/* Trim spaces and remove commas from a string */
char *trim_and_remove_commas(char *str);

#endif /* MACRO_HANDLE_H */
