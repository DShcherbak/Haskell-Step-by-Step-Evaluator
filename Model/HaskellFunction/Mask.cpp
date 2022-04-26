#include "Mask.h"
#include "../../Parsing/parsing.h"


//1 =1
// 1 8 = 9 = 3
// 1 8 27 = 36 = 6
// 1 8 27 64 = 100 = 10

//sum(i) = ((i+1)*i/2)^2 - ((i-1)*i/2)^2 = (i^2 + i)^2 - (i^2 - i)^2 /4 =
//        = i^4 + 2i^3 + i^2 - i^4 + 2i^3 - i^2 = 4i^3 / 4 = i^3

// f a b (c:(d:e)) with no '=' at the end
function::Mask::Mask(const std::string& mask_string) {
    using std::string, std::vector;

    std::vector<std::string> line_elements = parsing::split_by_space(mask_string);
    for(auto& line_element : line_elements){
        templates.emplace_back(std::make_shared<MaskTemplate>(line_element));
    }

}

void function::Mask::add_guard(const std::string &guard_head, const std::string &guard_body) {
    has_guards = true;
    guards.emplace_back(std::make_shared<function::Guard>(guard_head, guard_body));
}

void function::Mask::add_body(const std::string &body_string) {
    body = std::make_shared<function::Body>(body_string);
}
