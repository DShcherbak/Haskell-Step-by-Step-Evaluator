#ifndef BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H
#define BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H

#include <boost/variant/recursive_wrapper.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant.hpp>
#include <string>

struct expression_tree;

typedef
boost::variant<boost::recursive_wrapper<expression_tree>, std::string>
        expression_node;

enum class ExpressionType {Value, List, Curl};
struct expression_tree
{
   // ExpressionType type = ExpressionType::Value;
    std::vector<expression_node> children;
};

BOOST_FUSION_ADAPT_STRUCT(
        expression_tree,
                (std::vector<expression_node>, children)
)


#endif //BOOST_OPERATOR_PARSER_EXPRESSION_TREE_H
