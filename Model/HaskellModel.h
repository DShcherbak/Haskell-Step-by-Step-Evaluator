#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#include <string>
#include <fstream>
#include "HaskellFunction.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <unordered_map>
#include "../Lines/HaskellFileParser.h"


class HaskellModel {
public:
    void AddStatements(statement_vector& statements);

private:
    std::unordered_map<std::string, std::shared_ptr<HaskellFunction>> functions;

    void process_functions(const std::vector<lines::Statement> &statements);
    std::vector<lines::Statement> process_type_classes(const std::vector<lines::Statement> &statements);
    std::vector<lines::Statement> process_data_types(const std::vector<lines::Statement> &statements);
    std::vector<lines::Statement> process_headers(const std::vector<lines::Statement> &statements);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
