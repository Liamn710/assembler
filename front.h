#ifndef FRONT_H
#define FRONT_H

#define SPACES  " \t\v\f"
#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81

/* Struct for opcodes */
struct opcode {
    char *name;
    int code;
    int num_operands;
};

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
