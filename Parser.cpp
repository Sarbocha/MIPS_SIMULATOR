#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

        // VERY BASIC parsing (you will improve this)
        if (inst.opcode == "ADD") {
            char comma;
            ss >> inst.rd >> comma >> inst.rs >> comma >> inst.rt;
        }

        instructions.push_back(inst);
    }

    return instructions;
}
