#include <iostream>
#include <fstream>
#include <string>
#include <expreval.h>
#include "compiler_helper.h"
#include "config_file.hpp"

static void replace_all(std::string &str, const std::string &find, const std::string &replace);

config_file::config_file(std::string filename)
{
    std::ifstream file;
    file.open(filename);

    std::string current_line;
    while(std::getline(file, current_line))
    {
        replace_all(current_line, " ", "");
        replace_all(current_line, "\t", "");

        if(current_line.length() == 0)
            continue;

        if(current_line.at(0) == '#')
            continue;

        size_t equal_pos = current_line.find("=");
        if(equal_pos == std::string::npos || equal_pos == 0 || equal_pos == current_line.length() - 1)
            throw equal_pos; // We'll need an exception class to handle this situation

        std::string var_name = current_line.substr(0, equal_pos);
        std::string value_string = current_line.substr(equal_pos + 1, std::string::npos);

        if(var_name == "xmin")
        {
            cfunction0 f = compile0(value_string.c_str());
            xmin = f();
        }
        else if(var_name == "xmax")
        {
            cfunction0 f = compile0(value_string.c_str());
            xmax = f();
        }
        else if(var_name == "ymin")
        {
            cfunction0 f = compile0(value_string.c_str());
            ymin = f();
        }
        else if(var_name == "ymax")
        {
            cfunction0 f = compile0(value_string.c_str());
            ymax = f();
        }
        else if(var_name == "width")
        {
            cfunction0 f = compile0(value_string.c_str());
            width = (int)f();
        }
        else if(var_name == "height")
        {
            cfunction0 f = compile0(value_string.c_str());
            height = (int)f();
        }
        else if(var_name == "max_iterations")
        {
            cfunction0 f = compile0(value_string.c_str());
            max_iterations = (int)f();
        }
        else if(var_name == "red(i)")
            red_function = compile1(value_string.c_str());
        else if(var_name == "blue(i)")
            blue_function = compile1(value_string.c_str());
        else if(var_name == "green(i)")
            green_function = compile1(value_string.c_str());
        else if(var_name == "brightness")
        {
            cfunction0 f = compile0(value_string.c_str());
            brightness = f();
        }
        else if(var_name == "contrast")
        {
            cfunction0 f = compile0(value_string.c_str());
            contrast = f();
        }
        else
            throw var_name; // TODO: Implement an exception class
    }

    file.close();
}

static void replace_all(std::string &str, const std::string &find, const std::string &replace)
{
    for(size_t i = 0; i < str.length(); i += replace.length())
    {
        i = str.find(find);
        if(i == std::string::npos)
            return;
        str.replace(i, find.length(), replace);
    }
}
