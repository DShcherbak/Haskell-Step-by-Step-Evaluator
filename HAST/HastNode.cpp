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
    } else if (value == "True" || value == "False"){
        type = HastNodeType::Bool;
    } else if(value.find('.') != std::string::npos){
        type = HastNodeType::Double;
    } else {
        type = HastNodeType::Int;
    }
}

HastNode::HastNode(HastNodeType type) {
    this->type = type;
}

std::string HastNode::type_to_str(HastNodeType type) {
    switch(type){
        case HastNodeType::Any:
            return "_";
        case HastNodeType::String:
            return "S";
        case HastNodeType::Char:
            return "C";
        case HastNodeType::Int:
            return "I";
        case HastNodeType::Double:
            return "F";
        case HastNodeType::Bool:
            return "B";
        case HastNodeType::List:
            return "L";
        case HastNodeType::Tuple:
            return "T";
        case HastNodeType::DataConstructor:
            return "D";
        case HastNodeType::Variable:
            return "V";
    }
    return "?";
}
