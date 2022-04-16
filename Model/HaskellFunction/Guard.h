#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_GUARD_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_GUARD_H

#include <string>

namespace function {
    class Guard {
    public:
        Guard(const std::string& head, const std::string& body);
    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_GUARD_H
