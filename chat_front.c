#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int instruction_index;
int binary_instruction;
char line[100];
int i;


typedef struct {
    char name[10];
    int opcode;
    int operand_count;
    int operands[3];  
} Instruction;

typedef struct {
    char label[10];
    char name[10];
    char operands[3][10];
} ParsedLine;

Instruction instruction_set[] = {
    {"mov", 0, 2,{0}}, {"cmp", 1, 2,{0}}, {"add", 2, 2,{0}}, {"sub", 3, 2,{0}}, {"lea", 4, 2,{0}},{"clr",5, 1,{0}},{"not", 6, 1,{0}},{"inc", 7, 1,{0}},
    {"dec", 8, 1,{0}}, {"jmp", 9, 1,{0}}, {"bne", 10, 2,{0}},{"red", 11, 2,{0}},{"prn",12, 2,{0}}, {"jsr", 13, 2,{0}}, {"rts", 14, 2,{0}}, {"stop", 15, 2,{0}}
};
Instruction instruction;
const int INSTRUCTION_COUNT = sizeof(instruction_set) / sizeof(Instruction);

void parse_line(char *line, ParsedLine *parsed_line) {
    sscanf(line, "%s %s %s %s", parsed_line->label, parsed_line->name, parsed_line->operands[0], parsed_line->operands[1]);
}

int find_instruction(char *name) {
    int i;
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        if (strcmp(instruction_set[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void read_assembly_file(const char *filename) {
    ParsedLine parsed_line;
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    
    
    while (fgets(line, sizeof(line), file)) {
        parse_line(line, &parsed_line);
        instruction_index = find_instruction(parsed_line.name);
        if (instruction_index == -1) {
            printf("Unknown instruction: %s\n", parsed_line.name);
            continue;
        }
        
        instruction = instruction_set[instruction_index];
        printf("Instruction: %s, Opcode: %x\n", instruction.name, instruction.opcode);

    }
    
    fclose(file);
}

void translate_to_binary(ParsedLine *parsed_line) {
    int operand_value;
    int instruction_index = find_instruction(parsed_line->name);
    if (instruction_index == -1) {
        printf("Unknown instruction: %s\n", parsed_line->name);
        return;
    }

    instruction  = instruction_set[instruction_index];

    binary_instruction = instruction.opcode << 24; 
    for (i = 0; i < instruction.operand_count; i++) {
        operand_value = atoi(parsed_line->operands[i]);
        binary_instruction |= (operand_value << (16 - i * 8)); 
    }

    printf("Binary instruction: %08x\n", binary_instruction);
}

int main() {
    const char *filename = "ps.as";
    read_assembly_file(filename);
    return 0;
}
