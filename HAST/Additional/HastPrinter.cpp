#include <iostream>
#include "HastPrinter.h"


void HastPrinter::print_node(const std::shared_ptr<HastNode>& node, int depth) {
    if(node == nullptr)
        return;
    for(int i = 0; i < depth; i++){
        std::cout << "\t";
    }
    std::cout << "(" << HastNode::type_to_str(node->type) <<")" << node->value << std::endl;
    if(node->first != nullptr)
        print_node(node->first, depth);
    if(node->rest != nullptr)
        print_node(node->rest, depth+1);
    if(node->type == HastNodeType::FunctionCall){
        for(auto const & arg : node->function_call->arguments){
            print_node(arg, depth+1);
        }
    }

}

std::string HastPrinter::node_to_string(const std::shared_ptr<HastNode> &node) {
    std::string first_string, rest_string, body;
    if(node->first){
        first_string = HastPrinter::node_to_string(node->first);
    }
    if(node->rest){
        rest_string = HastPrinter::node_to_string(node->rest);
    }
    std::string result;
    switch(node->type){
        case HastNodeType::String:
        case HastNodeType::Char:
        case HastNodeType::Int:
        case HastNodeType::Double:
        case HastNodeType::Bool:
        case HastNodeType::EmptyTuple:
        case HastNodeType::Comma:
        case HastNodeType::Any:
        case HastNodeType::Variable:
            return node->value;
        case HastNodeType::PrefixComma:
            return "(,)";
        case HastNodeType::List:
            if(node->value == "[]")
                return "([])";
            return "(" + first_string + ":" + rest_string.substr(1);
        case HastNodeType::Tuple:
            if(rest_string.empty())
                return "(" + first_string + ")";
            return "(" + first_string + "," + rest_string.substr(1);
        case HastNodeType::DataConstructor:
        case HastNodeType::PrefixOperator:
            if(!node->value.empty())
                result = node->value;
            if(!first_string.empty())
                result +=  " " + first_string;
            if(!rest_string.empty())
                result +=  " " + rest_string;
            return result;
        case HastNodeType::InfixDataConstructor:
        case HastNodeType::Operator:
            return first_string + node->value + rest_string;
        case HastNodeType::FunctionCall:
            return HastPrinter::node_to_string(node->function_call);
    }
    return "";
}

std::string HastPrinter::node_to_string(const std::shared_ptr<HastFunctionCallNode>& node){

}
