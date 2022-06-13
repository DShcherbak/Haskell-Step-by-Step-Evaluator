#include "HastFunctionCallNode.h"

HastFunctionCallNode::HastFunctionCallNode(std::shared_ptr<HastFunctionNode> function) {
    arity = function->number_of_arguments;
    this->function = function;
}
