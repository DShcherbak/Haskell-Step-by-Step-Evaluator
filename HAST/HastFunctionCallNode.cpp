#include "HastFunctionCallNode.h"

HastFunctionCallNode::HastFunctionCallNode(std::shared_ptr<HastFunctionNode> function) {
    arity = function->arity.argument_arity.size();
    this->function = function;
}

std::shared_ptr<HastNode> HastFunctionCallNode::apply() {
    return nullptr;
}
