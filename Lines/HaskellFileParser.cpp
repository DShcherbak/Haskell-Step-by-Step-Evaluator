#include <stack>
#include <cassert>
#include "HaskellFileParser.h"

namespace lines {

    bool is_operator(char c){
        return c == '=' || c == ';' || c == '{' || c == '}' || c == '(' || c == ')';
    }

    bool is_spacy(char c){
        return c == ' ' || is_operator(c);
    }

    string found_keyword(const std::string &line, size_t& id){
        size_t n = line.size();
        if(line[id] == '}'){
            return "}";
        }

        for(auto &key : std::vector<string>{"where", "let", "do", "in", "of"}){
            size_t k = key.size();
            if(is_spacy(line[id])
                    && id + k + 1 <= n
                    && (id + k + 1 == n || is_spacy(line[id+k+1]))) {
                if (line.substr(id + 1, k) == key) {
                    if (key != "in")
                        id += k + 1;
                    else
                        id++;
                    return key;
                }
            } else if (id == 0 && id + k <= n && line.substr(id,k) == key && (id + k == n || is_spacy(line[id+k]))){
                    if(key != "in")
                        id += k;
                    return key;
            }
        }
        return "";
    }

    std::string insert_at(const std::string& str, char c, size_t position, bool remove = false){
        auto first_part = str.substr(0, position);
        auto second_part = str.substr(position + (remove ? 1 : 0));
        return first_part + c + second_part;
    }

    size_t count_block_indent(indent_vector & lines_with_indentation, size_t &line_id, size_t &char_id){
        size_t current_indent = lines_with_indentation[line_id].indentation;
        auto line = lines_with_indentation[line_id].parsed_line;
        size_t n = line.size();
        if (char_id < n) {
            if (line[char_id] == '{')
                return 0;
            else {
                lines_with_indentation[line_id].parsed_line = insert_at(line, '{', char_id-1, true); //line.substr(0, char_id - 1) + "{" + line.substr(char_id);
                return lines_with_indentation[line_id].indentation + char_id; //+1?
            }
        } else {
            lines_with_indentation[line_id].parsed_line.append("{");
            line_id++;
            char_id = 0;
            size_t new_indentation = lines_with_indentation[line_id].indentation;
            if(new_indentation > current_indent)
                return new_indentation;
            else{
                lines_with_indentation[line_id-1].parsed_line.append("}"); //line.substr(0, char_id - 1) + "{" + line.substr(char_id);
                return 0;
            }
        }
    }

    //returns true if found end of a "do" block or a "where" block and need to exit recursion
    bool check_for_new_line_operators(const std::string& key, indent_vector & lines_with_indentation, size_t &line_id, size_t &char_id, size_t block_indent, size_t do_let_special_case){
        auto line = lines_with_indentation[line_id].parsed_line;
        if(char_id == 0 && block_indent > 0){
            if(lines_with_indentation[line_id].indentation == block_indent){
                lines_with_indentation[line_id].insert_at(';', 0);//";" + lines_with_indentation[line_id].parsed_line;
            }
            else if(lines_with_indentation[line_id].indentation < block_indent){
                if(key == "where" || key == "do" || key == "of" ||
                        (key == "let" && do_let_special_case > 0 && lines_with_indentation[line_id].indentation == do_let_special_case)){
                    --line_id;
                    lines_with_indentation[line_id].parsed_line.append("}");
                    char_id = lines_with_indentation[line_id].parsed_line.size()+2;//end of the line
                    return true;
                }
            }
        }
        return false;
    }


    //special case : where ends the ongoing do-block (and let mean differently)
    void recursively_parse_keyword(const string &key, indent_vector &lines, size_t &line_id,
                                   size_t &char_id,
                                   size_t do_let_special = 0) {
        size_t block_indent = 0;
        bool enclosed = false;
        auto line = lines[line_id].parsed_line;
        size_t n = line.size();
        while(char_id < n && line[char_id] == ' ') ++char_id;
        if(key != "in" && key != "}") {
            block_indent = count_block_indent(lines, line_id, char_id);
            enclosed = block_indent == 0;
        }

        for(size_t lines_n = lines.size(); line_id < lines_n; line_id++){
            if (check_for_new_line_operators(key, lines, line_id, char_id, block_indent, do_let_special))
                return;

            for(; char_id < lines[line_id].parsed_line.size(); char_id++){
                string new_key = found_keyword(lines[line_id].parsed_line, char_id);
                if(!new_key.empty()){
                    if(new_key == "}"){
                        if(enclosed){
                            if(key == "let"){
                                char_id++;
                                lines[line_id].insert_at(' ', char_id);
                                n = lines[line_id].parsed_line.size();
                                while(char_id < n && lines[line_id].parsed_line[char_id] == ' ') ++char_id;
                                if(char_id == n){
                                    line_id++;
                                    char_id = 1;//misss ; because of this
                                }
                                new_key = found_keyword(lines[line_id].parsed_line, --char_id);
                                //std::cout << "LINE: \"" << lines[line_id].parsed_line.substr(char_id)<< "\"\n";
                                //std::cout << "IN: \"" << lines[line_id].parsed_line.substr(char_id, 3) <<  "\"\n";
                                if(new_key != "in")
                                    lines[line_id].insert_at(';', 0);
                                char_id+=1;
                            }
                        } else { //DO can't be inside of a {}, can It?
                                // Can a let inside a do have more lets and wheres, and
                            lines[line_id].insert_at('}', char_id);
                            ++char_id;
                        }
                        return;
                    } else {
                        if(key == "do"){
                            if(new_key == "let"){
                                recursively_parse_keyword(new_key, lines, line_id, char_id, block_indent);
                            } else {
                                lines[line_id].insert_at('}', char_id - new_key.size());
                                char_id += 2; // TOTAL MISTERY, but where after do breaks without it
                                recursively_parse_keyword(new_key, lines, line_id, char_id);
                                return;
                            }
                        } else {
                            if (new_key == "in") {
                                lines[line_id].insert_at('}', char_id);
                                if (key == "let")
                                    ++char_id;
                                else
                                    --char_id;
                                return;
                            } else {
                                recursively_parse_keyword(new_key, lines, line_id, char_id, do_let_special);
                            }
                        }
                    }

                }
            }
            char_id = 0;
        }
        lines[line_id-1].parsed_line.append("}");
    }

