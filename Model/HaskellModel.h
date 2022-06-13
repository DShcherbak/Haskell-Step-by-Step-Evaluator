#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H

class HastFunctionNode;

#include <string>
#include <fstream>
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
#include "../HAST/Additional/HastNodeFactory.h"
#include "../CommandController/ControllerCommand.h"

typedef std::vector<TokenNode> TokenList;
typedef std::vector<std::pair<TokenList, TokenList>> GuardVector;

class HaskellModel {
public:
    std::map<std::string, std::shared_ptr<HastFunctionNode>> functions;
    std::map<std::string, std::shared_ptr<HastOperatorNode>> operators;
    std::set<std::string> infix_data_constructors;
    std::map<std::string, int> data_constructor_arity;
    std::string moduleName;
    void add_statements(std::vector<std::string> &statements);
    void read_prelude(const std::vector<std::string>& lines);

    bool parse_expression(std::string basicString);

    std::string current_expression();

    bool expression_not_week_normal_form();
    bool expression_not_reduced();

    void perform_command(ControllerCommand command);

private:
    std::string temporary_countdown;

    std::vector<TokenTree> process_headers(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_data_types(const std::vector<TokenTree> &statements);
    std::vector<TokenTree> process_type_classes(const std::vector<TokenTree> &statements);
    void process_functions(const std::vector<TokenTree> &statements);

    std::shared_ptr<HastFunctionNode> get_or_create_function(const std::string &name);

    void setName(const std::string &name);
    void addImportToModel(const std::vector<TokenNode> &import);

    void add_data_structure(const TokenTree &tree);

    std::vector<std::tuple<TokenList, GuardVector>> add_function_arity(const std::vector<TokenTree> &trees);


    std::shared_ptr<HastMaskNode> make_mask(const TokenNode &token);

    std::shared_ptr<HastMaskNode> make_mask(const TokenTree &tree);

    std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>> build_full_function_mask(const TokenList &list);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASKELLMODEL_H
