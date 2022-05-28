#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#include <string>
#include <fstream>
#include "HaskellFunction/Function.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <unordered_map>
#include <map>
#include "../Lines/HaskellFileParser.h"


class HaskellModel {
public:
    void AddStatements(std::vector<string> &statements);


private:
    std::map<std::string, std::shared_ptr<function::Function>> functions;

    using LineStatements = std::vector<lines::LineStatement>;

    void process_functions(const LineStatements &statements);
    LineStatements process_type_classes(const LineStatements &statements);
    LineStatements process_data_types(const LineStatements &statements);
    LineStatements process_headers(const LineStatements &statements);

    std::shared_ptr<function::Function> GetOrCreateFunction(const string &name);

    void function_type_define(const string &name, const lines::LineStatement &stmnt);

    void add_expression_to_function(const string &name, const lines::LineStatement &statements);

    void process_function_guards(std::shared_ptr<function::Mask> mask, const string &line, size_t head_start);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
