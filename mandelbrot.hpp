#ifndef MANDELBROT_H
#define MANDELBROT_H
#include <math.h>
#include <expreval.h>
#include <thread>
#include <mutex>
#include "png_bitmap.hpp"

static double default_color_function(double num_iterations);

template <typename R>
class mandelbrot
{
private:
    R xmin;
    R xmax;
    R ymin;
    R ymax;

    // From 0 to 255 (use char?)
    int contrast = 0;
    int brightness = 0;

    int width;
    int height;

    png_bitmap *img;

    int pixel_counter;
    std::mutex pixel_counter_mutex;
    int progress_step;

    inline int truncate(int val)
    {
        if(val < 0)
            return 0;
        if(val > 255)
            return 255;

        return val;
    }

    template <typename T>
    void render(int starting_column, int step, T progress_callback, int enable_callback)
    {
        int count = 0;

        for(int xp = starting_column; xp <= width; xp += step)
        {
            for(int yp = 0; yp < height; yp++)
            {
                R x = xmin + (xmax - xmin) / width * xp;
                R y = ymin + (ymax - ymin) / height * (height - yp - 1);

                R fx = 0.0;
                R fy = 0.0;

                int flag = 1;
                int i;
                for(i = 0; i < max_iterations; i++)
                {
                    R tmp = fx;
                    fx = fx * fx - fy * fy + x;
                    fy = tmp * fy * 2 + y;

                    if(fx * fx + fy * fy > 4) // |z| > 2
                    {
                        flag = 0;
                        break;
                    }
                }

                if(flag)
                    img->set_pixel(xp, yp, 0, 0, 0, 255);
                else
                {
                    uint8_t vr = (uint8_t)(red_function((double)i) * 255);
                    uint8_t vg = (uint8_t)(green_function((double)i) * 255);
                    uint8_t vb = (uint8_t)(blue_function((double)i) * 255);

                    float factor = (129.0 * (contrast + 127.0)) / (127.0 * (129.0 - contrast));
                    vr = (uint8_t)truncate(factor * (vr - 128) + 128 + brightness);
                    vg = (uint8_t)truncate(factor * (vg - 128) + 128 + brightness);
                    vb = (uint8_t)truncate(factor * (vb - 128) + 128 + brightness);

                    img->set_pixel(xp, yp, vr, vg, vb, 255);
                }

                count++;

                if(count % progress_step == 0)
                {
                    pixel_counter_mutex.lock();
                    pixel_counter += count;
                    count = 0;
                    if(enable_callback)
                        progress_callback(pixel_counter);
                    pixel_counter_mutex.unlock();
                }
            }
        }
    }

public:
    mandelbrot(int width, int height, R xmin, R xmax, R ymin, R ymax)
    {
        this->width = width;
        this->height = height;
        this->xmin = xmin;
        this->xmax = xmax;
        this->ymin = ymin;
        this->ymax = ymax;

        progress_step = width * height / 10000;
    }

    cfunction1 red_function = default_color_function;
    cfunction1 green_function = default_color_function;
    cfunction1 blue_function = default_color_function;
    int max_iterations = 250000;

    void apply_filter_bc(float brightness, float contrast)
    {
        this->brightness = (int)(brightness * 128);
        this->contrast = (int)(contrast * 128);
    }

    template <typename C>
    void start(std::string filename, int num_threads, C progress_callback)
    {
        img = new png_bitmap(width, height);
        pixel_counter = 0;

        std::thread threads[num_threads];

        for(int i = 0; i < num_threads; i++)
        {
            threads[i] = std::thread([&, i]() {
                this->render(i, num_threads, progress_callback, i == 0);
            });
        }

        for(int i = 0; i < num_threads; i++)
            threads[i].join();

        progress_callback(width * height);

        img->save(filename);

        delete img;
    }
};

static double default_color_function(double num_iterations)
{
    return atan(num_iterations / 20) / 1.57079632679;
}
#endif
