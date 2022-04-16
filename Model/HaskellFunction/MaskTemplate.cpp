#include "MaskTemplate.h"

function::MaskTemplate::MaskTemplate(std::string &template_string) {
    template_body = template_string;
    if(template_string == "_"){
        type = Skip;
        return;
    }

    while (template_string.starts_with("(") && template_string.ends_with(")")) {
        template_string = template_string.substr(1, template_string.size() - 2);
    }

    if (template_string.find(':') < template_string.size() || template_string.starts_with('[')) {
        //something hard
    } else {
        char first = template_string[0];
        if (std::isdigit(first)) {
            type = IntValue;
            return;
        }
        if (first == '('){

        }
    }
}
