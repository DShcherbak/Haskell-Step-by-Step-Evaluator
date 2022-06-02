#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H

#include <memory>
#include <vector>
#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"
#include "../../Parsing/PartiallyParsedString.h"
#include "../../HAST/HastMaskNode.h"

namespace function {
    class Mask {
    private:
        std::vector<HAST_N> argument_templates;
        std::shared_ptr<HastNode> body_node;
        std::vector<std::pair<HAST_N, HAST_N>> guards;
    public:
        std::shared_ptr<HastNode> apply_args(const std::vector<HAST_N>& arguments);
        void add_body(const HAST_N& body);
        void add_guards(const std::vector<std::pair<HAST_N, HAST_N>>& guards);
        void add_arg_templates(const std::vector<HAST_N>& arg_tempaltes);


    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
