#ifndef COMM_H
#define COMM_H

#define MAX_MEM_SIZE 1024
#define MAX_SYMBOLS 1024
#define MAX_LINE_LEN 81 /* max line length in source file, including \n */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define AFTER_SPACE(s) while(*s && isspace(*s)) s++
#define WHITE_SPACES "\t\n\f\r\v"
#define strcatWithMalloc(s1, s2) strcat(strcpy(malloc(strlen(s1) + strlen(s2) + 1), s1), s2)

/*first pass*/
/*definition of symbol - name, type, memory address (optional), line location in file*/
struct symbol
{
    char symName[32];
    enum {
        symExtern,      /*external symbol - not defined in source file - no address*/
        symEntry,       /*internal symbol - must be defined as code (symEntryCode) or data (symEntryData) in first pass*/
        symEntryCode,
        symEntryData,
        symCode,
        symData
    } symType;
    int symAddress;
    int symLine;    /*!!!!!!!! implement in code for error checks !!!!!!!!*/
};

/*second pass*/
/**/
struct ext {
    char *externalName;
    int addresses[1024];
    int addressCount;
};

struct translationUnit {
    int codeImage[MAX_MEM_SIZE];
    int IC;
    int dataImage[MAX_MEM_SIZE];
    int DC;
    struct symbol symbolTable[MAX_SYMBOLS];
    int symCount;
    struct ext externals[MAX_SYMBOLS];
    int extCount;                           /*counter of external symbols for externals output file*/
    struct symbol * entries[MAX_SYMBOLS];
    int entriesCount;                       /*counter of entry symbols for entries output file*/
};

struct symbol *symbolLookUp(struct symbol *symbolTable, const int symbolTableSize, const char *name);

struct ext *extSearch(struct ext *externals, const int externalsSize, const char *name);

#endif