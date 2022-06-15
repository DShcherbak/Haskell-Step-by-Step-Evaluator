#include "HastFunctionCallNode.h"

HastFunctionCallNode::HastFunctionCallNode(std::shared_ptr<HastFunctionNode> function) {
    arity = function->arity.arity();
    this->function = function;
}

std::shared_ptr<HastNode> HastFunctionCallNode::apply() {
    return function->apply(arguments);
}
