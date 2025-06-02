#include "include/cpu.h"

void initWord(
    bool word[32],
    const int32_t value
) {

    for (int i = 0; i < 32; i++) {
        word[31 - i] = (value >> i) & 1;
    }
}

int main(void) {

    Cpu* cpu = newCpu();

    runCpu(cpu);

    return 0;
}
