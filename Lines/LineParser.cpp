#include "LineParser.h"
#include "IndentedLine.h"

LineParser::LineParser(){
    state = Regular;
    current_indent = 0;
    line_under_construction = lines::Statement();
}

std::vector<lines::Statement> LineParser::get_result() {
    if(!line_under_construction.line.empty()){
        result.push_back(line_under_construction);
    }
    return result;
}

void LineParser::parse_line(const lines::IndentedLine& line) {
    if(parse_special_cases(line))
        return;
    if(state == Regular){
        int where_id = line.parsed_line.find("where"); //fix all 'find' occurences
        add_body_to_current_line(line, where_id);
        if(where_id >= 0)
            update_where_state(line, where_id + 5);
    }
    else if(state == WhereUnknown){
        update_where_state(line);
    }
    else if (state == Where){
        if(line.indentation == current_indent){
            line_under_construction.where_clause.push_back(line.parsed_line);
        } else if (line.indentation > current_indent){
            line_under_construction.append_to_where(line.parsed_line);
        } else {
            push_next_line(line);
        }
    }
    line_under_construction.append_to_original(line.original_line);
}

bool LineParser::parse_special_cases(const lines::IndentedLine& line){
    if(line.parsed_line.starts_with("module") || line.parsed_line.starts_with("import")) {
        push_next_line(line);
        line_under_construction.append_to_original(line.original_line);
        return true;
    }
    return false;
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

void LineParser::add_body_to_current_line(const lines::IndentedLine& indented_line, int prefix_length){
    if(indented_line.indentation > current_indent){
        const std::string prefix = prefix_length > 0 ? indented_line.parsed_line.substr(0, prefix_length) : indented_line.parsed_line;
        line_under_construction.append(prefix);
    } else {
        push_next_line(indented_line, prefix_length);
    }
}

void LineParser::push_next_line(const lines::IndentedLine& line, int prefix_length){
    const std::string prefix = prefix_length > 0 ? line.parsed_line.substr(0, prefix_length) : line.parsed_line;

    result.push_back(line_under_construction);
    line_under_construction = lines::Statement(prefix);
    current_indent = line.indentation;
    state = Regular;
}

void LineParser::update_where_state(const lines::IndentedLine& line, int offset) {
    current_indent = indent_of_commands(line.parsed_line, offset);
    if(current_indent >= 0){
        state = Where;
        line_under_construction.where_clause.push_back(line.parsed_line.substr(current_indent));
        current_indent += line.indentation;
    }
}
