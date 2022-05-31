#include "HastFunctionNode.h"

HastFunctionNode::HastFunctionNode() = default;

void HastFunctionNode::add_expression(std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>> mask,
                                      std::shared_ptr<HastNode> body) {
    has_guards = false;
}

void HastFunctionNode::add_expression_with_guards(std::pair<std::string, std::vector<std::shared_ptr<HastMaskNode>>> mask,
                                      std::vector<std::pair<std::shared_ptr<HastNode>, std::shared_ptr<HastNode>>> guards) {
    has_guards = true;
}
