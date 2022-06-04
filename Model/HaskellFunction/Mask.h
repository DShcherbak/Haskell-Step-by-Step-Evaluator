#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H

#include <memory>
#include <vector>
#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"
#include "../../Parsing/PartiallyParsedString.h"
#include "../../HAST/HastMaskNode.h"
#include "../HaskellModel.h"

namespace function {
    class Mask {
    private:
        std::vector<std::shared_ptr<HastMaskNode>> argument_templates;
        std::shared_ptr<HastNode> body_node;
        std::vector<std::pair<HAST_N, HAST_N>> guards;
    public:
        std::shared_ptr<HastNode> apply_args(const std::vector<HAST_N>& arguments);
        void add_body(HaskellModel& model, const std::vector<TokenNode> &body);
        void add_guards(HaskellModel& model, const std::vector<std::pair<std::vector<TokenNode>, std::vector<TokenNode>>> &guards);
        void add_arg_templates(const std::vector<std::shared_ptr<HastMaskNode>> &arg_templates);


    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_MASK_H
