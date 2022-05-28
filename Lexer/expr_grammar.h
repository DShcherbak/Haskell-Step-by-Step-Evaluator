#ifndef BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H
#define BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include "expression_tree.h"

namespace qi = boost::spirit::qi;
using boost::spirit::ascii::space_type;

namespace parser {

    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;


    template <typename Iterator>
    struct mini_xml_grammar
            : qi::grammar<Iterator, expression_tree(), qi::locals<std::string>> {
        mini_xml_grammar()
                : mini_xml_grammar::base_type(expression) {
            using qi::lit;
            using phoenix::push_back;
            using qi::lexeme;
            using ascii::char_;
            using ascii::string;
            using namespace qi::labels;


            //TODO: comments
            //TODO: char and string literals
            //TODO: combine lines (think about multiple wheres)
            //TODO: infix operators

            little = char_("a-z") | char_('_');
            Big = char_("A-Z");
            identifier = little >> *char_("_'a-zA-Z0-9");
            number = little >> +char_("0-9") >> -(char_('.') >> +char_("0-9"));
            Data_identifier = Big >> *char_("_'a-zA-Z0-9");
            char_literal = lexeme['\'' >> ((char_('\\')[push_back(_val, _1)] >> char_[push_back(_val, _1)]) ||
                                           (char_ - char_('\''))[push_back(_val, _1)]) >> '\''];
            string_literal = lexeme['"' >> *(char_('\\')[push_back(_val, _1)] >> char_[push_back(_val, _1)] ||
                                             (char_ - char_('"'))[push_back(_val, _1)] ||
                                             (char_ - char_('\"'))[push_back(_val, _1)]) >> '"'];
            plain = string_literal[_val = '"' + _1 + '"'] | char_literal[_val = "'" + _1 + "'"] |
                    (Data_identifier | identifier | number)[_val = _1]; //[_val = "'" + _1 + "'"]
            operator_char = char_("+,:*^$/=.`<>;") |
                            char_('-');//"+" | lit('-') | lit('/') | lit(':') | lit('*') | lit('^');  //=
            operator_string = +operator_char;
            list = '[' >> -(just_plain_iders)[_val = _1] >> ']';
            curly = '(' >> -(just_plain_iders)[_val = _1] >> ')';
            silly = '{' >> -(just_plain_iders)[_val = _1] >> '}';
            node = list | curly | silly | operator_string | plain;
            just_plain_iders = *(char_(' ')) >> node[push_back(at_c<0>(_val), _1)]
                                             >> *(*(char_(' ')) >> node[push_back(at_c<0>(_val), _1)]) >> *(char_(' '));
            //     csvs = node[push_back(at_c<0>(_val), _1)] % ',';
            expression = just_plain_iders; //;csvs ;

        }

        qi::rule<Iterator, expression_tree(), qi::locals<std::string>> expression;
        qi::rule<Iterator, expression_tree(), qi::locals<std::string>> just_plain_iders;
        qi::rule<Iterator, expression_tree(), qi::locals<std::string>> list;
        qi::rule<Iterator, expression_tree(), qi::locals<std::string>> curly;
        qi::rule<Iterator, expression_tree(), qi::locals<std::string>> silly;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> plain;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> operator_string;
        qi::rule<Iterator, char(), qi::locals<std::string>> operator_char;

        qi::rule<Iterator, expression_node(), qi::locals<std::string>> node;
        qi::rule<Iterator, char(), qi::locals<std::string>> little;
        qi::rule<Iterator, char(), qi::locals<std::string>> Big;

        qi::rule<Iterator, std::string(), qi::locals<std::string>> identifier;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> number;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> Data_identifier;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> string_literal;
        qi::rule<Iterator, std::string(), qi::locals<std::string>> char_literal;
    };
}

/*



            auto non_operand = qi::char_ - '+' - '*' - '/' - '-' - '^' - '(' - ')';
            value = +(non_operand);//qi::int_[_val = _1];//lexeme[+(char_ - '<')];
            node %= ('(' >> power >> ')') | ('(' >> multiplication >> ')') | ('(' >> subtraction >> ')') |
                    ('(' >> addition >> ')') | value;//[_val = _1];//('(' >> addition[_val = _1] >> ')') || ; //
            //  node %=  ('(' >> addition >> ')') | value;//[_val = _1];//('(' >> addition[_val = _1] >> ')') || ; //
            addition = qi::eps[phoenix::clear(at_c<0>(_val))] >>
                                                              node[push_back(at_c<0>(_val), _1)]
                                                              >> +("+" >> node[push_back(at_c<0>(_val), _1)]);
            subtraction = qi::eps[phoenix::clear(at_c<0>(_val))] >>
                                                                 node[push_back(at_c<0>(_val), _1)]
                                                                 >> +("-" >> node[push_back(at_c<0>(_val), _1)]);
            multiplication = qi::eps[phoenix::clear(at_c<0>(_val))] >>
                                                                    node[push_back(at_c<0>(_val), _1)]
                                                                    >> +("*" >> node[push_back(at_c<0>(_val), _1)]);
            power = qi::eps[phoenix::clear(at_c<0>(_val))] >>
                                                           node[push_back(at_c<0>(_val), _1)]
                                                           >> +('^' >> node[push_back(at_c<0>(_val), _1)]);

          std::vector<qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type>> level6 = {addition, subtraction};
            std::vector<qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type>> level7 = {power, multiplication};

            qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> genericLevel7rule = power;
            qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> newGenericLevel7rule = power;
            qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> newGenericLevel6rule = addition;
            for(auto lev7 : level7){
                genericLevel7rule = newGenericLevel7rule.copy();
                newGenericLevel7rule = genericLevel7rule.copy() | lev7.copy();
            }


            qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> genericLevel6rule = addition;
            for(auto lev6 : level6){
                genericLevel6rule = genericLevel6rule.copy() | lev6.copy();
            }
 expression = genericLevel7rule | genericLevel6rule;
//     expression = power | multiplication | addition | subtraction;// | genericLevel6rule;


  qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> expression;
       qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> power;
       qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> multiplication;
       qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> addition;
       qi::rule<Iterator, expression_tree(), qi::locals<std::string>, ascii::space_type> subtraction;
       qi::rule<Iterator, expression_node(), ascii::space_type> node;
       qi::rule<Iterator, std::string(), ascii::space_type> value;*/

/*
 * xml =
                    start_tag                   [at_c<0>(_val) = _1]
                            >>  *node                       [push_back(at_c<0>(_val), _1)]
                            >>  end_tag(at_c<0>(_val))
                    ;


  start_tag %=
                    '<'
                            >>  !lit('/')
                            >>  lexeme[+(char_ - '>')]
                            >>  '>'
                    ;

            end_tag =
                    "</"
                            >>  lit(_r1)
                            >>  '>'
                    ;*/


#endif //BOOST_OPERATOR_PARSER_EXPR_GRAMMAR_H
