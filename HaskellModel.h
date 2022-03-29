#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#include <string>
#include <fstream>
#include <unordered_map>
#include <memory>
#include "dataType.h"
#include "HaskellFunction.h"
#include <vector>
#include <tuple>
#include <iostream>
#include "Lines/CompressedLine.h"


#define indent_vector std::vector<std::pair<std::string, int>>
#define compressed_vector std::vector<lines::CompressedLine>
#define lines_vector std::vector<std::string>

class HaskellModel {
public:
    void AddFile(const std::string& fileName);

private:
    std::unordered_map<std::string, std::shared_ptr<HaskellFunction>> functions;
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
