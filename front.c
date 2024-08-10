#include "front.h"
#include "errors.c"
#include "globals.h"
#include <ctype.h> 
#include <string.h>
#include <stdio.h>
#define SPACES  " \t\v\f"
#define MAX_LABEL_LENGTH 31
#define MAX_LINE_LENGTH 81

/* the table of all the assmebly opcode and their opcode */
struct opcode opcode_table[16] =
{
    {"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2}, {"lea", 4, 2},{"clr",5, 2},{"not", 6, 1},{"inc", 7, 1},
    {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 2},{"red", 11, 1},{"prn",12, 1}, {"jsr", 13, 1}, {"rts", 14, 1}, {"stop", 15, 0}
};
/*all the instructions allowed:*/
char *instructions_table[] = {".data", ".string", ".extern", ".entry"};

/*the registers allowed:*/
char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* we want to separate the opcodes,operands and instructions from the spaces,
   the result will be saved in a struct like this, each instruction/operand/opcode will be saved as a different string */
struct string_sep_result {
    char *strings[80]; 
    int strings_count; /* counts the amount of inst/opcode/operand */
};
/* the separation of strings from the spaces will be done using the isspace and the strpbrk functions*/
struct string_sep_result string_sep(char * str) {
    int strings_count = 0; /* counts the amount of inst/opcode/operand */
    char *s;
    struct string_sep_result ssr = {0};
    while(isspace(*str)) str++;
     /* skips the first spaces if there are any*/
    if(*str == '\0') { /* if the string is empty return nothing*/
        return ssr;
    }
    /* in this stage we know there is at least 1 string (inst/opcode/operand) and we begin to separate it from the spaces*/
    do {
        ssr.strings[strings_count++] = str; /*set the relevant pointer */
        s = strpbrk(str,SPACES); /* looking for space*/
        if(s) {
            *s = '\0'; 
            s++;/*when a space is found we stop it and move a char forword*/
            while(isspace(*s))s++; /*keep going pass the spaces until the string ends or until the next string*/
            if(*s == '\0')
                break;
            str = s;    
        }else {
            break;
        }
    }while(1);
    ssr.strings_count = strings_count;
    return ssr;
}
int is_instruction(char *str){
    int i;
    if (str == NULL){ /*if str is null return -1*/
        return -1;
    }
    else{
        for(i = 0;i < 3;i++){
            if (strcmp(str,instructions_table[i]) == 0)/*compare the str to the instructions, if there is a match return the matching index*/
            {
                return i;/*return the number of instruction*/
            }
            
        }
        return -1;
    }
}

int is_opcode(char *str) {
    int i;
    if (str == NULL) {
        return -1;
    }
    /* compare the str to the name of the opcodes*/
    for (i = 0; i < 16; i++) {
        if (strcmp(str, opcode_table[i].name) == 0) {
            return i; /* return the index of the matching opcode */
        }
    }
    return -1; /* return -1 if the string does not match any opcode */
}
int is_reg(char *str) {
    int i;

    if (str == NULL) {
        return -1;
    }

    /* check if the string is one of the registers by comparing the string with each register in the list */
    for (i = 0; i < 8; i++) {
        if (strcmp(str, registers[i]) == 0) {
            return i; /* Return the index of the matching register */
        }
    }
    return -1; /* Return -1 if the string does not match any known registers */
}

int extra_text() {
    char *token;
    token = strtok(NULL, "\n");

    /* If a token exists beyond the expected content, there is extra text */
    if (token != NULL) {
        return 1;
    }
    return 0; /* No extra text found */
}
int check_legal_label(char *str) {
    if (str == NULL) {
        return 0;
    }

    /* If str is in one of the following - the label name is not valid */
    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (is_opcode(str) < 0) && \
        !is_instruction(str)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ;
        }
    }
    /* If we reached this point and the string either terminates or we have a space
     * then it means that all the characters were legal and the label is legal */
    if (*str == '\0' || *str == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int is_reg_or_label(char *str) {
    /* Check if the string is a valid register using is_reg()
     * OR if it's a legal label using legal_label() */
    if ((is_reg(str) >= 0) || check_legal_label(str)) {
        return 1;
    }
    return 0; /* Neither a valid register nor a legal label */
    }
int is_num(char *str) {
    char *ptr;

    /* Check if the string is not NULL */
    if (str != NULL) {
        strtol(str, &ptr, 10);

        /* Check if the conversion result is followed by a space or the entire string is converted */
        if (*ptr == '\0' || *ptr == ' ') {
            return 1; /* The string represents a numeric value */
        }
    }
    return 0; /* The string does not represent a numeric value */
}
int is_reg_or_label_or_num(char *str) {
    /* Check if the string is a valid register or label, and if it's a valid number */
    return (is_reg_or_label(str) || is_num(str));
}
int check_first_arg(char *str, char *ptr) {
    /* Calculate the length of the first argument substring */
    int first_arg_len = (int) (ptr - str);

    /* Extract the first argument substring */
    char first_arg[MAX_LINE_LENGTH];
    strncpy(first_arg, str, first_arg_len);
    first_arg[first_arg_len] = '\0';

    /* Check if the extracted first argument is a valid register, label, or number */
    return is_reg_or_label_or_num(first_arg);
    }
int count_occurr(char *str, char ch) {
    int count;
    char *ptr;
    ptr = str;
    count = 0;

    /* Traverse the string until the end or until no more occurrences are found */
    while ((*ptr != '\0') && (ptr = strchr(ptr, ch)) != NULL) {
        count++;
        ptr++;
    }
    return count; /* Return the total count of occurrences */
}
int has_white_space(const char *str) {
    while (*str) {
        if (isspace(*str)) {
            return 1; /* Found a whitespace character */
        }
        str++;
    }
    return 0; /* No whitespace characters found */
}
void check_reg_error(int *error_code, char *str) {
    char temp_str1[MAX_LINE_LENGTH];
    strcpy(temp_str1, str);

    /* Check if any previous error has already occurred */
    if (error_code[0])
        return;

    /* Check if any previous error has already occurred */
    if (has_white_space(str))
        *error_code = ERROR_CODE_33;
    else if (atoi(strtok(temp_str1, "@r"))) {
        *error_code = ERROR_CODE_46; /* Illegal register name */
    } else
        *error_code = ERROR_CODE_33; /* Illegal argument */
}
int legal_label(char *str) {
    if (str == NULL) {
        return 0;
    }

    /* If str is in one of the following - the label name is not valid */
    if (isalpha(*str) && strlen(str) <= MAX_LABEL_LENGTH && (is_opcode(str) < 0) && \
        !is_instruction(str)) {
        while (*(++str) != '\0' && *(str) != ' ' && (isalpha(*str) || isdigit(*str))) { ;
        }
    }
    /* If we reached this point and the string either terminates or we have a space
     * then it means that all the characters were legal and the label is legal */
    if (*str == '\0' || *str == ' ') {
        return 1;
    } else {
        return 0;
    }
}

int legal_arg(char *str, command_parts *command, int *error_code) {
    char *str1, *str2, *ptr;

    /* Check if the argument is empty */
    if (str == NULL && opcode_table[command->opcode].arg_num != 0) {
        *error_code = ERROR_CODE_34;
        return 0;
    }

    /* Handle opcodes with no arguments */
    if (opcode_table[command->opcode].arg_num == 0) {
        if (extra_text()) {
            *error_code = ERROR_CODE_32;
            return 0;
        } else {
            command->source = command->dest = NULL;
            return 1;
        }
    }

    /* Handle cases where the command has two arguments */
    if (opcode_table[command->opcode].arg_num == 2) {
        if (strstr(str, ",") == NULL) {
            *error_code = ERROR_CODE_35; /* Missing comma between arguments */
            return 0;
        } else if (count_occurr(str, ',') > 1) {
            *error_code = ERROR_CODE_39; /* Extra commas between arguments */
            return 0;
        } else {
            str1 = strtok(str, ",");
            if ((ptr = strchr(str1, ' '))) {
                if (check_first_arg(str1, ptr)) {
                    *error_code = ERROR_CODE_35; /* Invalid first argument format */
                    return 0;
                }
                *error_code = ERROR_CODE_33; /* Other argument-related error */
                return 0;
            }
            str2 = strtok(NULL, " \n");
            if (extra_text()) {
                *error_code = ERROR_CODE_32; /* Extra text after arguments */
                return 0;
            }
        }
    }
    /* Handle cases where the command has one argument */
    else if (opcode_table[command->opcode].arg_num == 1) {
        if (strchr(str, ' ')) {
            *error_code = ERROR_CODE_32; /* Extra text after argument */
            return 0;
        }
    }

    /* Validate and process the argument based on the opcode */
    switch (command->opcode) {
        /* Handle each opcode case separately */
        /* For each case, validate the source and destination arguments, if applicable. */
        /* If validation fails, set the error code and return 0. */
        /* If validation succeeds, assign the validated arguments to the command structure. */

        /* source addressing code is 1,3,5 and dest addressing code is 1,3,5 */
        case 1: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label_or_num(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                if (str2 == NULL)
                    *error_code = ERROR_CODE_34; /* Missing argument */
                else if (is_reg(str1) == -1 || is_reg(str2) == -1)
                    *error_code = ERROR_CODE_46; /* Illegal register name */
                else
                    *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* source addressing code is 1,3,5 and dest addressing code is 3,5 */
        case 0:
        case 2:
        case 3: {
            if (is_reg_or_label_or_num(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                check_reg_error(error_code, str1);
                check_reg_error(error_code, str2);
                return 0;
            }
            break;
        }

        /* source addressing code is 3 and dest addressing code is 3,5 */
        case 6: {
            if (legal_label(str1) && is_reg_or_label(str2)) {
                command->source = str1;
                command->dest = str2;
            } else {
                *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* dest addressing code is 3,5 and no source */
        case 4:
        case 5:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 13: {
            if (is_reg_or_label(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                if (atoi(strtok(str, "@r")))
                    *error_code = ERROR_CODE_46; /* Illegal register name */
                else
                    *error_code = ERROR_CODE_33; /* Illegal argument */
                return 0;
            }
            break;
        }

        /* dest addressing code is 1,3,5 and no source */
        case 12: {
            if (is_reg_or_label_or_num(str)) {
                command->source = NULL;
                command->dest = str;
            } else {
                check_reg_error(error_code, str);
                return 0;
            }
            break;
        }
        /* case 14,15 are dealt with in previous stage */
        default: {
            *error_code = ERROR_CODE_33; /* Illegal argument */
            return 0;
        }
    }
    return 1;
}

int inc_array(inst_parts **inst, int len) {
    short *ptr;
    ptr = (*inst)->nums; /* Backup for the case realloc fails */
    (*inst)->nums = realloc((*inst)->nums, (len + 1) * sizeof(short));
    if ((*inst)->nums == NULL) {
        print_internal_error(ERROR_CODE_1); /* Internal error: Memory allocation failed */
        free(ptr);
        return 0;
    }
    return 1;   
}

int is_string_legal(const char *str) {
    int digitFlag = 0, commaFlag = 0, i;
    for (i = 0; str[i]; i++) {
        if (isdigit(str[i]) || str[i] == '-' || str[i] == ' ') {
            if (commaFlag) commaFlag = 0;
            if (isdigit(str[i])) digitFlag = 1;
        } else if (str[i] == ',') {
            if (!digitFlag || commaFlag) {
                return 0; /* Return false if comma found before any digit or after another comma*/
            }
            commaFlag = 1;
        } else {
            return 0; /* Return false for any other character */
        }
    }
    if (commaFlag) {
        return 0; /* Return false if string ends with a comma */
    }
    return 1; /*Return true if string is legal */
}

int main() {
    return 0;
}