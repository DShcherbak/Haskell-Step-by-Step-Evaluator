#include "HastFunctionNode.h"

HastFunctionNode::HastFunctionNode() = default;

void HastFunctionNode::add_expression(std::pair<std::string, std::vector<std::shared_ptr<HastNode>>> mask,
                                      std::shared_ptr<HastNode> body) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second);
    mask_element->add_body(body);
    masks.emplace_back(mask_element);
}

void HastFunctionNode::add_expression_with_guards(std::pair<std::string, std::vector<std::shared_ptr<HastNode>>> mask,
                                      std::vector<std::pair<std::shared_ptr<HastNode>, std::shared_ptr<HastNode>>> guards) {
    auto mask_element = std::make_shared<function::Mask>();
    name = mask.first;
    mask_element->add_arg_templates(mask.second);
    mask_element->add_guards(guards);
    masks.emplace_back(mask_element);
}
