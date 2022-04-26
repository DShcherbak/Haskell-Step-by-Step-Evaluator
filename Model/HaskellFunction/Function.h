#ifndef HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTION_H
#define HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTION_H

#include <string>
#include <vector>
#include <memory>
#include "Mask.h"

using std::string;

namespace function {

    class Function {
    public:
        string type_definition;
        string type_definition_string;
        string name;

        explicit Function(const std::string &new_name);

        void add_mask(std::shared_ptr<function::Mask> mask);


    private:
        std::vector<std::shared_ptr<Mask>> masks;
        std::size_t number_of_params{};

    };
}


#endif //HASKELL_STEP_BY_STEP_EVALUATOR_FUNCTION_H