    indent_vector remove_sugar_from_where(indent_vector &lines_with_indentation){
        for(size_t line_id = 0, lines_n = lines_with_indentation.size(); line_id < lines_n; line_id++){
            for(size_t char_id = 0; char_id < lines_with_indentation[line_id].parsed_line.size(); char_id++){
                string key = found_keyword(lines_with_indentation[line_id].parsed_line, char_id);
                if(!key.empty()){
                    recursively_parse_keyword(key, lines_with_indentation, line_id, char_id);
                }
            }
        }
        return lines_with_indentation;
    }

    std::vector<std::string> HaskellFileParser::parse_file(const std::string &file_name) {
        std::vector<std::string> lines;
        lines = get_lines_from_file(file_name);
        lines = preprocess_lines(lines);

        indent_vector lines_with_indentation = indentation(lines);
        indent_vector lines_without_comments = remove_comments(lines_with_indentation);
        indent_vector lines_with_desugared_where = remove_sugar_from_where(lines_without_comments);
        std::vector<string> statements = compress_lines(lines_with_desugared_where);
/*        for(auto &st : statements){
            std::cout << st << std::endl;
        }*/
        return statements;
    }


    std::vector<std::string> HaskellFileParser::get_lines_from_file(const std::string &file_name) {
        std::vector<std::string> lines;
        try {
            std::fstream istream;
            istream.open(file_name, std::fstream::in);
            if (istream.is_open()) {
                typedef std::istreambuf_iterator<char> buf_iter;
                char c;
                std::string current_line;
                while (std::getline(istream, current_line)) {
                    lines.push_back(current_line);
                }
            }
            istream.close();
        }
        catch (std::exception &ex) {
            std::cout << "Couldn't parse '" << file_name << "', got an error: " << ex.what() << std::endl;
        }
        return lines;
    }

    std::vector<string> HaskellFileParser::preprocess_lines(const std::vector<string> &lines) {
        std::vector<string> result;
        int current_line = 0;
        for (auto &line: lines) {
            result.emplace_back("");
            for (char c: line) {
                if (c == '\n') {
                    continue;
                } else if (c == '\t') {
                    result[current_line] += "    ";
                } else {
                    result[current_line] += c;
                }
            }
            current_line++;
        }
        return result;
    }

    indent_vector HaskellFileParser::indentation(const std::vector<std::string> &lines) {
        indent_vector result;
        for (auto &line: lines) {
            if (line.empty())
                continue;
            size_t ident = 0, i = 0, n = line.size();
            string new_string;
            while (i < n && line[i] == ' ') {
                ident++;
                i++;
            }
            if (i == n)
                continue;
            new_string += line.substr(i);
            result.emplace_back(new_string, line, ident);
        }
        return result;
    }

    indent_vector HaskellFileParser::remove_comments(const indent_vector &lines) {
        indent_vector result;
        size_t nested_comments = 0;
        bool inside_string_literal = false;
        for (auto &l: lines) {
            auto line = l.parsed_line;


            if (line.empty())
                continue;

            auto result_str = l;
            result_str.parsed_line = "";
            size_t i = 0, n = line.size();
            while (i < n) {
                if(inside_string_literal){
                    if(line[i] == '"' && line[i-1] != '\\'){
                        inside_string_literal = false;
                    }
                    result_str.parsed_line += line[i++];
                } else if(nested_comments > 0){
                    if (line[i] == '{' && i < n - 1 && line[i + 1] == '-') {
                        nested_comments++;
                        i+=2;
                    } else if (line[i] == '-' && i < n - 1 && line[i + 1] == '}') {
                        nested_comments--;
                        i+=2;
                    } else {
                        i++;
                    }
                } else {
                    if (line[i] == '{' && i < n - 1 && line[i + 1] == '-') {
                        nested_comments++;
                        i+=2;
                    } else  if (line[i] == '-' && i < n - 1 && line[i + 1] == '-') {
                        break;
                    } else {
                        if(line[i] == '"' && (i == 0 || line[i-1] != '\\'))
                            inside_string_literal = true;
                        result_str.parsed_line += line[i++];
                    }
                }
            }
            if(!result_str.parsed_line.empty())
                result.emplace_back(result_str);
        }
        return result;
    }

    std::vector<std::string> HaskellFileParser::compress_lines(const indent_vector &lines) {
        std::vector<std::string> result;
        size_t i = 0, j = 0, n = lines.size();
        while(i < n){
            result.emplace_back(lines[i++].parsed_line);
            while(i < n && lines[i].indentation > 0)
                result[j].append(" " + lines[i++].parsed_line);
            j++;
        }
        return result;
    }

    std::vector<std::string> HaskellFileParser::read_prelude() {
        return get_lines_from_file("../input/Prelude.hs");
    }


}