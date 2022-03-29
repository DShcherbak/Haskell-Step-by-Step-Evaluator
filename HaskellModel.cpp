#include "HaskellModel.h"
#include "Lines/LineParser.h"


indent_vector indentation(const std::vector<std::string>& lines){
    indent_vector result;
    for(auto &line : lines){
        if(line.empty())
            continue;
        size_t ident = 0, i = 0, n = line.size();
        string new_string;
        while(i < n && line[i] == ' ') {
            ident++;
            i++;
        }
        if(i == n)
            continue;
        while(i < n){
            if(line[i] == ' '){
                new_string += ' ';
                while(line[i] == ' ' ){
                    i++;
                }
            } else if(line[i] == '-' && i < n - 1 && line[i+1] == '-'){
                break;
            } else {
                new_string += line[i++];
            }
        }
        result.emplace_back(new_string, ident);
    }
    return result;
}

compressed_vector compress_lines(const indent_vector& lines){
    size_t cur_line = 0, compressed_line_id = 0, next_line = 0, n = lines.size();

    LineParser parser = LineParser();
    for(auto &line : lines){
        parser.parse_line(line);
    }
    return parser.get_result();
}

void HaskellModel::AddFile(const std::string &fileName) {
    std::vector<std::string> lines;

    try{
        std::fstream istream;
        istream.open(fileName, std::fstream::in);
        if(istream.is_open()) {
            typedef std::istreambuf_iterator<char> buf_iter;
            char c;
            std::string current_line;
            while (std::getline(istream, current_line)) {
                lines.push_back(current_line);
            }
        }
        istream.close();
    }
    catch(std::exception& ex){
        std::cout << "Couldn't parse '" << fileName << "', got an error: " << ex.what() << std::endl;
    }
    std::cout << lines.size() << std::endl;
    lines = preprocess_lines(lines);
    indent_vector lines_with_indentation = indentation(lines);
    compressed_vector compressed_lines = compress_lines(lines_with_indentation);
    for(int i = 0, n = lines.size(); i < n; i++){
        auto line = lines[i];
        if(!line.empty()){
            if(line.starts_with("module")){
                //module name?
            } else if(line.starts_with("import")){
               // imports.emplace_back(line);
            } else if (line.find("::") > 0){
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
        }
    }


}

std::vector<string> HaskellModel::preprocess_lines(const std::vector<string>& lines) {
    std::vector<string> result;
    int current_line = 0;
    for(auto &line : lines){
        result.emplace_back("");
        for(char c : line) {
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
