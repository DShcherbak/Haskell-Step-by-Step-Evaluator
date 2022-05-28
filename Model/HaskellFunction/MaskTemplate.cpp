#include "MaskTemplate.h"






function::MaskTemplate::MaskTemplate() {
    type = TemplateType::BrokenType;
}

bool function::MaskTemplate::check_skip_symbol(const std::string& template_string, function::TemplateType& type){
    if(template_string == "_"){
        type = function::TemplateType::Skip;
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_empty_list(const std::string& template_string, function::TemplateType& type){
    if(parsing::remove_spaces(template_string) == "[]"){
        type = function::TemplateType::EndList;
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_char_value(const std::string& template_string, function::TemplateType& type){
    if(template_string.starts_with('\'') && template_string.ends_with('\'')){
        type = function::TemplateType::CharValue;
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_string_value(const std::string& template_string, function::TemplateType& type){
    if(template_string.starts_with('\"') && template_string.ends_with('\"')){
        type = function::TemplateType::StringValue;
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_num_value(const std::string& template_string, function::TemplateType& type){
    if(std::isdigit(template_string[0])){
        if(template_string.find('.') != std::string::npos){
            type = function::TemplateType::FloatValue;
        } else {
            type = function::TemplateType::IntValue;
        }
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_any_value(const std::string& template_string, function::TemplateType& type){
    for(char c : template_string){
        if(!std::isalpha(c) || c == '_' || c == '\'')
            return false;
    }
    if(template_string[0] != '\''){
        type = function::TemplateType::Any;
        return true;
    }
    return false;
}

std::shared_ptr<function::MaskTemplate> make_list_template(std::vector<std::string> list_elements){
    return nullptr;
}

bool function::MaskTemplate::check_non_empty_list(const std::string& template_string, function::TemplateType& type){
    if(template_string.starts_with('[') && template_string.ends_with(']')){
        auto list_elements = parsing::split_by(template_string.substr(1, template_string.size()-2), ',');
        type = function::TemplateType::List;
        First = make_list_template(list_elements);
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_tuple(const std::string &template_string, function::TemplateType &type) {
    if(template_string.starts_with('[') && template_string.ends_with(']')){
        auto list_elements = parsing::split_by(template_string.substr(1, template_string.size()-2), ',');
        type = function::TemplateType::List;
        First = make_list_template(list_elements);
        return true;
    }
    return false;
}

bool function::MaskTemplate::check_complex_value(const std::string &template_string, function::TemplateType &type) {
    if(template_string.starts_with('[') && template_string.ends_with(']')){
        auto list_elements = parsing::split_by(template_string.substr(1, template_string.size()-2), ',');
        type = function::TemplateType::List;
        First = make_list_template(list_elements);
        return true;
    }
    return false;
}

std::string trim(std::string line){
    while(line.starts_with(' ')){
        line = line.substr(1);
    }
    while(line.ends_with(' ')){
        line.pop_back();
    }
    return line;
}

function::MaskTemplate::MaskTemplate(const std::string &template_string) {
    template_body = trim(template_string);

    if(check_skip_symbol(template_body, type)) return;
    if(check_empty_list(template_body, type)) return;
    if(check_char_value(template_body, type)) return;
    if(check_string_value(template_body, type)) return;
    if(check_num_value(template_body, type)) return;
    if(check_any_value(template_body, type)) return;
    type = TemplateType::BrokenType;
    throw CustomException("Unknown paramether type: " + template_string);
}

function::MaskTemplate::MaskTemplate(const PartiallyParsedString &template_string) : MaskTemplate{template_string.line}{

}

void function::MaskTemplate::count_body() {
    if(!template_body.empty())
        return;
    if(First != nullptr)
        First->count_body();
    if(Rest != nullptr)
        Rest->count_body();

    switch(type){
        case TemplateType::Skip:
            template_body = "_";
            break;
        case TemplateType::EndList:
            if(First == nullptr)
                template_body = "([])";
            else
                template_body = '(' + First->template_body + ')';
            break;
        case TemplateType::DataConstructor:
            template_body = First->template_body + (Rest != nullptr ? " " + Rest->template_body : "");
            break;
        case TemplateType::Tuple:
            if(Rest == nullptr){
                template_body = "(" + First->template_body + ")";
            } else {
                template_body = "(" + First->template_body + "," + Rest->template_body.substr(1);
            }
            break;
        case TemplateType::List:
            if(Rest == nullptr){
                template_body = "(" + First->template_body + " : [])";
            } else {
                auto rest_bod = Rest->template_body;
                //if(Rest->First != nullptr)
                rest_bod = rest_bod.substr(1);
                template_body = "(" + First->template_body + ":" + rest_bod;
            }
            break;
        case TemplateType::BrokenType:
            template_body = "BROKEN TYPE";
            break;
        default:
            return;
    }
}

void function::MaskTemplate::print_template(int depth) {
    for(int i = 0; i < depth; i++){
        std::cout << "|\t";
    }
    std::cout << template_body << std::endl;
    if(First != nullptr) First->print_template(depth+1);
    if(Rest != nullptr) Rest->print_template(depth+1);
}


