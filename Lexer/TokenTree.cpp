#include <iostream>
#include "TokenTree.h"




void TokenTree::print() const {
    token_tree_printer printer;
    printer(*this);
}


void tab(int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << '\t';
}

void token_tree_node_printer::operator()(TokenTree const& xml) const
{
    token_tree_printer(indent + 1)(xml);
}

void token_tree_node_printer::operator()(std::string const& text) const
{
    tab(indent+1);
    std::cout << "value: \"" << text << '"' << std::endl;
}

void token_tree_printer::operator()(TokenTree const& xml) const
{
    tab(indent);
    // std::cout << "operation: " << xml.operation << std::endl;
    // std::cout << (xml.type == ExpressionType::Value ? "EXPRESSION" : (xml.type == ExpressionType::Curl ? "Curl:" : "List:")) << std::endl;
    tab(indent);
    std::cout << '{' << std::endl;
    for(TokenNode const &node: xml.children){
        boost::apply_visitor(token_tree_node_printer(indent), node);
    }

    tab(indent);
    std::cout << '}' << std::endl;
}
