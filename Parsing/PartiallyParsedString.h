#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H


#include <vector>
#include <string>
#include <memory>
#include <set>
#include "../Model/HaskellFunction/MaskTemplate.h"


/*
 * TODO: Rename classes, maybe restructure
 * TODO: Data structure parsing
 * TODO: Parsing input into tree
 * TODO: Do I need type checking using GHCI or can I just go on using my raw tree system
 *
 * !! Maybe save the type for a mask template tree?
 * !! Problem of 2 substitudes - no idea what it is
 * !! May need "incomplete data constructor" template type - but seems to work ok for now
 */
namespace function {

    class MaskTemplate;

    class PartiallyParsedString {

        std::string operator_characters = ":!#$%&⋆+./<=>?@|\\^|-~,";


    public:
        std::set<char> operators;
        char parenthesized = '\0';
        PartiallyParsedString();
        explicit PartiallyParsedString(const std::string &line);
        std::string line;
        std::vector<std::shared_ptr<MaskTemplate>> replacements;

        static std::string preprocess_string(const std::string &line);

        std::shared_ptr<function::MaskTemplate> get_resulting_template();

        static std::shared_ptr<function::MaskTemplate> parse_line(PartiallyParsedString &line);


        static std::vector<PartiallyParsedString> split_by_space(std::string line);

    private:
        static PartiallyParsedString parse_internal_brackets(const PartiallyParsedString &line);

        static PartiallyParsedString parse_sugared_lists(const PartiallyParsedString &line);

        static std::shared_ptr<function::MaskTemplate> parse_constructor_operators(PartiallyParsedString parsedString);

        static std::shared_ptr<function::MaskTemplate> parse_named_constructors(PartiallyParsedString line);

        static PartiallyParsedString parse_any_brackets(PartiallyParsedString parsedLine, char open, char close);

        static std::vector<std::shared_ptr<function::MaskTemplate>>
        construct_templates(std::vector<PartiallyParsedString> vector1);

        PartiallyParsedString substr(int start, int finish);

        static std::shared_ptr<MaskTemplate> parse_desugared_list(PartiallyParsedString parsedString);

        static std::string postprocess_string(const std::string &line);
    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H
