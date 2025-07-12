//
// Created by Eliomar Alejandro Rodriguez Ferrer on 11/06/25.
//

#include <stdlib.h>
#include <ncurses.h>

#include "include/args_handler.h"
#include "include/assembly_data.h"

#include <string.h>

AssemblyData* newAssemblyData(const options_t options) {
    bool in_text_section = false;

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

    // Count file lines and pseudo-instructions to expand
    char buffer[1024];
    int extraLinesNeeded = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        data->lineCount++;

        // Trim leading whitespace
        char* trimmed = buffer;
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Check if this is a 'la' instruction that needs expansion
        if (strncmp(trimmed, "la ", 3)  == 0 ||
            strncmp(trimmed, "la\t", 3) == 0
        ) {
            extraLinesNeeded++; // Each 'la' becomes 2 instructions (so +1 extra line)
        }
    }

    rewind(file);

    // Allocate memory for all lines, including expanded pseudo-instructions
    int totalLines = data->lineCount + extraLinesNeeded;
    data->asmLines = malloc(totalLines * sizeof(char*));
    if (!data->asmLines) {
        fclose(file);
        free(data);
        return nullptr;
    }

    for (int i = 0; i < totalLines; i++) {
        data->asmLines[i] = nullptr;
    }

    // Process file and expand pseudo-instructions
    int destLine = 0;
    while (fgets(buffer, sizeof(buffer), file) && destLine < totalLines) {
        const size_t len = strlen(buffer);

        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }

        // Copy of buffer for analysis
        char analyzeBuf[1024];
        strcpy(analyzeBuf, buffer);

        // Trim leading whitespace
        char* trimmed = analyzeBuf;
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Check if this is a 'la' instruction
        bool isLaInstruction = false;
        if (strncmp(trimmed, "la ", 3) == 0 ||
            strncmp(trimmed, "la\t", 3) == 0) {
            isLaInstruction = true;

            // Parse the 'la' instruction: la rd, symbol
            char rd[10] = "";
            char symbol[256] = "";

            // Skip past the "la" part
            trimmed += 2;

            // Skip whitespace
            while (*trimmed == ' ' || *trimmed == '\t') {
                trimmed++;
            }

            // Parse register
            int i = 0;
            while (*trimmed != ',' && *trimmed != '\0' && i < 9) {
                rd[i++] = *trimmed++;
            }
            rd[i] = '\0';

            // Skip comma and whitespace
            while (*trimmed == ',' || *trimmed == ' ' || *trimmed == '\t') {
                trimmed++;
            }

            // Parse symbol
            i = 0;
            while (*trimmed != '\0' && i < 255) {
                symbol[i++] = *trimmed++;
            }
            symbol[i] = '\0';

            // Create the expanded instructions
            char* auipc = malloc(1024);
            char* addi = malloc(1024);
            char* paddingLine = malloc(2);

            if (!auipc || !addi || !paddingLine) {
                fclose(file);
                if (auipc)       free(auipc);
                if (addi)        free(addi);
                if (paddingLine) free(paddingLine);

                // Free previously allocated memory
                for (int i = 0; i < destLine; i++) {
                    if (data->asmLines[i]) free(data->asmLines[i]);
                }

                free(data->asmLines);
                free(data);
                return nullptr;
            }

            // Get the indentation from original line
            int indent = 0;
            for (indent = 0; buffer[indent] == ' ' || buffer[indent] == '\t'; indent++) {}

            char indentStr[256] = "";
            if (indent > 0) {
                strncpy(indentStr, buffer, indent);
                indentStr[indent] = '\0';
            }

            // Format the two instructions with proper indentation
            sprintf(auipc, "%sauipc %s,     %%pcrel_hi(%s)", indentStr, rd, symbol);
            sprintf(addi, "%saddi  %s, %s, %%pcrel_lo(%s)", indentStr, rd, rd, symbol);
            sprintf(paddingLine, " ");

            // Store the expanded instructions
            data->asmLines[destLine++] = auipc;
            data->asmLines[destLine++] = addi;
            data->asmLines[destLine++] = paddingLine;

            continue;
        }

        // For normal instructions, just copy the line
        data->asmLines[destLine] = malloc(strlen(buffer) + 1);
        if (!data->asmLines[destLine]) {
            fclose(file);
            for (int i = 0; i < destLine; i++) {
                if (data->asmLines[i]) free(data->asmLines[i]);
            }
            free(data->asmLines);
            free(data);
            return nullptr;
        }

        strcpy(data->asmLines[destLine], buffer);
        destLine++;
    }

    fclose(file);

    // Update the actual lineCount to reflect expanded instructions
    data->lineCount = destLine;

    data->lineToInstructionMap = malloc(sizeof(int) * data->lineCount);
    if (!data->lineToInstructionMap) {
        for (int i = 0; i < data->lineCount; i++) {
            if (data->asmLines[i]) free(data->asmLines[i]);
        }
        free(data->asmLines);
        free(data);
        return nullptr;
    }

    // First step: count instructions on .text section
    int totalInstructions = 0;
    in_text_section = false;

    for (int i = 0; i < data->lineCount; i++) {
        const char *trimmed = data->asmLines[i];

        // Skip spaces
        while (*trimmed == ' ' || *trimmed == '\t') {
            trimmed++;
        }

        // Control .text section
        if (strncmp(trimmed, ".text", 5) == 0) {
            in_text_section = true;
            continue;
        }

        // Control exit section and enter another section (example: .data or another)
        if (trimmed[0] == '.' && strncmp(trimmed, ".text", 5) != 0) {
            in_text_section = false;
            continue;
        }

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
