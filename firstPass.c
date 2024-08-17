/*middle end first pass - handling syntax errors & building symbols table*/
#include "front_Loren.h"
#include "comm.c"
#include <stdio.h>
#include <string.h>

int firstPass(struct translationUnit *prog, const char *fileName, FILE *file) {
    char line[81] = {0};            /* max line length in source file, including \n */

    /*ic & dc are holding the next availible spot in memory*/
    int ic = 100;                   /*ic = instructions memory counter initialized w address 100*/
    int dc = 0;                     /*dc = data memory counter*/

    int errorFlag = 0;
    int lineCounter = 1;
    int i;
    struct ast lineStruct = {0};
    struct symbol * symFind;

    /*extract source code lines*/
    while (fgets(line, sizeof(line), file)) {
        lineStruct = front_gets_ast_from_line(line);

        /*checking syntax error in each line in file*/
        if (lineStruct.lineError[0] != '\0') {
            printf("%s:%d: syntax error: %s", fileName, lineCounter, lineStruct.lineError);
            lineCounter++;
            errorFlag = 1;
            continue;
        }

        /*defining symbol - data or code*/
        /*check if there is a symbol, if so checking if data or code*/
        if (lineStruct.labelName != NULL && (lineStruct.lineType == inst || (lineStruct.lineType == dir && lineStruct.dirU.type == data))) {
            /*check if symbol already exists*/
            symFind = symbolLookUp(prog -> symbolTable, prog -> symCount, lineStruct.labelName);
            if (symFind) {
                /*check if symbol is entry & classify to code or data*/
                if (symFind -> symType == symEntry) {
                    symFind -> symType = lineStruct.lineType == inst ? symEntryCode : symEntryData;
                    symFind -> symAddress= lineStruct.lineType == inst ? ic : dc;
                }
                else {
                    printf("%s: error in line %d redefinition of symbol: '%s'\n", fileName, lineCounter, lineStruct.labelName);
                    errorFlag = 1;
                }
            }
            else {
                strcpy(prog->symbolTable[prog->symCount].symName, lineStruct.labelName);
                prog -> symbolTable[prog -> symCount].symType = lineStruct.lineType == inst ? symCode : symData;
                prog -> symbolTable[prog -> symCount].symAddress = lineStruct.lineType == inst ? ic : dc;
                prog -> symCount++;
            }
        }
        
        if (lineStruct.lineType == inst) {
            ic++;
            if (lineStruct.operands[0].type == reg && lineStruct.operands[1].type == reg) {
                ic++;
            }
            else {
                ic += (lineStruct.operands[0].type >= num) + (lineStruct.operands[1].type >= num);
            }
        }
        else if (lineStruct.lineType == dir && lineStruct.dirU.type >= data){
            memcpy(&prog -> dataImage[prog -> DC], lineStruct.dirU.operand.data, lineStruct.dirU.operand.dataCount *sizeof(int));
            dc += lineStruct.dirU.operand.dataCount;
            prog -> DC = dc;
        }
        else if (lineStruct.lineType == dir && lineStruct.dirU.type <= entry){
            symFind = symbolLookUp(prog -> symbolTable, prog -> symCount, lineStruct.dirU.operand.label);
            if(symFind && lineStruct.dirU.type == entry) {
                if(symFind -> symType == symCode) {
                    symFind -> symType = symEntryCode;
                }
                else if (symFind -> symType == symData) {
                    symFind -> symType = symEntryData;
                }
                else {
                    printf("%s: error in line %d redefinition of symbol: '%s'\n", fileName, lineCounter, lineStruct.dirU.operand.label);
                    errorFlag = 1;
                }
            }
            else if (!symFind) {
                strcpy(prog -> symbolTable[prog -> symCount].symName, lineStruct.dirU.operand.label);
                prog -> symbolTable[prog -> symCount].symType = lineStruct.dirU.type;
                prog -> symCount++;
            }
            else {
                printf("%s: error in line %d redifinition of symbol: '%s'\n", fileName, lineCounter, lineStruct.dirU.operand.label);
                errorFlag = 1;
            }
        }
        lineCounter++;
    }

    for (i = 0; i < prog -> symCount; i++) {
        if (prog -> symbolTable[i].symType == symEntry) {
            printf("%s: error symbol: '%s' declared entry but was never defined.\n", fileName, prog -> symbolTable[i].symName);
            errorFlag = 1;
        }
        if (prog -> symbolTable[i].symType == symData || prog -> symbolTable[i].symType == symEntryData){
            prog -> symbolTable[i].symAddress += ic;
        }
        if (prog -> symbolTable[i].symType >= symEntryCode) {
            prog -> entries[prog -> entriesCount] = &prog -> symbolTable[i];
            prog -> entriesCount++;
        }
    }
    return errorFlag;
}