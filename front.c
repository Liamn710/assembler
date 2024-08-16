#include "front.h"
#include "macro_handle.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Opcode table initialization */
struct opcode opcode_table[16] = {
    {"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2},
    {"lea", 4, 2}, {"clr", 5, 2}, {"not", 6, 1}, {"inc", 7, 1},
    {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 2}, {"red", 11, 1},
    {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 1}, {"stop", 15, 0}
};

/* Instruction table */
char *instructions_table[] = {".data", ".string", ".extern", ".entry"};

/* Registers */
char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Function to separate strings by spaces */
struct string_sep_result string_sep(char *str) {
    int strings_count = 0;
    char *s;
    struct string_sep_result ssr = {0};

    while (isspace(*str)) str++;

    if (*str == '\0') {
        return ssr;
    }

    do {
        ssr.strings[strings_count++] = str;
        s = strpbrk(str, SPACES);
        if (s) {
            *s = '\0';
            s++;
            while (isspace(*s)) s++;
            if (*s == '\0')
                break;
            str = s;
        } else {
            break;
        }
    } while (1);

    ssr.strings_count = strings_count;
    return ssr;
}

/* Function to check if a string is an instruction */
int is_instruction(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (strcmp(str, instructions_table[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/* Function to check if a string is an opcode */
int is_opcode(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        if (strcmp(str, opcode_table[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Function to check if a string is a register */
int is_reg(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    for (i = 0; i < 8; i++) {
        if (strcmp(str, registers[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/* Function to check for extra text after expected tokens */
int extra_text() {
    char *token;
    token = strtok(NULL, "\n");
    if (token != NULL) {
        return 1;
    }
    return 0;
}

/* Function to check if a label is legal */
int check_legal_label(char *str) {
    if (str == NULL) {
        return 0;
    }
    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (is_opcode(str) < 0) && !is_instruction(str) && (is_reg(str) < 0)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ;
        }
    }
    if (*str == '\0' || *str == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int is_macro(const char *str) {
    struct macro *m = get_macro_list();  /* Access the macro list using the accessor function */
    while (m != NULL) {
        if (strcmp(m->name, str) == 0) {
            return 1;
        }
        m = m->next;
    }
    return 0;
}

int find_variable(const char *name) {
    int i;
    for ( i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].address;
        }
    }
    return -1; /* Not found*/
}

int add_variable(const char *name) {
    int address;
    if (variable_count >= MAX_VARIABLES) {
        fprintf(stderr, "Error: Too many variables.\n");
        exit(1);
    }
    address = next_memory_address++;
    strcpy(variables[variable_count].name, name);
    variables[variable_count].address = address;
    variable_count++;
    return address;
}
