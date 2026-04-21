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

// ---------------- IF ----------------
void CPU::IF_stage() {
    if (PC < instructions.size()) {
        if_id.instr = instructions[PC];
        if_id.valid = true;
        PC++;
    } else {
        if_id.valid = false;
    }
}

// ---------------- ID ----------------
void CPU::ID_stage() {
    if (!if_id.valid) {
        id_ex.valid = false;
        return;
    }

    Instruction instr = if_id.instr;

    id_ex.instr = instr;
    id_ex.readData1 = registers[instr.rs];
    id_ex.readData2 = registers[instr.rt];
    id_ex.immediate = instr.immediate;
    id_ex.valid = true;
}

// ---------------- EX ----------------
void CPU::EX_stage() {
    if (!id_ex.valid) {
        ex_mem.valid = false;
        return;
    }

    Instruction instr = id_ex.instr;
    ex_mem.instr = instr;

    int result = 0;

    if (instr.opcode == "ADD") {
        result = id_ex.readData1 + id_ex.readData2;
    }
    else if (instr.opcode == "SUB") {
        result = id_ex.readData1 - id_ex.readData2;
    }
    else if (instr.opcode == "MUL") {
        result = id_ex.readData1 * id_ex.readData2;
    }
    else if (instr.opcode == "AND") {
        result = id_ex.readData1 & id_ex.readData2;
    }
    else if (instr.opcode == "OR") {
        result = id_ex.readData1 | id_ex.readData2;
    }
    else if (instr.opcode == "ADDI") {
        result = id_ex.readData1 + id_ex.immediate;
    }
    else if (instr.opcode == "SLL") {
        result = id_ex.readData2 << id_ex.immediate;
    }
    else if (instr.opcode == "SRL") {
        result = id_ex.readData2 >> id_ex.immediate;
    }
    else if (instr.opcode == "LW" || instr.opcode == "SW") {
        result = id_ex.readData1 + id_ex.immediate; // address calc
    }
    else if (instr.opcode == "BEQ") {
        if (id_ex.readData1 == id_ex.readData2) {
            PC = PC + instr.immediate;
            if_id.valid = false; // flush
        }
    }
    else if (instr.opcode == "J") {
        PC = instr.address;
        if_id.valid = false; // flush
    }
    else if (instr.opcode == "NOP") {
        // do nothing
    }

    ex_mem.aluResult = result;
    ex_mem.writeData = id_ex.readData2;
    ex_mem.valid = true;
}

// ---------------- MEM ----------------
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
    else if (instr.opcode == "SW") {
        memory.store(ex_mem.aluResult, ex_mem.writeData);
    }

    mem_wb.aluResult = ex_mem.aluResult;
    mem_wb.valid = true;
}

// ---------------- WB ----------------
void CPU::WB_stage() {
    if (!mem_wb.valid) return;

    Instruction instr = mem_wb.instr;

    if (instr.opcode == "ADD" || instr.opcode == "SUB" ||
        instr.opcode == "MUL" || instr.opcode == "AND" ||
        instr.opcode == "OR") {
        registers[instr.rd] = mem_wb.aluResult;
    }
    else if (instr.opcode == "ADDI") {
        registers[instr.rt] = mem_wb.aluResult;
    }
    else if (instr.opcode == "SLL" || instr.opcode == "SRL") {
        registers[instr.rd] = mem_wb.aluResult;
    }
    else if (instr.opcode == "LW") {
        registers[instr.rt] = mem_wb.memData;
    }
}

// ---------------- DEBUG ----------------
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
