#include "LineParser.h"

LineParser::LineParser(){
    state = Regular;
    current_indent = 0;
    line_under_construction = lines::CompressedLine();
}

std::vector<lines::CompressedLine> LineParser::get_result() {
    if(!line_under_construction.line.empty()){
        result.push_back(line_under_construction);
    }
    return result;
}

int indent_of_commands(const std::string& line, int starting_indent = 0){
    int ind = starting_indent, result = -1;
    size_t n = line.size();
    while(ind < n){
        if(line[ind] == ' ')
            ind++;
        else{
            return (int) ind;
        }
    }
    return result;
}

void LineParser::parse_line(const std::pair<std::string, int>& parsed_line) {
    if(parsed_line.first.starts_with("module")){
        line_under_construction.append(parsed_line.first);
        return;
    }
    if(state == Regular){

        int where_id = parsed_line.first.find("where"); //fix all 'find' occurences
        //TODO: REWRITE
        if(where_id >= 0){
            std::string line = parsed_line.first;
            if(parsed_line.second > current_indent){
                line_under_construction.append(line.substr(0, where_id));
            } else {
                result.push_back(line_under_construction);
                line_under_construction = lines::CompressedLine(line.substr(0, where_id));
                current_indent = parsed_line.second;
            }
            current_indent = indent_of_commands(line, where_id + 5);
            if(current_indent > 0){
                line_under_construction.where_clause.push_back(line.substr(current_indent));
                current_indent += parsed_line.second;
                state = Where;
            } else {
                state = WhereUnknown;
            }
        } else {
            if(parsed_line.second > current_indent){
                line_under_construction.append(parsed_line.first);
            } else {
                result.push_back(line_under_construction);
                line_under_construction = lines::CompressedLine(parsed_line.first);
                current_indent = parsed_line.second;
            }
        }
    }
    else if(state == WhereUnknown){
        //TODO: Maybe rewrite, only checks if has commands, and empty lines aren't allowed, can simply use indent
        current_indent = indent_of_commands(parsed_line.first);
        if(current_indent >= 0){
            state = Where;
            line_under_construction.where_clause.push_back(parsed_line.first.substr(current_indent));
            current_indent += parsed_line.second;
        }
    } else if (state == Where){
        if(parsed_line.second == current_indent){
            line_under_construction.where_clause.push_back(parsed_line.first);
        } else if (parsed_line.second > current_indent){
            //adds this line to current line of where
            line_under_construction.append_to_where(parsed_line.first);
        } else {
            result.push_back(line_under_construction);
            line_under_construction = lines::CompressedLine(parsed_line.first);
            current_indent = parsed_line.second;
            state = Regular;
        }
    }
}
