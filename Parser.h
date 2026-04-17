#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "Instruction.h"

using namespace std;

class Parser {
public:
    vector<Instruction> parseFile(string filename);
};

#endif
