#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<Instruction> Parser::parseFile(string filename) {
    vector<Instruction> instructions;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        Instruction inst;
        inst.raw = line;

        stringstream ss(line);
        ss >> inst.opcode;

        char comma;

        // ---------------- R-TYPE ----------------
        if (inst.opcode == "ADD" || inst.opcode == "SUB" ||
            inst.opcode == "MUL" || inst.opcode == "AND" ||
            inst.opcode == "OR") {

            // Format: OPCODE rd, rs, rt
            ss >> inst.rd >> comma >> inst.rs >> comma >> inst.rt;
        }

        // ---------------- ADDI ----------------
        else if (inst.opcode == "ADDI") {
            // Format: ADDI rt, rs, immediate
            ss >> inst.rt >> comma >> inst.rs >> comma >> inst.immediate;
        }

        // ---------------- SHIFT ----------------
        else if (inst.opcode == "SLL" || inst.opcode == "SRL") {
            // Format: OPCODE rd, rt, shamt
            ss >> inst.rd >> comma >> inst.rt >> comma >> inst.immediate;
        }

        // ---------------- LOAD ----------------
        else if (inst.opcode == "LW") {
            // Format: LW rt, offset, rs
            ss >> inst.rt >> comma >> inst.immediate >> comma >> inst.rs;
        }

        // ---------------- STORE ----------------
        else if (inst.opcode == "SW") {
            // Format: SW rt, offset, rs
            ss >> inst.rt >> comma >> inst.immediate >> comma >> inst.rs;
        }

        // ---------------- BRANCH ----------------
        else if (inst.opcode == "BEQ") {
            // Format: BEQ rs, rt, offset
            ss >> inst.rs >> comma >> inst.rt >> comma >> inst.immediate;
        }

        // ---------------- JUMP ----------------
        else if (inst.opcode == "J") {
            // Format: J address
            ss >> inst.address;
        }

        // ---------------- NOP ----------------
        else if (inst.opcode == "NOP") {
            // do nothing
        }

        // ---------------- UNKNOWN ----------------
        else {
            cout << "Warning: Unknown opcode -> " << inst.opcode << endl;
        }

        instructions.push_back(inst);
    }

    return instructions;
}
