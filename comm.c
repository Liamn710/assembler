#include "comm.h"
#include <string.h>

struct symbol *symbolLookUp (struct symbol *symbolTable, const int symbolTableSize, const char *name) {
    int i;
    for (i = 0; i < symbolTableSize; i++) {
        if (strcmp(symbolTable[i].symName, name) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;
}

struct ext *extSearch (struct ext *externals, const int externalsSize, const char *name) {
    int i;
    for (i = 0; i < externalsSize; i++) {
        if (strcmp(name, externals[i].externalName) == 0) {
            return & externals[i];
        }
    }
    return NULL;
}