#include <iostream>
#include "PartiallyParsedString.h"

namespace function {

    PartiallyParsedString::PartiallyParsedString() {
        operators = std::set<char>();
        for (char c: operator_characters)
            operators.insert(c);
    }

    std::string PartiallyParsedString::preprocess_string(const std::string &line) {
        std::string result;
        for (char c: line) {
            result += c;
            if (c == '$') {
                result += c;
            }
        }
        return result;
    }

    std::vector<PartiallyParsedString> PartiallyParsedString::split_by_space(std::string line) {
        line = preprocess_string(line);
        std::vector<PartiallyParsedString> result{};

        enum MaskParserState {
            Default, NewWord, SingleQuotes, DoubleQuotes
        };
        MaskParserState state;

        PartiallyParsedString line_element{};
        std::string substitude;
        int bracket_depth = 0;

        for (char current_symbol: line) {
            bool subst = false;

            switch (state) {
                case Default:
                    if (bracket_depth == 0 && current_symbol == ' ') {
                        if (!line_element.line.empty())
                            result.push_back(line_element);
                        line_element = PartiallyParsedString{};
                        state = NewWord;
                    } else {
                        switch (current_symbol) {
                            case '"':
                                subst = true;
                                state = DoubleQuotes;
                                break;
                            case '(':
                                bracket_depth++;
                                break;
                            case ')':
                                bracket_depth--;
                                break;
                            default:
                                if (line_element.operators.contains(current_symbol))
                                    state = NewWord;
                        }
                    }
                    break;
                case NewWord:
                    switch (current_symbol) {
                        case '"':
                            subst = true;
                            state = DoubleQuotes;
                            break;
                        case '\'':
                            subst = true;
                            state = SingleQuotes;
                            break;
                        case '(':
                            bracket_depth++;
                            break;
                        case ')':
                            bracket_depth--;
                            break;
                        default:
                            if (line_element.operators.contains(current_symbol))
                                state = NewWord;
                    }
                    break;
                case DoubleQuotes:
                    if (current_symbol == '"') {
                        line_element.line += '$';
                        substitude += current_symbol;
                        line_element.replacements.emplace_back(std::make_shared<function::MaskTemplate>(substitude));
                        substitude = "";
                        state = Default;
                        continue;
                    }
                    subst = true;
                    break;
                case SingleQuotes:
                    if (current_symbol == '\'') {
                        line_element.line += '$';
                        substitude += current_symbol;
                        line_element.replacements.emplace_back(std::make_shared<function::MaskTemplate>(substitude));
                        substitude = "";
                        state = Default;
                        continue;
                    }
                    subst = true;
                    break;
            }
            if (subst)
                substitude += current_symbol;
            else
                line_element.line += current_symbol;
        }

        if (!line_element.line.empty())
            result.push_back(line_element);
        return result;
    }

