#include <ctype.h> 
#include <string.h>
#include <stdio.h>
#define SPACES  " \t\v\f"

/* the instruction parameters */
struct instruction {
    char * name;
    int opcode;
};
/* the table of all the assmebly instructions and their opcode */
struct instruction instructions_table[16] =
{
    {"mov", 0}, {"cmp", 1}, {"add", 2}, {"sub", 3}, {"lea", 4},{"clr",5},{"not", 6},{"inc", 7},
    {"dec", 8}, {"jmp", 9}, {"bne", 10},{"red", 11},{"prn",12}, {"jsr", 13}, {"rts", 14}, {"stop", 15}
};
/* we want to separate the instructions and the operands from the spaces,
   the result will be saved in a struct like this, each inst/op will be saved as a different string */
struct string_sep_result {
    char *strings[80]; 
    int strings_count; /* counts the amount of inst/opcode */
};
/* the separation of strings from the spaces will be done using the isspace and the strpbrk functions*/
struct string_sep_result string_sep(char * str) {
    int strings_count = 0; /* counts the amount of inst/opcode */
    char *s;
    struct string_sep_result ssr = {0};
    while(isspace(*str)) str++; /* skips the first spaces if there are any*/
    if(*str == '\0') { /* if the string is empty return nothing*/
        return ssr;
    }
    /* in this stage we know there is at least 1 string (inst/op) and we begin to separate it from the spaces*/
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
int main() {
    struct string_sep_result ssr;
    char line[81];
    FILE * f;
    int line_count =1;
    int i;
    f = fopen("ps.as","r");
    while(fgets(line,sizeof(line),f)) {
        line[strcspn(line, "\r\n")] = 0;
        ssr = string_sep(line);

        printf("line%d:\n",line_count);
        for(i=0;i<ssr.strings_count;i++) {
            printf("\tstr[%d]='%s'\n",i,ssr.strings[i]);
        }
        line_count++;
    }
    fclose(f);
    return 0;
}


