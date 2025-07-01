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
    bool in_text_section = false;  // Flag per tracciare se siamo nella sezione .text

    AssemblyData* data = malloc(sizeof(AssemblyData));
    if (!data) return nullptr;

    data->lineCount = 0;
    data->asmLines = nullptr;
    data->lineToInstructionMap = nullptr;
    data->instructionToLineMap = nullptr;

    // Read the file header
    FILE *file = fopen(options.binary_file, "r");
    if (!file) {
        free(data);
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
        free(data);
        return nullptr;
    }

    for (int i = 0; i < data->lineCount; i++) {
        data->asmLines[i] = nullptr;
    }

    while (fgets(buffer, sizeof(buffer), file) && currentLine < data->lineCount) {
        const size_t len = strlen(buffer);

        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }

        data->asmLines[currentLine] = malloc(strlen(buffer) + 1);
        if (!data->asmLines[currentLine]) {
            fclose(file);
            for (int i = 0; i < currentLine; i++) free(data->asmLines[i]);
            free(data->asmLines);
            free(data);
            return nullptr;
        }

        strcpy(data->asmLines[currentLine], buffer);
        currentLine++;
    }

    fclose(file);

    data->lineToInstructionMap = malloc(sizeof(int) * data->lineCount);
    if (!data->lineToInstructionMap) {
        for (int i = 0; i < data->lineCount; i++) free(data->asmLines[i]);
        free(data->asmLines);
        free(data);
        return nullptr;
    }

    // Primo passaggio: conta solo le istruzioni nella sezione .text
    int totalInstructions = 0;
    in_text_section = false;

    for (int i = 0; i < data->lineCount; i++) {
        const char *trimmed = data->asmLines[i];

        // Skip spaces
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Verifica se siamo entrati nella sezione .text
        if (strncmp(trimmed, ".text", 5) == 0) {
            in_text_section = true;
            continue;
        }

        // Verifica se siamo usciti dalla sezione .text (entrati in .data o altra sezione)
        if (trimmed[0] == '.' && strncmp(trimmed, ".text", 5) != 0) {
            in_text_section = false;
            continue;
        }

        // Conta solo le istruzioni nella sezione .text
        if (in_text_section) {
            // Skip empty lines, comments
            if (*trimmed == '\0' || *trimmed == '#') {
                continue;
            }

            // Skip labels (lines ending with colon only)
            const char *colon = strchr(trimmed, ':');
            if (colon && *(colon + 1) == '\0') {
                continue;
            }

            totalInstructions++;
        }
    }

    // Allocate instruction-to-line map with correct size
    data->instructionToLineMap = malloc(sizeof(int) * totalInstructions);
    if (!data->instructionToLineMap) {
        free(data->lineToInstructionMap);
        for (int i = 0; i < data->lineCount; i++) {
            free(data->asmLines[i]);
        }
        free(data->asmLines);
        free(data);
        return nullptr;
    }

    // Initialize mapping arrays
    for (int i = 0; i < data->lineCount; i++) {
        data->lineToInstructionMap[i] = -1;
    }

    for (int i = 0; i < totalInstructions; i++) {
        data->instructionToLineMap[i] = -1;
    }

    // Fill mapping arrays - Solo con istruzioni nella sezione .text
    int instructionIndex = 0;
    in_text_section = false;

    for (int i = 0; i < data->lineCount; i++) {
        const char *trimmed = data->asmLines[i];

        // Skip spaces
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Verifica se siamo entrati nella sezione .text
        if (strncmp(trimmed, ".text", 5) == 0) {
            in_text_section = true;
            continue;
        }

        // Verifica se siamo usciti dalla sezione .text
        if (trimmed[0] == '.' && strncmp(trimmed, ".text", 5) != 0) {
            in_text_section = false;
            continue;
        }

        // Mappa solo le istruzioni nella sezione .text
        if (in_text_section) {
            // Skip empty lines, comments
            if (*trimmed == '\0' || *trimmed == '#') {
                continue;
            }

            // Skip labels
            const char *colon = strchr(trimmed, ':');
            if (colon && *(colon + 1) == '\0') {
                continue;
            }

            data->lineToInstructionMap[i] = instructionIndex;
            data->instructionToLineMap[instructionIndex] = i;
            instructionIndex++;
        }
    }

    return data;
}
