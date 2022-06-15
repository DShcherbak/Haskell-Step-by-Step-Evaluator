#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORCALLNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORCALLNODE_H

#include <memory>
#include "HastOperatorNode.h"

class HastOperatorCallNode {
public:

    std::shared_ptr<HastOperatorNode> operator_node;
    size_t arity = 0;
    std::shared_ptr<HastNode> left_argument;
    std::shared_ptr<HastNode> right_argument;

    HastOperatorCallNode()=default;
    explicit HastOperatorCallNode(std::shared_ptr<HastOperatorNode> function);
    void add_operand(std::shared_ptr<HastNode> arg, bool left = true);

    std::shared_ptr<HastNode> apply();
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTOPERATORCALLNODE_H
