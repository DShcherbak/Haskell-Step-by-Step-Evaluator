#include "HastOperatorNode.h"
#include "Additional/HastNodeFactory.h"

std::pair<bool, std::shared_ptr<HastNode>> HastOperatorNode::apply() {
    return HastNode::apply();
}

bool num_operator(std::string oper){
    return  oper == "+" || oper == "-" || oper == "*" || oper == "/";
}

bool ord_operator(std::string oper){
    return  oper == "<" || oper == ">"
            || oper == "<=" || oper == ">="
            || oper == "==" || oper == "/=";
}

int get_value_from_node(std::shared_ptr<HastNode> node){
    try{
        int result = std::stoi(node->value);
        return result;
    }catch(std::exception ex){
        return 0;
    }
}


bool compareValues(std::shared_ptr<HastNode> left, std::shared_ptr<HastNode> right, std::string oper){
    std::string left_val, right_val;
    left_val = left->value;
    right_val = right->value;
    bool res;
    if(oper == ">")
        res = left_val > right_val;
    else if(oper == "<")
        res = left_val < right_val;
    else if(oper == ">=")
        res = left_val >= right_val;
    else if(oper == "<=")
        res = left_val <= right_val;
    else if(oper == "==")
        res = left_val == right_val;
    else
        res = left_val != right_val;
    return res;
}

std::shared_ptr<HastNode> HastOperatorNode::compute_fully(std::shared_ptr<HastNode> left, std::shared_ptr<HastNode> right) {
    left = left->compute_fully();
    right = right->compute_fully();
    if(num_operator(value)){
        int left_value = get_value_from_node(left);
        int right_value = get_value_from_node(right);
        int res;
        if(value == "+")
            res = left_value + right_value;
        if(value == "-")
            res = left_value - right_value;
        if(value == "*")
            res = left_value * right_value;
        if(value == "/")
            res = left_value / right_value;
        return HastNodeFactory::create_node(0).with_value(std::to_string(res)).get_node();
    } else if (ord_operator(value)){
        bool res;
        res = compareValues(left, right, value);
        return HastNodeFactory::create_node(0).with_value(res ? "True" : "False").get_node();
    } else {
        return HastNodeFactory::create_node(0).with_value("777").get_node();
       // return body->apply();
    }
}



std::pair<bool, std::shared_ptr<HastNode>>
HastOperatorNode::apply(std::shared_ptr<HastNode> left, std::shared_ptr<HastNode> right) {
    return {false, nullptr};
}
