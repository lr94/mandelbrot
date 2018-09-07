#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include "compiler_helper.h"
#include "mandelbrot.hpp"
#include "config_file.hpp"

#include <boost/multiprecision/float128.hpp>
using namespace boost::multiprecision;

static void print_usage_and_exit(char *argv[]);
static unsigned long long get_monotonic_microseconds();

template <typename T>
void do_job(config_file &cf, int threads, std::string &out_filename)
{
    mandelbrot<T> m(cf.width, cf.height, cf.xmin, cf.xmax, cf.ymin, cf.ymax);

    if(cf.red_function != nullptr)
        m.red_function = cf.red_function;
    if(cf.blue_function != nullptr)
        m.blue_function = cf.blue_function;
    if(cf.green_function != nullptr)
        m.green_function = cf.green_function;

    m.max_iterations = cf.max_iterations;

    m.apply_filter_bc(cf.brightness, cf.contrast);

    int tot_pixel = cf.width * cf.height;

    std::cout << "Starting " << threads << " threads" << std::endl;

    unsigned long long microseconds0 = get_monotonic_microseconds();

    m.start(out_filename, threads, [&tot_pixel](int p) {
		std::cout << " "
            << static_cast<float>(p) / tot_pixel * 100
            << " %                                \r" << std::flush;
	});

    unsigned long long microseconds1 = get_monotonic_microseconds();

    std::cout << "Completed.                                      " << std::endl;

    std::cout << "Image saved in " << out_filename << std::endl;

    double seconds = static_cast<double>(microseconds1 - microseconds0) / 1000000.0;
    std::cout << "Time elapsed: "
        << std::setiosflags(std::ios::fixed)
        << std::setprecision(3) << seconds
        << " s" << std::endl;

    std::cout << "Speed: "
        << std::setprecision(6)
        << static_cast<double>(tot_pixel) / seconds
        << " pixel/s" << std::endl;
}

int main(int argc, char *argv[])
{
    std::string out_filename = "mandelbrot.png";
    std::string in_filename;
    int threads = 0;

    for(int i = 1; i < argc; i++)
    {
        std::string current_arg = argv[i];
        int next_arg_index = i + 1;

        if(current_arg == "-t")
        {
            i++;
            if(next_arg_index >= argc)
                print_usage_and_exit(argv);

            try
            {
                threads = std::stoi(argv[next_arg_index]);
            }
            catch(std::invalid_argument &e)
            {
                print_usage_and_exit(argv);
            }
        }
        else if(current_arg == "-o")
        {
            i++;
            if(next_arg_index >= argc)
                print_usage_and_exit(argv);

            out_filename = argv[next_arg_index];
        }
        else if(in_filename == "")
            in_filename = argv[i];
        else
            print_usage_and_exit(argv);
    }

    if(in_filename == "")
        print_usage_and_exit(argv);

    if(threads == 0)
        threads = std::thread::hardware_concurrency();

    config_file cf(in_filename);

    switch(cf.real_type)
    {
        case config_file::base_type::FLOAT:
            do_job<float>(cf, threads, out_filename);
            break;

        case config_file::base_type::DOUBLE:
            do_job<double>(cf, threads, out_filename);
            break;

        case config_file::base_type::FLOAT128:
            do_job<float128>(cf, threads, out_filename);
            break;
    }

    return EXIT_SUCCESS;
}

static void print_usage_and_exit(char *argv[])
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << argv[0] << " CONFIG_FILE [-t threads] [-o output_file]" << std::endl;

    exit(EXIT_FAILURE);
}

static unsigned long long get_monotonic_microseconds()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return ((unsigned long long)now.tv_sec) * 1000000LL + now.tv_nsec / 1000;
}
