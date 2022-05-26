#include <stack>
#include "HaskellFileParser.h"

namespace lines {

    bool is_operator(char c){
        return c == '=' || c == ';' || c == '{' || c == '}';
    }

    bool is_spacy(char c){
        return c == ' ' || is_operator(c);
    }

    string found_keyword(const std::string &line, size_t& id){
        size_t n = line.size();
        if(!is_spacy(line[id]))
            return "";
        for(auto &key : std::vector<string>{"where", "let", "do"}){
            size_t k = key.size();
            if(id + k + 1 <= n && line.substr(id+1,k) == key && (id + k + 1 == n || is_spacy(line[id+k+1]))){
            //    std::cout << line.substr(id+1,k) << "<==>" << key << std::endl;
                id += k + 1;
                return key;
            }
        }
        return "";
    }

    //special case : where ends the ongoing do-block (and let mean differently)
    void recursively_parse_keyword(const string &key, indent_vector & lines_with_indentation, size_t &line_id, size_t &char_id){
        size_t block_indent = 0;
        auto line = lines_with_indentation[line_id].parsed_line;
        size_t n = line.size();
        while(char_id < n && line[char_id] == ' ') ++char_id;
        if(char_id < n){
            if(line[char_id] == '{')
                return;
            else {
                lines_with_indentation[line_id].parsed_line = line.substr(0, char_id-1) + "{" + line.substr(char_id);
                line = lines_with_indentation[line_id].parsed_line;
                block_indent = lines_with_indentation[line_id].indentation + char_id;
            }
        } else {
            lines_with_indentation[line_id].parsed_line.append("{");
            line_id++;
            block_indent = lines_with_indentation[line_id].indentation;
            char_id = 0;
        }

        for(size_t lines_n = lines_with_indentation.size(); line_id < lines_n; line_id++){
            if(char_id == 0){
                if(lines_with_indentation[line_id].indentation == block_indent){
                    --lines_with_indentation[line_id].indentation;
                    lines_with_indentation[line_id].parsed_line = ";" + lines_with_indentation[line_id].parsed_line;
                }
                else if(lines_with_indentation[line_id].indentation < block_indent){
                    lines_with_indentation[line_id-1].parsed_line.append("}");
                    --line_id;
                    char_id = lines_with_indentation[line_id].parsed_line.size();
                    return;
                }
            }
            for(;char_id < lines_with_indentation[line_id].parsed_line.size(); char_id++){
                string new_key = found_keyword(lines_with_indentation[line_id].parsed_line, char_id);
                if(!new_key.empty()){
                    recursively_parse_keyword(new_key, lines_with_indentation, line_id, char_id);
                }
            }
            char_id = 0;
        }


    }

    indent_vector remove_sugar_from_where(indent_vector &lines_with_indentation){
        for(size_t line_id = 0, lines_n = lines_with_indentation.size(); line_id < lines_n; line_id++){
            auto line_with_ind = lines_with_indentation[line_id];
            for(size_t char_id = 0; char_id < lines_with_indentation[line_id].parsed_line.size(); char_id++){
                string key = found_keyword(lines_with_indentation[line_id].parsed_line, char_id);
                if(!key.empty()){
                    recursively_parse_keyword(key, lines_with_indentation, line_id, char_id);
                }
            }
        }


        return lines_with_indentation;
    }

    statement_vector HaskellFileParser::parse_file(const std::string &file_name) {
        std::vector<std::string> lines;
        lines = get_lines_from_file(file_name);
        lines = preprocess_lines(lines);

        indent_vector lines_with_indentation = indentation(lines);
        indent_vector lines_without_comments = remove_comments(lines_with_indentation);
        indent_vector lines_with_desugared_where = remove_sugar_from_where(lines_without_comments);
        indent_vector statements = compress_lines1(lines_with_desugared_where);
        for(auto &l : statements){
            std::cout << l.parsed_line << std::endl;
        }
        return {};
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
            /*
            while (i < n) {
                if (line[i] == ' ') {
                    new_string += ' ';
                    while (line[i] == ' ') {
                        i++;
                    }
                } else if (line[i] == '-' && i < n - 1 && line[i + 1] == '-') {
                    break;
                } else {
                    new_string += line[i++];
                }
            }*/
            result.emplace_back(new_string, line, ident);
        }
        return result;
    }

    indent_vector HaskellFileParser::remove_comments(const indent_vector &lines) {
        indent_vector result;
        size_t nested_comments = 0;
        for (auto &l: lines) {
            auto line = l.parsed_line;


            if (line.empty())
                continue;

            auto result_str = l;
            result_str.parsed_line = "";
            size_t i = 0, n = line.size();
            while (i < n) {
                if (line[i] == '-' && i < n - 1 && line[i + 1] == '-') {
                    break;
                } else if (line[i] == '{' && i < n - 1 && line[i + 1] == '-') {
                    nested_comments++;
                    i+=2;
                } else if (line[i] == '-' && i < n - 1 && line[i + 1] == '}') {
                    nested_comments--;
                    i+=2;
                }else if (nested_comments == 0){
                    result_str.parsed_line += line[i++];
                } else {i++;}
            }
            if(!result_str.parsed_line.empty())
                result.emplace_back(result_str);
        }
        return result;
    }

    indent_vector HaskellFileParser::compress_lines1(const indent_vector &lines) {
        indent_vector result;
        size_t i = 0, j = 0, n = lines.size();
        while(i < n){
            result.emplace_back(lines[i++]);
            while(i < n && lines[i].indentation > 0)
                result[j].parsed_line.append(" " + lines[i++].parsed_line);
            j++;
        }
        return result;
    }

    statement_vector HaskellFileParser::compress_lines(const indent_vector &lines) {
        size_t cur_line = 0, compressed_line_id = 0, next_line = 0, n = lines.size();

        LineParser parser = LineParser();
        for (auto &line: lines) {
            parser.parse_line(line);
        }
        return parser.get_result();
    }

}