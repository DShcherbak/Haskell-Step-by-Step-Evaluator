#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"


function::MaskTemplate::MaskTemplate(std::string &template_string) {
    template_body = template_string;
    if(template_string == "_"){
        type = TemplateType::Skip;
        return;
    }

    if(parsing::remove_spaces(template_string) == "[]"){
        type = TemplateType::EmptyList;
    }

    if(template_string.starts_with('\'') && template_string.ends_with('\'')){
        type = TemplateType::CharValue;
    }

    if(template_string.starts_with('\"') && template_string.ends_with('\"')){
        type = TemplateType::StringValue;
    }

    if (template_string.find(':') < template_string.size() || template_string.starts_with('[')) {
        //something hard
    } else {
        char first = template_string[0];
        if (std::isdigit(first)) {
            type = TemplateType::IntValue;
            return;
        }
        if (first == '('){

        }
    }
}

bool function::MaskTemplate::fits_empty_template(){

}
