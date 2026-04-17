#include "CPU.h"
#include <iostream>
using namespace std;

CPU::CPU(vector<Instruction> instrs) : instructions(instrs) {}

void CPU::run() {
    int cycle = 0;

    while (PC < instructions.size() || if_id.valid || id_ex.valid || ex_mem.valid || mem_wb.valid) {
        cycle++;

        WB_stage();
        MEM_stage();
        EX_stage();
        ID_stage();
        IF_stage();

        if (debugMode)
            printState(cycle);
    }

    cout << "\nFinal Register State:\n";
    for (int i = 0; i < 32; i++) {
        cout << "R" << i << ": " << registers[i] << endl;
    }
}

void CPU::IF_stage() {
    if (PC < instructions.size()) {
        if_id.instr = instructions[PC];
        if_id.valid = true;
        PC++;
    } else {
        if_id.valid = false;
    }
}

void CPU::ID_stage() {
    if (!if_id.valid) {
        id_ex.valid = false;
        return;
    }

    id_ex.instr = if_id.instr;
    id_ex.readData1 = registers[if_id.instr.rs];
    id_ex.readData2 = registers[if_id.instr.rt];
    id_ex.valid = true;
}

void CPU::EX_stage() {
    if (!id_ex.valid) {
        ex_mem.valid = false;
        return;
    }

    Instruction instr = id_ex.instr;
    ex_mem.instr = instr;

    if (instr.opcode == "ADD") {
        ex_mem.aluResult = id_ex.readData1 + id_ex.readData2;
    }

    ex_mem.valid = true;
}

void CPU::MEM_stage() {
    if (!ex_mem.valid) {
        mem_wb.valid = false;
        return;
    }

    Instruction instr = ex_mem.instr;
    mem_wb.instr = instr;

    if (instr.opcode == "LW") {
        mem_wb.memData = memory.load(ex_mem.aluResult);
    }

    mem_wb.aluResult = ex_mem.aluResult;
    mem_wb.valid = true;
}

void CPU::WB_stage() {
    if (!mem_wb.valid) return;

    Instruction instr = mem_wb.instr;

    if (instr.opcode == "ADD") {
        registers[instr.rd] = mem_wb.aluResult;
    }

    if (instr.opcode == "LW") {
        registers[instr.rt] = mem_wb.memData;
    }
}

void CPU::printState(int cycle) {
    cout << "\nCycle " << cycle << ":\n";

    if (if_id.valid) cout << "IF/ID: " << if_id.instr.raw << endl;
    if (id_ex.valid) cout << "ID/EX: " << id_ex.instr.raw << endl;
    if (ex_mem.valid) cout << "EX/MEM: " << ex_mem.instr.raw << endl;
    if (mem_wb.valid) cout << "MEM/WB: " << mem_wb.instr.raw << endl;

    cout << "Registers:\n";
    for (int i = 0; i < 8; i++) {
        cout << "R" << i << ":" << registers[i] << " ";
    }
    cout << endl;
}
