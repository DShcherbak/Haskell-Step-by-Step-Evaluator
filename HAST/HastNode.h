#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H

class HastFunctionCallNode;

#include <memory>
#include <map>
#include "../Lexer/TokenTree.h"

//Haskell Abstract Tree Node

#define HAST_N std::shared_ptr<HastNode>

enum class HastNodeType{String, Char, Int, Double, Bool,
                            List, Tuple, EmptyTuple, DataConstructor, InfixDataConstructor,
                            Comma, PrefixComma,
                            Operator, PrefixOperator,
                            Any, Variable, FunctionCall};

class HastNode {
public:
    std::shared_ptr<HastNode> first;
    std::shared_ptr<HastNode> rest;
    std::shared_ptr<HastNode> parent;
    std::shared_ptr<HastFunctionCallNode> function_call;

    std::string value;
    std::string name;
    void set_value(const std::string& value);
    HastNodeType type;
    explicit HastNode()=default;
    explicit HastNode(HastNodeType type);
    explicit HastNode(const TokenTree& tree);
    explicit HastNode(const std::string& tree);
    explicit HastNode(const TokenNode& tree);

    static std::string type_to_str(HastNodeType type);

    std::pair<bool,std::shared_ptr<HastNode>> apply();
    std::shared_ptr<HastNode> compute_fully();
};

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H
