#define MAX_DATA_SIZE 80 /* max line length in source file, not including \n */

/* Statement definition */
struct ast {
    char lineError[200];
    const char *labelName;
    enum {
        inst,                         /*instructions - opcode*/
        dir,                          /*direction - data, string, entry or extern*/
        error,                        /*fisrt char is ;*/
        emptyLine                     /*spaces and tabs*/
    } lineType;

    struct{
        enum{
            external,                 /*!!!!!!!! need to change to extern !!!!!!!!*/
            entry,
            data,
            string                    /*!!!!!!!! need to see how to implement in code !!!!!!!!*/
        } type;                       /*line type - dir type*/

        struct {
            const char *label;
            int data[MAX_DATA_SIZE];
            int dataCount;
        } operand;                   /*line type - dir - operand & attributes*/
    } dirU;

    enum {
        mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, red, prn, jsr, rts, stop
    } opcode;
    
    struct {
        enum {
            none,
            num,
            opLabel = 3,
            reg = 5
        } type;

        /*define different types of operands based on what the instruction requires*/
        union {
            const char *label;  /*pointer to a constant character*/
            int num;            /*numeric value*/
            int reg;            /*register index*/
        } operand;
    } operands[2];              /*holds max amount of operands*/
};

struct ast front_gets_ast_from_line (char *line);