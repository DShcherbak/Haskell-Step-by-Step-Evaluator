#include "HaskellModel.h"

void HaskellModel::AddStatements(std::vector<lines::LineStatement> &statements) {
    statements = process_headers(statements);
    statements = process_data_types(statements);
    statements = process_type_classes(statements);
    process_functions(statements);
    std::cout << functions.size() << std::endl;
    std::cout << functions.size() << std::endl;
}


std::pair<statement_vector, statement_vector> filter_by_prefixes(const std::vector<lines::LineStatement> &statements, const std::vector<std::string> &prefixes){
    statement_vector filtered, filtered_out;
    for(auto& statement : statements){
        bool found = false;
        for(const auto& prefix: prefixes){
            if(statement.line.starts_with(prefix)){
                filtered.push_back(statement);
                found = true;
            }
        }
        if(!found)
            filtered_out.push_back(statement);
    }
    return {filtered, filtered_out};
}

std::string get_name(const std::string& line){
    return line.substr(0, line.find(' '));
}

std::shared_ptr<function::Function> HaskellModel::GetOrCreateFunction(const std::string& name){
    if(!functions.contains(name)){
        functions[name] = std::make_shared<function::Function>(name);
    }
    return functions[name];
}

void HaskellModel::function_type_define(const std::string& name, const lines::LineStatement& stmnt){
    auto func = GetOrCreateFunction(name);
    func->type_definition = stmnt.line;
    func->type_definition_string = stmnt.original_line;
}

void HaskellModel::process_function_guards(std::shared_ptr<function::Mask> mask, const std::string& line, size_t head_start){
    size_t equal_id, next_guard_id = head_start;
    do{
        // TODO: == and /= in guards !!!
        equal_id = line.find('=', next_guard_id + 1);
        while(line.substr(equal_id - 1, 2) == "/=" || line.substr(equal_id - 1, 2) == "==" || line.substr(equal_id, 2) == "=="){
            equal_id = line.find('=', equal_id + 1);
        }
        std::string guard_head = line.substr(next_guard_id + 1, equal_id - next_guard_id - 1);
        next_guard_id = line.find('|', equal_id + 1);
        std::string body = line.substr(equal_id + 1, next_guard_id - equal_id - 1);
        mask->add_guard(guard_head, body);
    }while(next_guard_id < line.size());
}

void HaskellModel::add_expression_to_function(const std::string& name, const lines::LineStatement &stmnt) {
    std::string line = stmnt.line;
    size_t head_start = line.find('=');
    size_t guard_start = line.find('|');

    std::shared_ptr<function::Mask> mask;
    if(guard_start < head_start){
        head_start = guard_start;
        std::string head = line.substr(0, head_start);
        mask = std::make_shared<function::Mask>(head);
        process_function_guards(mask, line, guard_start);

    } else {
        std::string head = line.substr(0, head_start);
        std::string body = line.substr(head_start+1);

        mask = std::make_shared<function::Mask>(head);
        mask->add_body(body);
    }
    auto func = GetOrCreateFunction(name);
    func->add_mask(mask);


}

void HaskellModel::process_functions(const std::vector<lines::LineStatement> &statements) {
    for(const auto& statement : statements){
        const std::string& line = statement.line;
        std::string name = get_name(line);

        if(line.substr(name.size() + 1).starts_with("::")){
            function_type_define(name, statement);
        }
        else {
            add_expression_to_function(name, statement);
        }
    }
    /*
     if (line.find("::") > 0){
                while(i + 1< n  && (lines[i+1].starts_with('\t') || lines[i+1].starts_with(' '))){
                    line.append(lines[i+1]);
                    i+=1;
                }
                auto name = line.substr(0,line.find(' '));
                if(functions.contains(name))
                    continue;
                size_t count_params = 0, start_pos = 0, pos = 0;
                do {
                    pos = 1 + line.find("->", start_pos);
                    if(pos >= 0){
                        start_pos = pos + 1;
                        count_params++;
                    }
                }while(pos > 0);
                //functions.insert({name, count_params});
            } else if (line.find('=') >= 0){
                auto name = line.substr(0,line.find(' '));

                if(!functions.contains(name)){
                    int ch = 0;
                    int number_of_params = 0;
                    for(; ch < line.size() && line[ch] != '='; ch++){
                        int depth = 0;
                        while(depth == 0 && line[ch] == ' '){
                            ch++;
                        }
                        if (line[ch] == '('){
                            depth++;
                        } else if (line[ch] == ')'){
                            depth--;
                        } else {

                        }
                    }
                }
                std::vector<std::string> function_body;
                function_body.push_back(line);
                while(i + 1< n  && (lines[i+1].starts_with('\t') || lines[i+1].starts_with(' '))){
                    function_body.push_back(lines[i+1]);
                    i+=1;
                }


        }
     */
}

std::vector<lines::LineStatement> HaskellModel::process_type_classes(const std::vector<lines::LineStatement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, type_prefixes);

    //TODO : Do something with type classes

    return filtered.second;
}

std::vector<lines::LineStatement> HaskellModel::process_data_types(const std::vector<lines::LineStatement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, data_prefixes);

    //TODO : Do something with data

    return filtered.second;
}



std::vector<lines::LineStatement> HaskellModel::process_headers(const std::vector<lines::LineStatement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, header_prefixes);

    //TODO : Do something with headers

    return filtered.second;
}

