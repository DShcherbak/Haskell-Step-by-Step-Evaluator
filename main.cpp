#include <iostream>
#include <vector>
#include "Model/HaskellModel.h"
#include "CommandController/CommandController.h"

auto getFilesToParse() -> std::vector<std::string>{
    return {"../input/file3.hs"};
}

int main() {
    std::cout << "INITIATING HASKELL STEP-BY-STEP EVALUATOR" << std::endl;
    HaskellModel haskellModel{};
    lines::HaskellFileParser fileParser{};

    haskellModel.read_prelude(lines::HaskellFileParser::read_prelude());

    auto files = getFilesToParse();
    for(auto &file : files){
        auto statements = lines::HaskellFileParser::parse_file(file);
        haskellModel.add_statements(statements);
    }

    CommandController commandController(haskellModel);
    commandController.perform_commands();

    std::cout << "Good bye!" << std::endl;
    return 0;
}
