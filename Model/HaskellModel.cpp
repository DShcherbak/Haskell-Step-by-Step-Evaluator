#include "HaskellModel.h"

void HaskellModel::AddStatements(std::vector<lines::Statement> &statements) {
    statements = process_headers(statements);
    statements = process_data_types(statements);
    statements = process_type_classes(statements);
    process_functions(statements);
}


std::pair<statement_vector, statement_vector> filter_by_prefixes(const std::vector<lines::Statement> &statements, const std::vector<std::string> &prefixes){
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

void HaskellModel::process_functions(const std::vector<lines::Statement> &statements) {
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

std::vector<lines::Statement> HaskellModel::process_type_classes(const std::vector<lines::Statement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, type_prefixes);

    //TODO : Do something with type classes

    return filtered.second;
}

std::vector<lines::Statement> HaskellModel::process_data_types(const std::vector<lines::Statement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, data_prefixes);

    //TODO : Do something with data

    return filtered.second;
}



std::vector<lines::Statement> HaskellModel::process_headers(const std::vector<lines::Statement> &statements) {
    std::pair<statement_vector, statement_vector> filtered = filter_by_prefixes(statements, header_prefixes);

    //TODO : Do something with headers

    return filtered.second;
}

