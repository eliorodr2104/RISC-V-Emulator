//
// Created by Eliomar Alejandro Rodriguez Ferrer on 02/06/25.
//

#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H
#include <stdint.h>

typedef struct {
    bool branch;      // 1 se istruzione di branch
    bool memRead;     // 1 se legge dalla memoria
    bool memToReg;    // 1 se il dato da scrivere nel registro viene dalla memoria
    uint8_t operation;    // Codice operazione ALU (2 bit: 00, 01, 10)
    bool memWrite;    // 1 se scrive in memoria
    bool aluSrc;      // 1 se secondo operando ALU viene da immediato
    bool regWrite;    // 1 se scrive nel registro destinazione
} ControlSignals;

ControlSignals getControlSignals(
    uint8_t opcode
);

#endif //CONTROLUNIT_H
