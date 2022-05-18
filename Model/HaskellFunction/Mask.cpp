#include <iostream>
#include "Mask.h"


// f a b ('c':('d':"e")) with no '=' at the end



function::Mask::Mask(const std::string& mask_string) {
    using std::string, std::vector;

    auto line_elements = PartiallyParsedString::split_by_space(mask_string);
    auto good_lines = std::vector<PartiallyParsedString>();
    for(int i = 1, n = (int) line_elements.size(); i < n; i++){
        if(!line_elements[i].line.empty())
            good_lines.push_back(line_elements[i]);
    }
    for(auto& line_element : good_lines){
        std::cout << mask_string << std::endl;
        auto line_template = PartiallyParsedString::parse_line(line_element);
        line_template->count_body();
        templates.emplace_back(line_template);
    }
    if(!templates.empty()){
        for(auto temp : templates){
            temp->print_template();
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
