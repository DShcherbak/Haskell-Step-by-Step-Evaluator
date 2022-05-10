#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H

#include <string>
#include <memory>
#include "../../Parsing/PartiallyParsedString.h"

namespace function {

    class PartiallyParsedString;

    enum class TemplateType{Skip,
                    Any,
                    IntValue,
                    FloatValue,
                    CharValue,
                    StringValue,
                    EmptyList,
                    Tuple,
                    TupleConstructor,
                    List,
                    ListConstructor,
                    DataConstructor,
                    BrokenType};

    class MaskTemplate {
    public:
        ~MaskTemplate()=default;
        TemplateType type;
        std::string template_body;
        std::shared_ptr<MaskTemplate> First;
        explicit MaskTemplate(const std::string &template_string);
        explicit MaskTemplate(const PartiallyParsedString& template_string);
        explicit MaskTemplate();

    private:
        static bool check_skip_symbol(const std::string& template_string, function::TemplateType& type);
        static bool check_empty_list(const std::string& template_string, function::TemplateType& type);
        static bool check_char_value(const std::string& template_string, function::TemplateType& type);
        static bool check_string_value(const std::string& template_string, function::TemplateType& type);
        static bool check_num_value(const std::string& template_string, function::TemplateType& type);
        static bool check_any_value(const std::string& template_string, function::TemplateType& type);
        bool check_non_empty_list(const std::string& template_string, function::TemplateType& type);
        bool check_tuple(const std::string& template_string, function::TemplateType& type);
        bool check_complex_value(const std::string& template_string, function::TemplateType& type);

    };

    class ComplexMaskTemplate : public MaskTemplate {
    public:
        std::shared_ptr<ComplexMaskTemplate> Rest;
        explicit ComplexMaskTemplate(const std::string &template_string);
        explicit ComplexMaskTemplate(const PartiallyParsedString& template_string);
        ComplexMaskTemplate()=default;
    };
}

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H
