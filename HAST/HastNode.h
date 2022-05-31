#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H

#include <memory>
#include "../Lexer/TokenTree.h"

//Haskell Abstract Tree Node

#define HAST_N std::shared_ptr<HastNode>

enum class HastNodeType{String, Char, Int, Double, List, Tuple, DataConstructor, Any, Variable};

class HastNode {
public:
    std::shared_ptr<HastNode> first;
    std::shared_ptr<HastNode> rest;
    std::shared_ptr<HastNode> parent;
    std::string value;
    std::string name;
    HastNodeType type;
    explicit HastNode()=default;
    explicit HastNode(const TokenTree& tree);
    explicit HastNode(const std::string& tree);
    explicit HastNode(const TokenNode& tree);

};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTNODE_H
