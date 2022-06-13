#include "HastNodeFactory.h"


HastNodeFactory HastNodeFactory::create_node(int type) {
    HastNodeFactory factory{};
    if(type == 0){
        factory.node = std::make_shared<HastNode>();
    } else if (type == 1){
        factory.node = std::make_shared<HastMaskNode>();
    } else if (type == 2){
        factory.node = std::make_shared<HastOperatorNode>();
    }
    return factory;
}

HastNodeFactory HastNodeFactory::create_node(std::shared_ptr<HastFunctionNode> node) {
    HastNodeFactory factory{};
    factory.node = std::make_shared<HastNode>();
    factory.node->type = HastNodeType::FunctionCall;
    factory.node->function_call = std::make_shared<HastFunctionCallNode>(node);
    return factory;
}

HastNodeFactory HastNodeFactory::with_type(HastNodeType type) {
    this->node->type = type;
    return *this;
}

HastNodeFactory HastNodeFactory::with_value(const std::string& val) {
    this->node->set_value(val);
    return *this;
}

std::shared_ptr<HastNode> HastNodeFactory::get_node() const {
    return this->node;
}

