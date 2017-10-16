#include <iostream>
#include "compiler_helper.h"
#include "mandelbrot.hpp"
#include "config_file.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "\t" << argv[0] << " FILE" << std::endl;

        return EXIT_FAILURE;
    }

    config_file cf(argv[1]);

    mandelbrot<double> m(cf.width, cf.height, cf.xmin, cf.xmax, cf.ymin, cf.ymax);

    if(cf.red_function != nullptr)
        m.red_function = cf.red_function;
    if(cf.blue_function != nullptr)
        m.blue_function = cf.blue_function;
    if(cf.green_function != nullptr)
        m.green_function = cf.green_function;

    m.max_iterations = cf.max_iterations;

    m.apply_filter_bc(cf.brightness, cf.contrast);

    int tot_pixel = cf.width * cf.height;

    m.start("mandelbrot.png", 4, [&tot_pixel](int p) {
		std::cout << " " << (float)p / tot_pixel * 100 << " %                                \r" << std::flush;
	});

    std::cout << "Completed.                                      \n" << std::endl;

    return EXIT_SUCCESS;
}
