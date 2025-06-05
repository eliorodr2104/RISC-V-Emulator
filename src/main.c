#include <stdio.h>

#include "tui.h"
#include "cpu.h"

int main(void) {
    headerProgram();

    Cpu* cpu = newCpu();

    userChoices();

    char choice[3];

    if (fgets(choice, sizeof(choice), stdin) != NULL) {

        switch(choice[0]) {
            case '1':
                printf("\n Mode: Full execution\n");
                printf("═══════════════════════════════════════════════════════════════\n");
                runCpuFull(cpu);
                break;

            case '2':
                runCpuStepByStep(cpu);
                break;

            case '3':
                printf("\nExit\n");
                break;

            default:
                printf("\nError\n");
                break;
        }
    }

    free(cpu);
    return 0;
}
