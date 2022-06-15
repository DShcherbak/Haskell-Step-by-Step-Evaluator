#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONCALLNODE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONCALLNODE_H

class HastNode;

#include <memory>
#include <vector>
#include "HastFunctionNode.h"


class HastFunctionCallNode {
public:
    std::shared_ptr<HastFunctionNode> function;
    size_t arity = 0;
    std::vector<std::shared_ptr<HastNode>> arguments;

    HastFunctionCallNode()=default;
    HastFunctionCallNode(std::shared_ptr<HastFunctionNode> function);

    std::shared_ptr<HastNode> apply();

    std::shared_ptr<HastNode> apply(const std::vector<std::shared_ptr<HastNode>> &args);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTFUNCTIONCALLNODE_H
