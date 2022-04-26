#include "Function.h"

function::Function::Function(const string &new_name) {
    name = new_name;
}

void function::Function::add_mask(std::shared_ptr<Mask> mask) {
    masks.push_back(std::move(mask));
}
