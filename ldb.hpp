#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class ldb
{
    private:
        string db_name;
        string spaces;
        bool loaded_db = false;
        bool in_class = false;
        bool in_header = false;
        vector<string> classes;
        vector<string> headers;
        vector<string> datas;
        string raw;
        bool starts_with(string line, string find)
        {
            return line.compare(0, find.length(), find) == 0;
        }
        vector<string> split(string line, string delimiter)
        {
            vector<string> tokens;
            string token;
            size_t start = 0;
            size_t end;
            size_t len = delimiter.length();
            while ((end = line.find(delimiter, start)) != string::npos)
            {
                token = line.substr(start, end - start);
                start = end + len;
                tokens.push_back(token);
            }
            tokens.push_back(line.substr(start));
            return tokens;
        }
    public:
        void init(string file)
        {
            if (file.substr(file.find_last_of("."), file.length() - file.find_last_of(".")) != ".ldb")
            {
                printf("Unknown file type <%s>.\n", file);
            }
            ifstream data(file);
            file.replace(file.find_last_of("."), file.length() - file.find_last_of("."), "");
            db_name = file;
            if (data)
            {
                loaded_db = true;
                data.close();
            }
            else
            {
                printf("Invalid file <%s>.\n", (file + ".ldb").c_str());
            }
        }
        vector<string> get(string _class, string header)
        {
            if (loaded_db == true)
            {
                ifstream data(db_name + ".ldb");
                if (data)
                {
                    vector<string> lines;
                    string class_name;
                    string header_name;
                    bool type_class = false;
                    bool type_header = false;
                    while (data.good())
                    {
                        string line;
                        getline(data, line);
                        while (line.find_first_of(" ") != string::npos)
                        {
                            if (line.substr(0, 1) == " ")
                            {
                                line.replace(0, 1, "");
                                continue;
                            }
                            break;
                        }
                        if (starts_with(line, "class "))
                        {
                            line.replace(0, 6, "");
                            type_class = true;
                            class_name = line;
                        }
                        else if (starts_with(line, "header "))
                        {
                            line.replace(0, 7, "");
                            type_header = true;
                            header_name = line;
                        }
                        else if (line == ".")
                        {
                            if (type_header == true)
                            {
                                type_header = false;
                                if (header_name == header)
                                {
                                    break;
                                }
                            }
                            if (type_header == false and type_class == true)
                            {
                                type_class = false;
                            }
                            continue;
                        }
                        else
                        {
                            if (type_header == false)
                            {
                                printf("Use of data without header first <%s>.\n", line.c_str());
                                return vector<string>();
                            }
                            if (type_header == true)
                            {
                                if (class_name == _class)
                                {
                                    if (header_name == header)
                                    {
                                        lines.push_back(line);
                                        continue;
                                    }
                                }
                            }
                            printf("Unknown command <%s>.\n", line.c_str());
                            return vector<string>();
                        }
                    }
                    return lines;
                }
            }
            else
            {
                printf("Use of get_ldb without initializing it first <init_ldb>.\n");
                return vector<string>();
            }
            return vector<string>();
        }
        bool find(string _class, string header)
        {
            if (loaded_db == true)
            {
                ifstream data(db_name + ".ldb");
                if (data)
                {
                    vector<string> lines;
                    string class_name;
                    string header_name;
                    bool type_class = false;
                    bool type_header = false;
                    while (data.good())
                    {
                        string line;
                        getline(data, line);
                        while (line.find_first_of(" ") != string::npos)
                        {
                            if (line.substr(0, 1) == " ")
                            {
                                line.replace(0, 1, "");
                                continue;
                            }
                            break;
                        }
                        if (starts_with(line, "class "))
                        {
                            if (type_class == true)
                            {
                                printf("Cannot stack classes <%s>.\n", line.c_str());
                                return false;
                            }
                            line.replace(0, 6, "");
                            type_class = true;
                            class_name = line;
                        }
                        else if (starts_with(line, "header "))
                        {
                            if (type_header == true)
                            {
                                printf("Cannot stack headers <%s>.\n", line.c_str());
                                return false;
                            }
                            line.replace(0, 7, "");
                            type_header = true;
                            header_name = line;
                        }
                        else if (line == ".")
                        {
                            if (type_header == true)
                            {
                                type_header = false;
                                if (header_name == header)
                                {
                                    return true;
                                }
                            }
                            if (type_header == false and type_class == true)
                            {
                                type_class = false;
                            }
                            continue;
                        }
                        else
                        {
                            if (type_header == false)
                            {
                                printf("Use of data without header first <%s>.\n", line.c_str());
                                return false;
                            }
                            if (type_header == true)
                            {
                                if (class_name == _class)
                                {
                                    if (header_name == header)
                                    {
                                        return true;
                                    }
                                }
                            }
                            printf("Unknown command <%s>.\n", line.c_str());
                            return false;
                        }
                    }
                    return false;
                }
            }
            else
            {
                printf("Use of find_ldb without initializing it first <init_ldb>.\n");
                return false;
            }
            return false;
        }
        void run(string query)
        {
            if (loaded_db == true)
            {
                if (starts_with(query, "#"))
                {
                    return;
                }
                else if (starts_with(query, "header "))
                {
                    if (in_header == true)
                    {
                        printf("Cannot stack headers <%s>.\n", query.c_str());
                        return;
                    }
                    in_header = true;
                    headers.push_back(query);
                    raw += spaces + query + "\n";
                    spaces += " ";
                }
                else if (starts_with(query, "class "))
                {
                    if (in_class == true)
                    {
                        printf("Cannot stack classes <%s>.\n", query.c_str());
                        return;
                    }
                    in_class = true;
                    classes.push_back(query);
                    raw += spaces + query + "\n";
                    spaces += " ";
                }
                else if (query == "end")
                {
                    if (in_header == true)
                    {
                        raw += spaces + ".\n";
                        spaces.replace(spaces.find_last_of(" "), spaces.length() - spaces.find_last_of(" "), "");
                        in_header = false;
                    }
                    if (in_class == true)
                    {
                        raw += spaces + ".\n";
                        spaces.replace(spaces.find_last_of(" "), spaces.length() - spaces.find_last_of(" "), "");
                        in_header = false;
                    }
                    else
                    {
                        printf("Use of end without header first <%s>.\n", query.c_str());
                        return;
                    }
                }
                else
                {
                    if (in_header == true)
                    {
                        raw += spaces + query + "\n";
                    }
                    else
                    {
                        printf("Invalid query <%s>.\n", query.c_str());
                        return;
                    }
                }
            }
            else
            {
                printf("Use of run without initializing the file first <%s>.\n", query.c_str());
                return;
            }
        }
        void save()
        {
            if (loaded_db == true)
            {
                if (in_class == true)
                {
                    printf("Use of save_ldb without using end first <class>.\n");
                    return;
                }
                if (in_header == true)
                {
                    printf("Use of save_ldb without using end first <header>.\n");
                    return;
                }
                ofstream data(db_name + ".ldb");
                data << raw;
                data.close();
            }
            else
            {
                printf("Use of save_ldb without initializing the file first <save>.\n");
                return;
            }
        }
};