#include <iostream>
#include "Parser.h"
#include "CPU.h"

using namespace std;

int main() {
    Parser parser;
    vector<Instruction> instructions = parser.parseFile("sample.asm");

    CPU cpu(instructions);
    cpu.run();

    return 0;
}
