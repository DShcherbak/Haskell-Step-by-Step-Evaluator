#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H

#include <memory>
#include <vector>
#include "Guard.h"
#include "Body.h"
#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"
#include "../../Parsing/PartiallyParsedString.h"

namespace function {
    class Mask {
    private:
        std::vector<std::shared_ptr<function::MaskTemplate>> templates;
        std::vector<std::shared_ptr<function::Guard>> guards;
        std::shared_ptr<function::Body> body;
        bool has_guards = false;
    public:

        explicit Mask(const std::string& mask_string);

        void add_guard(const std::string& guard_head, const std::string& guard_body);
        void add_body(const std::string& body_string);

    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
