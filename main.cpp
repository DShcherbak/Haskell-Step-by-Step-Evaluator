#include <iostream>
#include <vector>
#include "HaskellModel.h"

auto getFilesToParse() -> std::vector<std::string>{
    return {"file3.hs"};
}

int main() {
    HaskellModel haskellModel{};
    auto files = getFilesToParse();
    for(auto &file : files){
        haskellModel.AddFile(file);
    }
    return 0;
}
