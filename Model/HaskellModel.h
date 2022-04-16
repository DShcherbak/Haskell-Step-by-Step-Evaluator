#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#include <string>
#include <fstream>
#include "HaskellFunction/Function.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <unordered_map>
#include "../Lines/HaskellFileParser.h"


class HaskellModel {
public:
    void AddStatements(statement_vector& statements);

private:
    std::unordered_map<std::string, std::shared_ptr<function::Function>> functions;

    void process_functions(const std::vector<lines::LineStatement> &statements);
    std::vector<lines::LineStatement> process_type_classes(const std::vector<lines::LineStatement> &statements);
    std::vector<lines::LineStatement> process_data_types(const std::vector<lines::LineStatement> &statements);
    std::vector<lines::LineStatement> process_headers(const std::vector<lines::LineStatement> &statements);

    std::shared_ptr<function::Function> GetOrCreateFunction(const string &name);

    void function_type_define(const string &name, const lines::LineStatement &stmnt);

    void add_expression_to_function(const string &name, const lines::LineStatement &statements);

    void process_function_guards(std::shared_ptr<function::Mask> mask, const string &line, size_t head_start);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
