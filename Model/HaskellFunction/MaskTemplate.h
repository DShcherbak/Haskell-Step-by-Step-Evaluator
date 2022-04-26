#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H

#include <string>

namespace function {

    enum class TemplateType{Skip,
                    Any,
                    IntValue,
                    CharValue,
                    StringValue,
                    EmptyList,
                    TupleConstructor,
                    ListConstructor,
                    DataConstructor};

    class MaskTemplate {
    public:
        TemplateType type;
        std::string template_body;
        explicit MaskTemplate(std::string& template_string);
    };
}

#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASKTEMPLATE_H
