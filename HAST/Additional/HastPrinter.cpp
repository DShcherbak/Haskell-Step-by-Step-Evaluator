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
