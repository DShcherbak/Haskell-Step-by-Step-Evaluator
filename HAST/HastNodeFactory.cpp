#include "HastNodeFactory.h"


HastNodeFactory HastNodeFactory::create_node(int type) {
    HastNodeFactory factory{};
    if(type == 0){
        factory.node = std::shared_ptr<HastNode>();
    } else if (type == 1){
        factory.node = std::shared_ptr<HastMaskNode>();
    } else if (type == 2){
        factory.node = std::shared_ptr<HastOperatorNode>();
    }
    return factory;
}

HastNodeFactory HastNodeFactory::with_type(HastNodeType type) {
    node->type = type;
    return *this;
}

HastNodeFactory HastNodeFactory::with_value(const std::string& val) {
    this->node->value = val;
    return *this;
}

std::shared_ptr<HastNode> HastNodeFactory::get_node() {
    return node;
}

