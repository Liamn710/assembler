#ifndef FRONT_H
#define FRONT_H

#define SPACES  " \t\v\f"
#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81
#define MAX_VARIABLES 100
#define MAX_LABELS 100
#define LABEL_START_VALUE 100
#define MAX_BINARY_WORDS 3
/* Struct for opcodes */
struct opcode {
    char *name;
    int code;
    int num_operands;
};
struct variable {
    char name[32];
    int address;
};
struct label_entry {
    char name[32];
    int value;  /* Use 'value' instead of 'address' */
};
struct label_entry label_table[MAX_LABELS];
static struct variable variables[MAX_VARIABLES];
static int variable_count = 0;
static int next_memory_address = 0x1000; /* Starting address for variables */

/* External declarations for opcode table, instruction table, and registers */
extern struct opcode opcode_table[16];
extern char *instructions_table[];
extern char *registers[];

/* Struct for storing separated strings */
struct string_sep_result {
    char *strings[80];
    int strings_count;
};

/* Functions for processing assembly code */
struct string_sep_result string_sep(char *str);
int is_instruction(char *str);
int is_opcode(char *str);
int is_reg(char *str);
int extra_text();
int check_legal_label(char *str);

#endif /* FRONT_H */
