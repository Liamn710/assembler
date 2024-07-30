#include "front.h"
#include <ctype.h> 
#include <string.h>
#include <stdio.h>
#define SPACES  " \t\v\f"

/* the opcode parameters */

/* the table of all the assmebly opcode and their opcode */
struct opcode opcode_table[16] =
{
    {"mov", 0}, {"cmp", 1}, {"add", 2}, {"sub", 3}, {"lea", 4},{"clr",5},{"not", 6},{"inc", 7},
    {"dec", 8}, {"jmp", 9}, {"bne", 10},{"red", 11},{"prn",12}, {"jsr", 13}, {"rts", 14}, {"stop", 15}
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
    while(isspace(*str)) str++; /* skips the first spaces if there are any*/
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
int main() {
    return 0;
}


