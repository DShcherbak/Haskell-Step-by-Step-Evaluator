#include "HastNodeFactory.h"


HastNodeFactory HastNodeFactory::create_node(int type) {
    HastNodeFactory factory{};
    if(type == 0){
        factory.node = std::make_shared<HastNode>();
    } else if (type == 1){
        factory.node = std::make_shared<HastMaskNode>();
    } else if (type == 2){
        factory.node = std::make_shared<HastOperatorNode>();
    } else if (type == 3){
        factory.node = std::make_shared<HastFunctionNode>();
    }
    return factory;
}

HastNodeFactory HastNodeFactory::create_node(std::shared_ptr<HastFunctionNode> node) {
    HastNodeFactory factory{};
    factory.func_node = std::make_shared<HastFunctionNode>();
    factory.func_node->type = node->type;
    for(auto const & mask : node->masks){
        factory.func_node->masks.push_back(mask);
    }
    factory.func_node->number_of_arguments = node->number_of_arguments;
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

std::shared_ptr<HastFunctionNode> HastNodeFactory::get_func_node() {
    return this->func_node;
}

