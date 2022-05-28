#include "Lexer.h"

int const tabsize = 4;

void tab(int indent)
{
    for (int i = 0; i < indent; ++i)
        std::cout << ' ';
}

struct mini_xml_printer
{
    mini_xml_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(expression_tree const& xml) const;

    int indent;
};

struct mini_xml_node_printer : boost::static_visitor<>
{
    mini_xml_node_printer(int indent = 0)
            : indent(indent)
    {
    }

    void operator()(expression_tree const& xml) const
    {
        mini_xml_printer(indent+tabsize)(xml);
    }

    void operator()(std::string const& text) const
    {
        tab(indent+tabsize);
        std::cout << "value: \"" << text << '"' << std::endl;
    }

    int indent;
};

void mini_xml_printer::operator()(expression_tree const& xml) const
{
    tab(indent);
   // std::cout << "operation: " << xml.operation << std::endl;
   // std::cout << (xml.type == ExpressionType::Value ? "EXPRESSION" : (xml.type == ExpressionType::Curl ? "Curl:" : "List:")) << std::endl;
    tab(indent);
    std::cout << '{' << std::endl;

    BOOST_FOREACH(expression_node const& node, xml.children)
                {
                    boost::apply_visitor(mini_xml_node_printer(indent), node);
                }

    tab(indent);
    std::cout << '}' << std::endl;
}

std::vector<expression_tree> Lexer::try_parse(const std::vector<std::string>& text) {
    namespace qi = boost::spirit::qi;
    typedef parser::simple_expression_grammar<std::string::const_iterator> expr_grammar;

    expr_grammar grammar;

    std::vector<expression_tree> result;

    //TODO: Change to exception if function not parsed

    for(auto line : text){
        if(line.empty())
            continue;
        expression_tree ast;// Our tree
        std::string::const_iterator iter = line.begin();
        std::string::const_iterator end = line.end();
        using boost::spirit::ascii::space;

        bool parsed = parse(iter, end, grammar, ast);
        if (parsed && iter == end){
            result.emplace_back(ast);
        } else if (parsed){
            std::cout << "Parsing: " << line << "\n";
            std::string::const_iterator some = iter + std::min(30, int(end - iter));
            std::string context(iter, (some>end)?end:some);
            std::cout << "-------------------------\n";
            std::cout << "Partial parsing\n";
            std::cout << "stopped at: \"" << context << "...\"\n";
            std::cout << "-------------------------\n";
            std::cout << "Result:\n";
            mini_xml_printer printer;
            printer(ast);
            break;
        }
        else
        {
            std::cout << "Parsing: " << line << "\n";
            std::string::const_iterator some = iter + std::min(30, int(end - iter));
            std::string context(iter, (some>end)?end:some);
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "stopped at: \"" << context << "...\"\n";
            std::cout << "-------------------------\n";
            break;
        }

    }
    return result;


}
