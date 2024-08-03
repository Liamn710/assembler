#include "pre_processor.h"
#include "front.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Global pointer to the head of the linked list of macros*/
Macr *macr_list = NULL;;

/*List of instructions and opcodes to check for conflicts (example instructions)*/
const int INSTRUCTION_COUNT = sizeof(instructions_table) / sizeof(instructions_table[0]);
const int OPCODE_COUNT = sizeof(opcode_table) / sizeof(opcode_table[0]);

void initialize_macrs() {
    macr_list = NULL;
}

int is_macr_name_valid(const char *name) {
    int i;
    /* Check against instruction names*/
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(name, instructions_table[i]) == 0) {
            return 0; /* Invalid name*/
        }
    }
    /* Check against opcode names*/
    for (i = 0; i < OPCODE_COUNT; i++) {
        if (strcmp(name, opcode_table[i]) == 0) {
            return 0; /* Invalid name*/
        }
    }
    return 1; /* Valid name*/
}

int add_macr(const char *name, const char *body) {
    if (!is_macr_name_valid(name)) {
        fprintf(stderr, "Error: Macro name conflicts with instruction name.\n");
        return 0;
    }

    Macr *new_macr = (Macr *)malloc(sizeof(Macr));
    new_macr->name = strdup(name);
    new_macr->body = strdup(body);
    new_macr->next = macr_list;
    macr_list = new_macr;

    return 1;
}

void process_macrs_from_string(const char *assembly_code) {
    char line[80];
    char macr_name[100];
    char *macr_body = NULL;
    size_t macr_body_size = 0;
    int in_macr = 0;

    /* Copy the assembly code to a temporary buffer to use strtok*/
    char *assembly_code_copy = strdup(assembly_code);
    char *line_ptr = strtok(assembly_code_copy, "\n");

    while (line_ptr) {
        strcpy(line, line_ptr);

        if (strncmp(line, "macr ", 6) == 0) {
            sscanf(line + 6, "%s", macr_name);
            if (!is_macro_name_valid(macr_name)) {
                fprintf(stderr, "Error: Macro name conflicts with instruction name.\n");
                free(assembly_code_copy);
                return;
            }
            in_macr = 1;
            free(macr_body); // Free the previous macro body if it exists
            macr_body = NULL;
            macr_body_size = 0;
        } else if (in_macr) {
            if (strncmp(line, "endmacr", 7) == 0) {
                in_macr = 0;
                add_macr(macr_name, macr_body);
            } else {
                size_t line_len = strlen(line) + 1; // +1 for newline character
                macr_body = (char *)realloc(macr_body, macr_body_size + line_len + 1);
                strcpy(macr_body + macr_body_size, line);
                macr_body_size += line_len;
                macr_body[macr_body_size] = '\n'; // Maintain line breaks
                macr_body_size += 1;
                macr_body[macr_body_size] = '\0';
            }
        }

        line_ptr = strtok(NULL, "\n");
    }

    free(assembly_code_copy);
}

void free_macrs() {
    macr *current = macr_list;
    while (current) {
        macr *next = current->next;
        free(current->name);
        free(current->body);
        free(current);
        current = next;
    }
}