    std::shared_ptr<function::MaskTemplate> PartiallyParsedString::get_resulting_template() {
        return replacements[0];
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

    std::vector<PartiallyParsedString> divide_pps_by(const PartiallyParsedString &parsingLine, char separator){
        std::vector<PartiallyParsedString> result;
        PartiallyParsedString current{};
        std::string line = parsingLine.line;
        int n = line.size();
        int i = 0, j = 0;
        if(line.starts_with('(') && line.ends_with(')') || line.starts_with('[') && line.ends_with(']'))
            i++, n--;
        while(i < n){
            if(line[i] == separator) {
                result.push_back(current);
                current = PartiallyParsedString{};
            }else {
                if(line[i] == '$') {
                    if (i == n - 1 || line[i + 1] != '$') {
                        current.replacements.push_back(parsingLine.replacements[j++]);
                    } else {
                        i++;
                        current.line += line[i];
                    }
                }
                current.line += line[i];
            }
            i++;
        }
        result.push_back(current);
        return result;
    }

    std::shared_ptr<function::MaskTemplate> construct_iterative(std::vector<std::shared_ptr<function::MaskTemplate>> list_elements, function::TemplateType type){
        size_t i = 1, n = list_elements.size();
        if(n == 0){
            return nullptr;
        }

        auto result = std::make_shared<function::ComplexMaskTemplate>();
        result->First = list_elements[0];
        result->type = type == TemplateType::List ? function::TemplateType::ListConstructor : function::TemplateType::TupleConstructor;
        auto rest = (std::shared_ptr<function::ComplexMaskTemplate>) result;

        while(i < n){
            std::shared_ptr<function::ComplexMaskTemplate> new_rest = std::make_shared<function::ComplexMaskTemplate>();
            rest->Rest = new_rest;
            rest = new_rest;
            rest->type = result->type;
            rest->First = list_elements[i];
            i++;
        }
        return result;
    }

    std::shared_ptr<function::MaskTemplate> PartiallyParsedString::parse_desugared_list(PartiallyParsedString parsedString) {
        std::string line = parsedString.line;
        int start = line.find(':'), prev_start = 0;
        if(start == std::string::npos)
            return nullptr;
        std::vector<PartiallyParsedString> list_elements{};


        while(start != std::string::npos){
            if(start != line.size()-1 && !parsedString.operators.contains(line[start+1])){
                list_elements.push_back(parsedString.substr(prev_start, start));
            }
            prev_start = start;
            start = line.find(':', start);
        }
        if(list_elements.empty()) {
            return nullptr;
        }else{
            list_elements.push_back(parsedString.substr(prev_start, line.size()));
        }

        auto result = std::make_shared<function::ComplexMaskTemplate>();
        result->type = TemplateType::List;
        auto constructed = construct_templates(list_elements);
        result->First = construct_iterative(constructed, result->type);
        auto current = result;
        while(current->Rest != nullptr){
            current = current->Rest;
        }
        current->First->type = TemplateType::List; // Так точно має бути але я хз чи це спрацює
        return result;


        return result;
    }

    std::shared_ptr<function::MaskTemplate> PartiallyParsedString::parse_line(PartiallyParsedString &line) {
        line.line = trim(line.line);
        std::cout << line.line << std::endl;
        line = parse_internal_brackets(line);
        line = parse_sugared_lists(line);
        if(line.line.find(',') != std::string::npos){
            auto result = std::make_shared<function::MaskTemplate>();
            if(line.line.starts_with('['))
                result->type = TemplateType::List;
            else
                result->type = TemplateType::Tuple;
            auto list_elements = divide_pps_by(line, ',');
            auto constructed = construct_templates(list_elements);
            result->First = construct_iterative(constructed, result->type);
            return result;
        }
        auto desugared_list = parse_desugared_list(line);
        if(desugared_list != nullptr)
            return desugared_list;

        line = parse_constructor_operators(line);
        line = parse_named_constructors(line);
        if (line.line == "$") {
            return line.get_resulting_template();
        } else {
            return std::make_shared<MaskTemplate>(line);
        }
    }

    PartiallyParsedString PartiallyParsedString::parse_any_brackets(PartiallyParsedString parsedLine, char open, char close) {
        PartiallyParsedString result{};
        std::string line = parsedLine.line;
        bool exter_brackets = line.starts_with(open) && line.ends_with(close);
        PartiallyParsedString bracket_part;
        int i = exter_brackets, n = (int) line.size() - exter_brackets, bracket_cnt = 0;

        int j = 0;
        while (i < n) {
            char cur = line[i];
            if(bracket_cnt > 0){
                if(line[i] == '$'){
                    bracket_part.line += '$';
                    if(i >= n - 1 || line[i+1] != '$'){
                        bracket_part.replacements.push_back(parsedLine.replacements[j++]);
                    }else{
                        bracket_part.line += '$';
                        ++i;
                    }
                    ++i;
                    continue;
                }
            } else {
                if(line[i] == '$'){
                    result.line += '$';
                    if(i >= n - 1 || line[i+1] != '$'){
                        result.replacements.push_back(parsedLine.replacements[j++]);
                    }else{
                        result.line += '$';
                        ++i;
                    }
                    ++i;
                    continue;
                }
            }

            if (bracket_cnt == 0 && cur != open)
                result.line += cur;
            else if (bracket_cnt > 0)
                bracket_part.line += cur;

            if (cur == open) {
                if (bracket_cnt == 0)
                    bracket_part.line = "", bracket_part.line += open;
                bracket_cnt++;
            } else if (cur == close) {
                if (bracket_cnt == 1) {
                    result.line += '$'; //maybe "$ "
                    result.replacements.emplace_back(parse_line(bracket_part));
                }
                bracket_cnt--;
            }
            i++;
        }
        if(exter_brackets)
            result.line = open + result.line + close;
        return result;
    }

    PartiallyParsedString PartiallyParsedString::parse_internal_brackets(const PartiallyParsedString &parsedLine) {
        return parse_any_brackets(parsedLine, '(', ')');
    }

    PartiallyParsedString PartiallyParsedString::parse_sugared_lists(const PartiallyParsedString &parsedLine) {
        return parse_any_brackets(parsedLine, '[', ']');
    }

    PartiallyParsedString PartiallyParsedString::substr(int start, int finish){
        PartiallyParsedString substring;
        int j = 0;
        for(int i = 0; i < start; i++){
            if(line[i] == '$' && line[i+1] != '$')
                j++;
        }
        if(start > 0 && line[start-1] == '$' && line[start] == '$')
            start++;
        if(finish < line.size() && line[finish+1] == '$' && line[finish] == '$')
            finish++;
        substring.line = line.substr(start, finish);
        for(int i = start; i <= finish; i++){
            if(line[i] == '$' && line[i+1] != '$')
                substring.replacements.push_back(replacements[j++]);
        }
        return substring;
    }

    std::shared_ptr<function::MaskTemplate> PartiallyParsedString::parse_constructor_operators(PartiallyParsedString parsedString) {
        size_t col = parsedString.line.find(':');
        if(col == std::string::npos)
            return nullptr;
        auto first_half = parsedString.substr(0, col);
        auto first_ptr = std::make_shared<function::MaskTemplate>(first_half);
        while(parsedString.operators.contains(parsedString.line[col]))
            col++;
        auto second_half = parsedString.substr(col, parsedString.line.size());
        auto second_ptr = std::make_shared<function::ComplexMaskTemplate>(second_half);
        auto result = std::make_shared<function::ComplexMaskTemplate>();
        result->type = TemplateType::DataConstructor;
        result->First = first_ptr;
        result->Rest = second_ptr;
        return result;
    }

    PartiallyParsedString PartiallyParsedString::parse_named_constructors(const PartiallyParsedString &line) {
        size_t col = parsedString.line.find(':');
        if(col == std::string::npos)
            return nullptr;
        auto first_half = parsedString.substr(0, col);
        auto first_ptr = std::make_shared<function::MaskTemplate>(first_half);
        while(parsedString.operators.contains(parsedString.line[col]))
            col++;
        auto second_half = parsedString.substr(col, parsedString.line.size());
        auto second_ptr = std::make_shared<function::ComplexMaskTemplate>(second_half);
        auto result = std::make_shared<function::ComplexMaskTemplate>();
        result->type = TemplateType::DataConstructor;
        result->First = first_ptr;
        result->Rest = second_ptr;
        return result;
    }

    std::vector<std::shared_ptr<function::MaskTemplate>> PartiallyParsedString::construct_templates(std::vector<PartiallyParsedString> partuallyParsedStrings) {
        std::vector<std::shared_ptr<function::MaskTemplate>> result;
        for(auto pps : partuallyParsedStrings){
            result.emplace_back(PartiallyParsedString::parse_line(pps));
        }
        return result;
    }

    PartiallyParsedString::PartiallyParsedString(const std::string &line) {
        this->line = line;
    }


}





