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
    void print();
    std::vector<TokenNode> children;

};


BOOST_FUSION_ADAPT_STRUCT(
        TokenTree,
        (std::vector<TokenNode>, children)
)




#endif //BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H
