//
// Created by sadoffnick on 5/30/22.
//

#include "HastNode.h"
#include "HastFunctionCallNode.h"

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
        case HastNodeType::FunctionCall:
            return "F";
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
    } else if(value.find('.') != std::string::npos && std::isdigit(value[0])){
        type = HastNodeType::Double;
    } else if(value.length() > 0 && std::isdigit(value[0])){
        type = HastNodeType::Int;
    } else if(value.length() > 0 && std::islower(value[0])){
        type = HastNodeType::Variable;
    } else if(value.length() > 0 && std::isupper(value[0])){
        type = HastNodeType::DataConstructor;
    } else if(value.length() > 0 && value[0] == ':'){
        type = HastNodeType::InfixDataConstructor;
    } else if(value.length() > 0 && value[0] == ','){
        type = HastNodeType::Comma;
    } else if(value.length() > 0 && (value[0] == '+' || value[0] == '-' || value[0] == '*'
    || value[0] == '/'  || value[0] == '>'  || value[0] == '<' || value[0] == '='
    || value[0] == '.')){//TODO: is_operator
        type = HastNodeType::Operator;
    } else {
        type = HastNodeType::Any;
    }
}

std::shared_ptr<HastNode> HastNode::compute_fully() {
    if(type == HastNodeType::FunctionCall){
        auto function_result = function_call->apply();
        return function_result->compute_fully();
    } else if (type == HastNodeType::List || type == HastNodeType::Tuple){
        auto result = HastNodeFactory::create_node(0).with_type(type).get_node();
        result->first = first->compute_fully();
        if(rest)
            result->rest = rest->compute_fully();
        return result;
    } //operator
    return nullptr;
}

std::pair<bool,std::shared_ptr<HastNode>> HastNode::apply() {
    if(type == HastNodeType::FunctionCall){
        auto function_result = function_call->apply();
        return {true, function_result};
    } else if (type == HastNodeType::List || type == HastNodeType::Tuple){//operator
        auto result = HastNodeFactory::create_node(0).with_type(type).get_node();
        if(first)
            result->first = first;
        if (rest)
            result->rest = rest;
        if(first){
            auto apply_result = first->apply();
            if(apply_result.first) {
                result->first = apply_result.second;
                return {true, result};
            }
        }
        if(rest){
            auto apply_result = rest->apply();
            if(apply_result.first) {
                result->rest = apply_result.second;
                return {true, result};
            }
        }
        return {false, result};
    } //operator
    return {false, nullptr};
}


