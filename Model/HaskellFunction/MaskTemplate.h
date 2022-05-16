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
                    EndList,
                    Tuple,
                    List,
                    DataConstructor,
                    BrokenType};

    class MaskTemplate {
    public:
        ~MaskTemplate()=default;
        TemplateType type;
        std::string template_body;
        std::shared_ptr<MaskTemplate> First;
        std::shared_ptr<MaskTemplate> Rest;
        explicit MaskTemplate(const std::string &template_string);
        explicit MaskTemplate(const PartiallyParsedString& template_string);
        explicit MaskTemplate();
        void count_body();
        void print_template(int depth = 0);

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
}

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H
