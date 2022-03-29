#include "LineParser.h"

LineParser::LineParser(){
    state = Regular;
    current_indent = 0;
    state_indent = 0;
    line_under_construction = lines::CompressedLine();
}

std::vector<lines::CompressedLine> LineParser::get_result() {
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
    if(state == Regular){
        int where_id = parsed_line.first.find("where"); //fix all 'find' occurences
        if(where_id >= 0){
            std::string line = parsed_line.first;
            line_under_construction.append(line.substr(0, where_id));
            current_indent = indent_of_commands(line, where_id + 4);
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
        current_indent = indent_of_commands(parsed_line.first, parsed_line.second);
        if(current_indent > 0){
            state = Where;
            line_under_construction.where_clause.push_back(parsed_line.first.substr(current_indent));
        }
    } else if (state == Where){
        if(parsed_line.second == current_indent){
            line_under_construction.where_clause.push_back(parsed_line.first);
        } else if (parsed_line.second == current_indent){
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
