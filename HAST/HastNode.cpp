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
    set_value(value);
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
        case HastNodeType::Operator:
            return "O";
        case HastNodeType::PrefixOperator:
            return "Pref0";
        case HastNodeType::InfixDataConstructor:
            return "InfixD";
        case HastNodeType::Variable:
            return "V";
        case HastNodeType::EmptyTuple:
            return "()";
    }
    return "?";
}

void HastNode::set_value(const std::string &value) {
    this->value = value;
    if(value == "[]"){
        type = HastNodeType::List;
    }else if(value == "()") {
        type = HastNodeType::EmptyTuple;
    }else if(value.starts_with("\"")){
        type = HastNodeType::String;
    } else if(value.starts_with("\'")) {
        type = HastNodeType::Char;
    } else if (value == "True" || value == "False"){
        type = HastNodeType::Bool;
    } else if(value.find('.') != std::string::npos){
        type = HastNodeType::Double;
    } else if(value.length() > 0 && std::isdigit(value[0])){
        type = HastNodeType::Int;
    } else if(value.length() > 0 && std::islower(value[0])){
        type = HastNodeType::Variable;
    } else if(value.length() > 0 && std::isupper(value[0])){
        type = HastNodeType::DataConstructor;
    } else if(value.length() > 0 && value[0] == ':'){
        type = HastNodeType::InfixDataConstructor;
    } else if(value.length() > 0 && (value[0] == '+' || value[0] == '-' || value[0] == '*' || value[0] == '/')){//TODO: is_operator
        type = HastNodeType::Operator;
    } else {
        type = HastNodeType::Any;
    }
}
