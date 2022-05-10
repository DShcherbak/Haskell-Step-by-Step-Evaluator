#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H


#include <vector>
#include <string>
#include <memory>
#include <set>
#include "../Model/HaskellFunction/MaskTemplate.h"


/*
 * TODO: Finish complex template parsing
 * TODO: Infix data constructor operators!
 * TODO: Template string value
 * TODO: Remove 'complexList' and 'ComplexTuple' types
 * TODO: Rename classes, maybe restructure
 */
namespace function {

    class MaskTemplate;

    class PartiallyParsedString {

        std::string operator_characters = ":!#$%&⋆+./<=>?@|\\^|-~,";


    public:
        std::set<char> operators;
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

        static PartiallyParsedString parse_named_constructors(const PartiallyParsedString &line);

        static PartiallyParsedString parse_any_brackets(PartiallyParsedString parsedLine, char open, char close);

        static std::vector<std::shared_ptr<function::MaskTemplate>>
        construct_templates(std::vector<PartiallyParsedString> vector1);

        PartiallyParsedString substr(int start, int finish);

        static std::shared_ptr<MaskTemplate> parse_desugared_list(PartiallyParsedString parsedString);
    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_PARTIALLYPARSEDSTRING_H
