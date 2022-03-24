#include "HaskellModel.h"



indent_vector indentation(lines_vector lines){
    indent_vector result;
    for(auto &line : lines){
        if(line.empty())
            continue;
        size_t ident = 0, i = 0, n = line.size();
        while(i < n && (line[i] == ' ' || line[i] == '\t')) {
            if(line[i] == ' ')
                ident++;
            else
                ident += 4;
            i++;
        }
        if(i == n)
            continue;
        string new_string = "";
        while(i < n){
            if(line[i] == ' ' || line[i] == '\t'){
                while(line[i] == ' ' || line[i] == '\t'){
                    i++;
                }
                new_string += ' ';
                continue;
            }
            if(line[i] == '-' && i < n - 1 && line[i+1] == '-'){
                break;
            }
            new_string += line[i++];
        }
        result.emplace_back(new_string, ident);
    }
    return result;
}

compressed_vector compress_lines(const indent_vector& lines){
    compressed_vector result;
    size_t cur_line = 0, compressed_line_id = 0, next_line = 0, n = lines.size();
    while(cur_line < n){
        bool where = false;
        result.emplace_back(lines[cur_line].first,lines[cur_line].second,std::vector<std::string>());
        next_line = cur_line + 1;
        if(lines[cur_line].first.find("where") > 0){
            //do where
        }
        while(next_line < n - 1){
            if (lines[next_line].second > lines[cur_line].second){
                result[compressed_line_id]
            }
            if(lines[next_line].first.find("where") > 0){
                //do where
            }
            else
        }

            compressed_line_id++;
    }
    return result;
}

void HaskellModel::AddFile(const std::string &fileName) {
    lines_vector lines;

    try{
        std::fstream istream(fileName, std::fstream::in);
        typedef std::istreambuf_iterator<char> buf_iter;
        char c;
        lines.emplace_back("");
        int current_line = 0;
        for(buf_iter i(istream), e; i != e; ++i){
            c = *i;
            if(c == '\n'){
                lines.emplace_back("");
                current_line++;
            } else {
                lines[current_line] += c;
            }
        }
    }catch(std::exception& ex){
        std::cout << "Couldn't parse '" << fileName << "', got an error: " << ex.what() << std::endl;
    }
    std::cout << lines.size() << std::endl;
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
