#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H
#include <string>
#include <expreval.h>

class config_file
{
private:
    config_file operator=(const config_file &cf);
    config_file(const config_file &cf);
public:
    enum base_type
    {
        FLOAT, DOUBLE, FLOAT128
    };
    config_file(std::string filename);

    cfunction1 red_function = nullptr;
    cfunction1 green_function = nullptr;
    cfunction1 blue_function = nullptr;

    double xmin = -2.0, xmax = 1.0, ymin = -1.0, ymax = 1.0;

    int max_iterations = 1000;

    int width = 300, height = 200;

    float brightness = 0, contrast = 0;

    config_file::base_type real_type = config_file::base_type::DOUBLE;
};
#endif
