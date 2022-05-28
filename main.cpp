#include <iostream>
#include <vector>
#include "Model/HaskellModel.h"

auto getFilesToParse() -> std::vector<std::string>{
    return {"../input/file2.hs"};
}

int main() {
    std::cout << "INITIATING HASKELL STEP-BY-STEP-EVALUATOR" << std::endl;
    HaskellModel haskellModel{};
    lines::HaskellFileParser fileParser{};
    auto files = getFilesToParse();
    for(auto &file : files){
        auto statements = fileParser.parse_file(file);
        haskellModel.AddStatements(statements);
    }
    return 0;
}
