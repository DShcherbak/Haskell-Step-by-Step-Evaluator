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
#include "../HAST/HastFunctionNode.h"
#include "../HAST/HastOperatorNode.h"
#include "../HAST/HastNodeFactory.h"

typedef std::vector<TokenNode> TokenList;
typedef std::vector<std::pair<TokenList, TokenList>> GuardVector;

class HaskellModel {
public:

    std::string moduleName;
    void AddStatements(std::vector<string> &statements);

    void read_prelude(const std::vector<std::string>& lines);

private:
    std::map<std::string, HAST_FN> functions;
    std::map<std::string, HAST_ON> operators;

    std::vector<TokenTree> process_headers(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_data_types(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_type_classes(const std::vector<TokenTree> &statements);
    void process_functions(const std::vector<TokenTree> &statements);

    HAST_FN get_or_create_function(const string &name);

    void setName(const std::string &name);
    void addImportToModel(const std::vector<TokenNode> &import);

    void add_data_structure(const TokenTree &tree);

    std::vector<std::tuple<TokenList, GuardVector>> add_function_arity(const std::vector<TokenTree> &trees);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
