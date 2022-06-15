#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H

#include "HastNode.h"

#define HAST_ON std::shared_ptr<HastOperatorNode>

enum class OperatorAssociativity {Left,Right,None};

class HastOperatorNode : public HastNode {
public:
   // std::shared_ptr<HastFunctionNode> body;
    int precedence = 0;
    OperatorAssociativity associativity = OperatorAssociativity::Left;
    std::pair<bool,std::shared_ptr<HastNode>> apply();
    std::pair<bool,std::shared_ptr<HastNode>> apply(std::shared_ptr<HastNode> left, std::shared_ptr<HastNode> right);
    std::shared_ptr<HastNode> compute_fully(std::shared_ptr<HastNode> left, std::shared_ptr<HastNode> right);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORNODE_H
