//
// Created by Eliomar Alejandro Rodriguez Ferrer on 11/06/25.
//

#ifndef ASSEMBLYDATA_H
#define ASSEMBLYDATA_H

#include "args_handler.h"

typedef struct {

    char **asmLines;
    int    lineCount;

} AssemblyData;

AssemblyData* newAssemblyData(options_t options);

#endif //ASSEMBLYDATA_H
