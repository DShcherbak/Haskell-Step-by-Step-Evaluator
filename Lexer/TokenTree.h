#ifndef BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H
#define BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <string>

class TokenTree;

typedef
boost::variant<boost::recursive_wrapper<TokenTree>, std::string>
        TokenNode;



class TokenTree
{
public:
    void print() const;
    std::vector<TokenNode> children;

};


void tab(int indent);


struct token_tree_printer
{
    explicit token_tree_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(TokenTree const& xml) const;

    int indent;
};

struct token_tree_node_printer : boost::static_visitor<>
{
    explicit token_tree_node_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(TokenTree const& xml) const;
    void operator()(std::string const& text) const;

    int indent;
};



BOOST_FUSION_ADAPT_STRUCT(
        TokenTree,
        (std::vector<TokenNode>, children)
)





#endif //BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H
