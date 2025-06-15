//
// Created by Eliomar Alejandro Rodriguez Ferrer on 11/06/25.
//

#include <stdlib.h>
#include <ncurses.h>

#include "include/args_handler.h"
#include "include/assembly_data.h"

#include <string.h>

AssemblyData* newAssemblyData(const options_t options) {

    int currentLine = 0;
    AssemblyData* data = malloc(sizeof(AssemblyData));
    if (!data) return nullptr;

    data->lineCount = 0;

    // Read the file header
    FILE *file = fopen(options.binary_file, "r");
    if (!file) {
        return nullptr;

    }

    // Count file line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        data->lineCount++;

    }

    rewind(file);

    // Alloc memory for the rows
    data->asmLines = malloc(data->lineCount * sizeof(char*));
    if (!data->asmLines) {
        fclose(file);

    }


    while (fgets(buffer, sizeof(buffer), file) && currentLine < data->lineCount) {
        const size_t len = strlen(buffer);

        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';

        }

        data->asmLines[currentLine] = malloc(strlen(buffer) + 1);

        strcpy(data->asmLines[currentLine], buffer);

        currentLine++;
    }

    fclose(file);

    data->lineToInstructionMap = malloc(sizeof(int) * data->lineCount);

    if (!data->lineToInstructionMap) return nullptr;

    data->instructionToLineMap = malloc(sizeof(int) * options.instruction_count);

    int totalInstructions = 0;

    for (int i = 0; i < data->lineCount; i++) {
        data->lineToInstructionMap[i] = -1; // -1
    }

    for (int i = 0; i < options.instruction_count; i++) {
        data->instructionToLineMap[i] = -1;
    }

    for (int i = 0; i < data->lineCount; i++) {
        const char *trimmed = data->asmLines[i];

        // Skip spaces
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Skip row empty, comment e directive
        if (*trimmed == '\0' || *trimmed == '#' || *trimmed == '.' || *trimmed == '_') {
            continue;
        }

        // Skip tag
        const char *colon = strchr(trimmed, ':');
        if (colon && *(colon + 1) == '\0') {
            continue;
        }

        data->lineToInstructionMap[i] = totalInstructions;

        if (totalInstructions < options.instruction_count) {
            data->instructionToLineMap[totalInstructions] = i;
        }

        totalInstructions++;
    }

    return data;

}
