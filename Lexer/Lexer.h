#ifndef BOOST_OPERATOR_PARSER_EXPRESSION_PARSER_H
#define BOOST_OPERATOR_PARSER_EXPRESSION_PARSER_H

#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include "TokenTree.h"
#include "expr_grammar.h"

#include <string>

class Lexer {
public:
    static std::vector<TokenTree> functions_to_tokens(const std::vector<std::string>& text);

    static std::vector<TokenNode> functions_to_nodes(const std::vector<std::string> &functions);
};


#endif //BOOST_OPERATOR_PARSER_EXPRESSION_PARSER_H
