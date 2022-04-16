#include <iostream>
#include <vector>
#include "Model/HaskellModel.h"

auto getFilesToParse() -> std::vector<std::string>{
    return {"../input/file2.hs"};
}

int main() {
    HaskellModel haskellModel{};
    lines::HaskellFileParser fileParser{};
    auto files = getFilesToParse();
    for(auto &file : files){
        auto statements = fileParser.ParseFile(file);
        haskellModel.AddStatements(statements);
    }
    return 0;
}
