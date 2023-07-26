#include <map>
#include "Parser.hpp"

class InstructionHandler {
public:
    InstructionHandler(Parser& parser) : parser(parser) {
        initJumpMap();
        initDestMap();
        initCompMap();
        initSymbols();
    }

    string handleAInstruction() const;
    string handleCInstruction() const;
private:
    Parser& parser;

    string handleSymbol() const;

    string getComp() const;

    string handleNumber() const;

    map<string, string> jumpMap;
    void initJumpMap();

    map<string, string> destMap;
    void initDestMap();

    map<string, string> compMap;
    void initCompMap();

    map<string, string> symbols;
    void initSymbols();
};
