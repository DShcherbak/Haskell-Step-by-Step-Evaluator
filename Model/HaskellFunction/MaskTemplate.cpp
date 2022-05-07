#include "MaskTemplate.h"
#include "../../Parsing/parsing.h"
#include <iostream>

bool check_simple_string(const std::string& template_string, function::TemplateType& type);
bool check_empty_list(const std::string& template_string, function::TemplateType& type);
bool check_char_value(const std::string& template_string, function::TemplateType& type);
bool check_string_value(const std::string& template_string, function::TemplateType& type);
bool check_num_value(const std::string& template_string, function::TemplateType& type);

struct CustomException : public std::exception{

    explicit CustomException(std::string exception_text){
        text = std::move(exception_text);
    }
    std::basic_string<char> text;
    [[nodiscard]] const char * what() const noexcept override{
        return text.c_str();
    }
};


function::MaskTemplate::MaskTemplate(std::string &template_string) {
    template_body = template_string;

    if(check_simple_string(template_string, type)) return;
    if(check_empty_list(template_string, type)) return;
    if(check_char_value(template_string, type)) return;
    if(check_string_value(template_string, type)) return;
    if(check_num_value(template_string, type)) return;


    if (template_string.find(':') != std::string::npos) {

    }
    type = TemplateType::BrokenType;
    //throw CustomException("Unknown paramether type: " + template_string);
}

bool check_simple_string(const std::string& template_string, function::TemplateType& type){
    if(template_string == "_"){
        type = function::TemplateType::Skip;
        return true;
    }
    return false;
}

bool check_empty_list(const std::string& template_string, function::TemplateType& type){
    if(parsing::remove_spaces(template_string) == "[]"){
        type = function::TemplateType::EmptyList;
        return true;
    }
    return false;
}

bool check_char_value(const std::string& template_string, function::TemplateType& type){
    if(template_string.starts_with('\'') && template_string.ends_with('\'')){
        type = function::TemplateType::CharValue;
        return true;
    }
    return false;
}

bool check_string_value(const std::string& template_string, function::TemplateType& type){
    if(template_string.starts_with('\"') && template_string.ends_with('\"')){
        type = function::TemplateType::StringValue;
        return true;
    }
    return false;
}

bool check_num_value(const std::string& template_string, function::TemplateType& type){
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








