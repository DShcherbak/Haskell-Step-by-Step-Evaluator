#include "Mask.h"

function::Mask::Mask(const std::string& mask_string) {
    using std::string, std::vector;
    size_t current = mask_string.find(' '), number_of_arguments = 0, bracket_depth = 0, len = mask_string.size();
    string name = mask_string.substr(0, current);
    string mask_element;
    current++;

    while(current < len){
        if(mask_string[current] != ' ' && mask_string[current] != '='){
            mask_element = "";
            while(bracket_depth > 0 || mask_string[current] != ' '){
                mask_element += mask_string[current++];
            }
            if(!mask_element.empty()){
                templates.emplace_back(std::make_shared<function::MaskTemplate>(mask_element));
            }
        } else {
            current++;
        }
    }





}

void function::Mask::add_guard(const std::string &guard_head, const std::string &guard_body) {
    has_guards = true;
    guards.emplace_back(std::make_shared<function::Guard>(guard_head, guard_body));
}

void function::Mask::add_body(const std::string &body_string) {
    body = std::make_shared<function::Body>(body_string);
}
