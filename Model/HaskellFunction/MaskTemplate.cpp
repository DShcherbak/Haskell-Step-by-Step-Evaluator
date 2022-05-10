#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"
#include <iostream>
#include <cassert>

struct CustomException : public std::exception{

    explicit CustomException(std::string exception_text){
        text = std::move(exception_text);
    }
    std::basic_string<char> text;
    [[nodiscard]] const char * what() const noexcept override{
        return text.c_str();
    }
};



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
        type = function::TemplateType::EmptyList;
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
    return template_string[0] != '\'';
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


function::ComplexMaskTemplate::ComplexMaskTemplate(const std::string &template_string) : MaskTemplate(template_string) {

}

function::ComplexMaskTemplate::ComplexMaskTemplate(const PartiallyParsedString &template_string) : MaskTemplate(template_string) {

}

