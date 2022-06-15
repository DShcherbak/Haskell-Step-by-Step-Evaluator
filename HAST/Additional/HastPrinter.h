#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H


#include <memory>
#include "../HastNode.h"
#include "../HastFunctionCallNode.h"

class HastPrinter {
public:
    static void print_node(const std::shared_ptr<HastNode>& node, int depth = 0);
    static std::string node_to_string(const std::shared_ptr<HastNode>& node);
    static std::string node_to_string(const std::shared_ptr<HastFunctionCallNode>& node);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H
