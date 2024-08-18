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
int label_count = 0;
int label_value_counter = LABEL_START_VALUE;
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
    if (str[strlen(str) - 1] == ':') {
        str[strlen(str) - 1] = '\0';  /* Remove the colon */
        return 1;
    }
    return 0;
}
void add_label(const char *label) {
    if (label_count >= MAX_LABELS) {
        fprintf(stderr, "Label table overflow.\n");
        exit(1);
    }
    strncpy(label_table[label_count].name, label, sizeof(label_table[label_count].name) - 1);
    label_table[label_count].value = label_value_counter++;
    label_count++;
}
int find_label_address(const char *label) {
    int i;
    for (i = 0; i < label_count; i++) {
        if (strcmp(label_table[i].name, label) == 0) {
            return label_table[i].value;
        }
    }
    return -1;  /* Return -1 if label not found */
}
int is_label(char *str){
    if (check_legal_label(str) > 0){
        int value;
        add_label(str);
        value = find_label_address(str);
        if(value >= 0){
            return value;
        }
    }
    return -1;
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

void process_line(char *line, int *binary_words, int *binary_word_count) {
    char *token;
    int instr_index, opcode_index, reg_index;
    int word1 = 0, word2 = 0, word3 = 0;

    /* Reset binary word count */
    *binary_word_count = 0;

    /* Tokenize the line to get the first word */
    token = strtok(line, " \t");
    if (token == NULL) return;

    /* Check if the first token is a label */
    if (is_label(token) >= 0) {
        token = strtok(NULL, " \t"); /* Get the next token */
    }

    /* If there's no token left, return */
    if (token == NULL) return;

    /* Check if the token is an instruction or opcode */
    instr_index = is_instruction(token);
    opcode_index = is_opcode(token);
    if (instr_index >= 0) {
        /* Handle instruction */
        word1 |= (opcode_table[instr_index].code << 11);
    } else if (opcode_index >= 0) {
        /* Handle opcode */
        word1 |= (opcode_table[opcode_index].code << 11);
    } else {
        /* Handle macro or unknown */
        if (is_macro(token)) {
            printf("Macro found: %s\n", token);
        } else {
            add_variable(token);
        }
        return;
    }

    /* Tokenize the next operand (if any) */
    token = strtok(NULL, " ,\t");
    if (token != NULL) {
        reg_index = is_reg(token);
        if (reg_index >= 0) {
            /* Handle register as source operand */
            word1 |= (1 << (7 + reg_index)); /* Example for setting source operand bits */
        } else {
            /* Handle variable or label as source operand */
            add_variable(token);
        }
    }

    /* Tokenize the next operand (if any) */
    token = strtok(NULL, " ,\t");
    if (token != NULL) {
        reg_index = is_reg(token);
        if (reg_index >= 0) {
            /* Handle register as destination operand */
            word1 |= (1 << (3 + reg_index)); /* Example for setting destination operand bits */
        } else {
            /* Handle variable or label as destination operand */
            add_variable(token);
        }
    }

    /* Set the A, R, E bits for the first word */
    word1 |= (1 << 01); /* Set 'A' bit */
    /* R and E bits remain 0 */

    /* Store the binary word */
    binary_words[(*binary_word_count)++] = word1;

    /* Additional words (e.g., for variables) would be added similarly */
}

void print_binary(int num) {
    int i;
    for (i = 14; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }
}