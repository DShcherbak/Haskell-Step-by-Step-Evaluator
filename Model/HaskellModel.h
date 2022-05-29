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
#include "../Lexer/Lexer.h"
#include "../Exception/IncorrectTokenException.h"


class HaskellModel {
public:
    std::string moduleName;
    void AddStatements(std::vector<string> &statements);


private:
    std::map<std::string, std::shared_ptr<function::Function>> functions;

    using LineStatements = std::vector<lines::LineStatement>;

    std::vector<TokenTree> process_headers(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_data_types(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_type_classes(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_functions(const std::vector<TokenTree> &statements);

    std::shared_ptr<function::Function> GetOrCreateFunction(const string &name);

    void function_type_define(const string &name, const lines::LineStatement &stmnt);
    void add_expression_to_function(const string &name, const lines::LineStatement &statements);

    void setName(const std::string &name);
    void addImportToModel(const std::vector<TokenNode> &import);

    void add_data_structure(const TokenTree &tree);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
