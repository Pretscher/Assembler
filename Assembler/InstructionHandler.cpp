#include "InstructionHandler.hpp"


string InstructionHandler::handleAInstruction() const {
    if (parser.isSymbol()) {
        return handleSymbol();
    }
    else {
        return handleNumber();
    }
}

string InstructionHandler::handleCInstruction() const {
    string instruction;
    instruction.append("111");
    instruction.append(getComp());
    instruction.append(destMap.at(parser.dest()));
    instruction.append(jumpMap.at(parser.jump()));
    return instruction;
}


string InstructionHandler::handleSymbol() const {
    string line = parser.getLine();
    line.erase(0, 1);
    string s = symbols.at(line);
    return symbols.at(line);
}

string InstructionHandler::getComp() const {
    string out;
    string comp = parser.comp();
    int place = comp.find('M');
    if (place != std::string::npos) {
        out.append("1");
        comp.replace(place, place + 1, "A");//just act like we would use A, so that we dont have to make a difference in the table.
    }
    else {
        out.append("0");
    }
    out.append(compMap.at(comp));
    return out;
}

string InstructionHandler::handleNumber() const {
    string s = parser.getLine().erase(0, 1);//erase the @
    string instruction = Utility::toBinary(std::stoi(s));
    //fill up with zeros. at least one zero will be added here always to signal that it's an 
    //a instruction, no matter how large the number, because in toBinary we cut off at 15 bits
    for (size_t i = instruction.length(); i < 16; i++) {
        instruction.insert(0, "0");
    }
    return instruction;
}

void InstructionHandler::initJumpMap() {
    jumpMap["null"] = "000";
    jumpMap["JGT"] = "001";
    jumpMap["JEQ"] = "010";
    jumpMap["JGE"] = "011";
    jumpMap["JLT"] = "100";
    jumpMap["JNE"] = "101";
    jumpMap["JLE"] = "110";
    jumpMap["JMP"] = "111";
}

void InstructionHandler::initDestMap() {
    destMap["null"] = "000";
    destMap["M"] = "001";
    destMap["D"] = "010";
    destMap["MD"] = "011";
    destMap["DM"] = "011";
    destMap["A"] = "100";
    destMap["AM"] = "101";
    destMap["MA"] = "101";
    destMap["AD"] = "110";
    destMap["DA"] = "110";
    destMap["ADM"] = "111";
}

void InstructionHandler::initCompMap() {
    compMap["0"] = "101010";
    compMap["1"] = "111111";
    compMap["-1"] = "111010";
    compMap["D"] = "001100";
    compMap["A"] = "110000";
    compMap["!D"] = "001101";
    compMap["!A"] = "110001";
    compMap["-D"] = "001111";
    compMap["-A"] = "110011";
    compMap["D+1"] = "011111";
    compMap["A+1"] = "110111";
    compMap["D-1"] = "001110";
    compMap["A-1"] = "110010";
    compMap["D+A"] = "000010";
    compMap["D-A"] = "010011";
    compMap["A-D"] = "000111";
    compMap["D&A"] = "000000";
    compMap["D|A"] = "010101";
}

void InstructionHandler::initSymbols() {
    for (int i = 0; i < 16; i++) {
        string r = "R";
        r.append(std::to_string(i));
        symbols[r] = Utility::toBinary(i);
        symbols[r].insert(0, "0");//toBinary only writes 15 bits
    }
    symbols["SCREEN"] = "0100000000000000";
    symbols["KBD"] = "0110000000000000";
    symbols["SP"] = "0000000000000000";
    symbols["LCL"] = "0000000000000001";
    symbols["ARG"] = "0000000000000010";
    symbols["THIS"] = "0000000000000011";
    symbols["THAT"] = "0000000000000100";

    int customSymbols = 16;
    int labelCount = 0;
    //write l-instructions
    while (parser.hasMoreLines()) {
        if (parser.instructionType() == 1) {
            string line = parser.getLine();
            //remove brackets
            line.erase(0, 1);
            line.erase(line.size() - 1);

            /*
                substract the amount of labels from the line number, because the labels are not in the output lines.
                For the first label, nothing needs to be substracted because it is erased from the actual lines, and 
                the next instruction takes its place.
            */
            int lineNumber = parser.getLineNumber() - labelCount++;
            symbols[line] = Utility::toBinary(lineNumber);
            symbols[line].insert(0, "0");
        }
        parser.advance();
    }
    parser.reset();
    //needs to be done in a second loop, because we dont want to add label-calls to the map as symbols, so we need to know all labels
    while (parser.hasMoreLines()) {
        if (parser.instructionType() == 0 && Utility::isLetter(parser.getLine().at(1))) {
            string line = parser.getLine();
            //remove brackets
            line.erase(0, 1);
            //if a label, dont init a symbol
            if (symbols.find(line) == symbols.end()) {
                symbols[line] = Utility::toBinary(customSymbols);
                symbols[line].insert(0, "0");
                customSymbols++;
            }
        }
        parser.advance();
    }
    parser.reset();
}