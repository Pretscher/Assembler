#include <iostream>
#include <map>
#include "Parser.hpp"
#include "InstructionHandler.hpp"
using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorret amount of arguments. Exiting...";
        return -1;
    }
    Parser parser(argv[1]);
    InstructionHandler handler(parser);
    vector<string> instructions;
    while (parser.hasMoreLines()) {
        int type = parser.instructionType();
        string instruction;
        if (type == 0) {//a-instruction
            instruction = handler.handleAInstruction();
        }
        else if (type == 2) {//d-instruction
           instruction = handler.handleCInstruction();
        }
        else if (type == 3) {//comments and white space (are not pushed back)
            parser.advance();
            continue;
        }
        //l-instructions will leave the line empty, so we dont push them back.
        if (instruction.empty() == false) {
            instructions.push_back(instruction);
        }
        parser.advance();
    }
    
    for (string instruction : instructions) {
        std::cout << instruction << "\n";
    }

    parser.writeFile(argv[1], instructions);
    return 0;
}