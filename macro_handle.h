#ifndef MACRO_HANDLE_H
#define MACRO_HANDLE_H

#define MAX_MACROS 100
#define LINE_BUFFER_SIZE 80

/* Structure to hold macro information */
struct macro {
    char name[32];
    char text[1024];
    struct macro *next;  /* To support linked list */
};

/* Enum to represent the line type */
enum line_type { 
    macro_call, 
    macro_definition, 
    any_other_line, 
    end_macro_definition 
};

/* Function declarations */
void initialize_macros();
void process_macros_from_string(char *assembly_code);
void add_macro(const char *name, const char *text);
enum line_type determine_line_type(const char *line);
struct macro* get_macro_list();  /* Function to access the macro list */
void free_macros(); /* Function to free the allocated memory for macros */
char *trim_and_remove_commas(char *str);
#endif
