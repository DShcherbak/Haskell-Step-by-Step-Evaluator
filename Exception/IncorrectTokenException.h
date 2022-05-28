#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H

#include <iostream>
#include "CustomException.h"
#include "../Lexer/TokenTree.h"

void tab(int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << '\t';
}

struct token_tree_printer
{
    token_tree_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(TokenTree const& xml) const;

    int indent;
};

struct token_tree_node_printer : boost::static_visitor<>
{
    token_tree_node_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(TokenTree const& xml) const
    {
        token_tree_printer(indent + 1)(xml);
    }

    void operator()(std::string const& text) const
    {
        tab(indent+1);
        std::cout << "value: \"" << text << '"' << std::endl;
    }

    int indent;
};

void token_tree_printer::operator()(TokenTree const& xml) const
{
    tab(indent);
    // std::cout << "operation: " << xml.operation << std::endl;
    // std::cout << (xml.type == ExpressionType::Value ? "EXPRESSION" : (xml.type == ExpressionType::Curl ? "Curl:" : "List:")) << std::endl;
    tab(indent);
    std::cout << '{' << std::endl;

    BOOST_FOREACH(TokenNode const& node, xml.children)
    {
        boost::apply_visitor(token_tree_node_printer(indent), node);
    }

    tab(indent);
    std::cout << '}' << std::endl;
}

struct IncorrectTokenException : CustomException{
    explicit IncorrectTokenException(const TokenTree& token_tree, std::string exception_text)
            : CustomException(exception_text){
        tree = token_tree;
        text = std::move(exception_text);
    }
    std::basic_string<char> text;
    TokenTree tree;
    [[nodiscard]] const char * what() const noexcept override{
        token_tree_printer printer;
        printer(tree);
        return text.c_str();
    }
};

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_INCORRECTTOKENEXCEPTION_H
