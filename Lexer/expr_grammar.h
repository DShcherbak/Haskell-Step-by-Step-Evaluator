#ifndef BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H
#define BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include "TokenTree.h"

namespace qi = boost::spirit::qi;
using boost::spirit::ascii::space_type;

namespace parser {

    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;


    template <typename Iterator>
    struct simple_expression_grammar
            : qi::grammar<Iterator, TokenTree(), qi::locals<std::string>> {
        simple_expression_grammar()
                : simple_expression_grammar::base_type(expression) {
            using qi::lit;
            using phoenix::push_back;
            using qi::lexeme;
            using qi::eps;
            using ascii::char_;
            using ascii::string;
            using namespace qi::labels;

            little = char_("a-z") | char_('_');
            Big = char_("A-Z");
            identifier = little >> *char_("_'a-zA-Z0-9");
            number = +char_("0-9") >> -(char_('.') >> +char_("0-9"));
            Data_identifier = Big >> *char_("_'a-zA-Z0-9");
            char_literal = lexeme['\'' >> ((char_('\\')[push_back(_val, _1)] >> char_[push_back(_val, _1)]) ||
                                           (char_ - char_('\''))[push_back(_val, _1)]) >> '\''];
            string_literal = lexeme['"' >> *(char_('\\')[push_back(_val, _1)] >> char_[push_back(_val, _1)] ||
                                             (char_ - char_('"'))[push_back(_val, _1)] ||
                                             (char_ - char_('\"'))[push_back(_val, _1)]) >> '"'];
            plain = string_literal[_val = '"' + _1 + '"'] | char_literal[_val = "'" + _1 + "'"] |
                    (Data_identifier | identifier | number)[_val = _1]; //[_val = "'" + _1 + "'"]
            operator_char = char_("+,:*^$|/=.`<>;") | //TODO: every operator (Look up from Haskell Report)
                            char_('-');//"+" | lit('-') | lit('/') | lit(':') | lit('*') | lit('^');  //=
            operator_string = +operator_char;
            list = '[' >> -(just_plain_iders)[_val = _1] >> ']' >> eps[push_back(at_c<0>(_val), "[]")];
            curly = '(' >> -(just_plain_iders)[_val = _1] >> ')' >> eps[push_back(at_c<0>(_val), "()")];
            silly = '{' >> -(just_plain_iders)[_val = _1] >> '}' >> eps[push_back(at_c<0>(_val), "{}")];
            node = list
                    | curly
                    | silly
                    | operator_string | plain;
            just_plain_iders = *(char_(' ')) >> node[push_back(at_c<0>(_val), _1)]
                                             >> *(*(char_(' ')) >> node[push_back(at_c<0>(_val), _1)]) >> *(char_(' '));
            //     csvs = node[push_back(at_c<0>(_val), _1)] % ',';
            expression = just_plain_iders; //;csvs ;

        }

        qi::rule<Iterator, TokenTree(), qi::locals<std::string>> expression;
        qi::rule<Iterator, TokenTree(), qi::locals<std::string>> just_plain_iders;
        qi::rule<Iterator, TokenTree(), qi::locals<std::string>> list;
        qi::rule<Iterator, TokenTree(), qi::locals<std::string>> curly;
        qi::rule<Iterator, TokenTree(), qi::locals<std::string>> silly;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> plain;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> operator_string;
        qi::rule<Iterator, char(), qi::locals<std::string>> operator_char;

        qi::rule<Iterator, TokenNode(), qi::locals<std::string>> node;
        qi::rule<Iterator, char(), qi::locals<std::string>> little;
        qi::rule<Iterator, char(), qi::locals<std::string>> Big;

        qi::rule<Iterator, std::string(), qi::locals<std::string>> identifier;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> number;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> Data_identifier;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> string_literal;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> char_literal;
    };
}
#endif //BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H
