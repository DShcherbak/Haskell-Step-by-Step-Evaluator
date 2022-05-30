//
// Created by sadoffnick on 5/30/22.
//

#include "HastNode.h"

HastNode::HastNode(const TokenNode &node) {
    if(node.which() == 0)
        HastNode(get<TokenTree>(node));
    else
        HastNode(get<std::string>(node));
}

HastNode::HastNode(const TokenTree &tree) {

}

HastNode::HastNode(const std::string &value) {
    this->value = value;
    if(value.starts_with("\"")){
        type = HastNodeType::String;
    } else if(value.starts_with("\'")) {
        type = HastNodeType::Char;
    } else if(value.find('.') != std::string::npos){
        type = HastNodeType::Double;
    } else {
        type = HastNodeType::Int;
    }
}
