/*middle end second pass - handling logic errors (undefined symbols) & building translation unit from source code*/
#include "front_Loren.h"
#include "comm.c"
#include <stdio.h>

int secondPass(struct translationUnit *prog, const char *fileName, FILE *file) {
    char line[81] = {0};
    int errorFlag = 0;
    int lineCounter = 1;
    struct ast lineStruct = {0};
    struct symbol * symFind;
    struct ext *extFind;
    int i;

    while (fgets(line, sizeof(line), file)) {
        lineStruct = front_gets_ast_from_line(line);

        if (lineStruct.lineType == inst) {
            prog -> codeImage[prog -> IC] = lineStruct.operands[0].type << 9;
            prog -> codeImage[prog -> IC] |= lineStruct.operands[1].type << 2;
            prog -> codeImage[prog -> IC] |= lineStruct.opcode << 5;
            prog -> IC++;
            if (lineStruct.operands[0].type == reg && lineStruct.operands[1].type == reg) {
                prog -> codeImage[prog -> IC] = lineStruct.operands[0].operand.reg << 7;
                prog -> codeImage[prog -> IC] |= lineStruct.operands[1].operand.reg << 2;
                prog -> IC++;
            }
            else {
                for (i = 0; i < 2; i++) {
                    if (lineStruct.operands[i].type == reg) {
                        prog -> codeImage[prog -> IC] = lineStruct.operands[i].operand.reg << (7 - (i * 5));
                    }
                    else if (lineStruct.operands[i].type == opLabel) {
                        symFind = symbolLookUp(prog -> symbolTable, prog -> symCount, lineStruct.operands[i].operand.label);
                        if (symFind) {
                            prog -> codeImage[prog -> IC] = symFind -> symAddress << 2;
                            if (symFind -> symType == symExtern) {
                                prog -> codeImage[prog -> IC] |= 1;
                                extFind = extSearch(prog -> externals, prog -> extCount, symFind -> symName);
                                if (extFind) {
                                    extFind -> addresses[extFind -> addressCount] = prog -> IC + 100;
                                    extFind -> addressCount++;
                                }
                                else {
                                    prog -> externals[prog -> extCount].externalName = symFind -> symName;
                                    prog -> externals[prog -> extCount].addresses[prog -> externals[prog -> extCount].addressCount] = prog -> IC + 100;
                                    prog -> externals[prog -> extCount].addressCount++;
                                    prog -> extCount++;
                                }
                            }
                            else {
                            prog -> codeImage[prog -> IC] |= 2;
                            }
                        }
                        else {
                        printf("error in line %d undefined label: '%s'\n", lineCounter, lineStruct.operands[i].operand.label);
                        errorFlag = 1;
                        }
                    }
                    else if(lineStruct.operands[i].type == num) {
                        prog -> codeImage[prog -> IC] = lineStruct.operands[i].operand.num << 2;
                    }
                    if (lineStruct.operands[i].type != none) {
                        prog -> IC++;
                    }
                }
            }
        }
        lineCounter++;
    }
    return errorFlag; 
}