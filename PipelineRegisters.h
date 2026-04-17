#ifndef PIPELINE_REGISTERS_H
#define PIPELINE_REGISTERS_H

#include "Instruction.h"

struct IF_ID {
    Instruction instr;
    bool valid = false;
};

struct ID_EX {
    Instruction instr;
    int readData1 = 0;
    int readData2 = 0;
    bool valid = false;
};

struct EX_MEM {
    Instruction instr;
    int aluResult = 0;
    int writeData = 0;
    bool valid = false;
};

struct MEM_WB {
    Instruction instr;
    int memData = 0;
    int aluResult = 0;
    bool valid = false;
};

#endif
