#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H


#include <memory>
#include "../HastNode.h"

class HastPrinter {
public:
    static void print_node(const std::shared_ptr<HastNode>& node, int depth = 0);
};


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_HASTPRINTER_H
