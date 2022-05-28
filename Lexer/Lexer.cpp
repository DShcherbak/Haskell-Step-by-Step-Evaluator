#include "Lexer.h"
#include "../Exception/IncorrectTokenException.h"

int const tabsize = 4;


std::vector<TokenTree> Lexer::functions_to_tokens(const std::vector<std::string>& text) {
    namespace qi = boost::spirit::qi;
    typedef parser::simple_expression_grammar<std::string::const_iterator> expr_grammar;

    expr_grammar grammar;
    std::vector<TokenTree> result;

    for(auto line : text){
        if(line.empty())
            continue;
        TokenTree ast;

        std::string::const_iterator iter = line.begin();
        std::string::const_iterator end = line.end();
        using boost::spirit::ascii::space;

        bool parsed = parse(iter, end, grammar, ast);
        if (parsed && iter == end)
            result.emplace_back(ast);
        else if (parsed)
            throw IncorrectTokenException(ast, "Partial parsing error");
        else
            throw CustomException("Parsing error");

    }
    return result;
}
