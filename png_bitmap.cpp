#include <cstdio>
#include <png.h>
#include "png_bitmap.hpp"

png_bitmap::png_bitmap(int width, int height)
{
    this->width = width;
    this->height = height;

    this->data = new uint32_t[height * width];
}

png_bitmap::~png_bitmap()
{
    delete[] this->data;
}

int png_bitmap::get_width()
{
    return this->width;
}

int png_bitmap::get_height()
{
    return this->height;
}

void png_bitmap::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    int i = y * this->width + x;
    this->data[i] = (a << 24) | (r << 16) | (g << 8) | b;
}

void png_bitmap::set_pixel(int x, int y, struct pixel color)
{
    this->set_pixel(x, y, color.r, color.g, color.b, color.a);
}

struct pixel png_bitmap::get_pixel(int x, int y)
{
    int i = y * this->width + x;

    struct pixel color;
    color.a = (uint8_t)((this->data[i] >> 24) & 0xff);
    color.r = (uint8_t)((this->data[i] >> 16) & 0xff);
    color.g = (uint8_t)((this->data[i] >>  8) & 0xff);
    color.b = (uint8_t)(this->data[i] & 0xff);

    return color;
}

int png_bitmap::save(std::string filename)
{
    FILE *fp = fopen(filename.c_str(), "wb");
    if(fp == nullptr)
        return -1;

    png_structp png_ptr;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if(png_ptr == nullptr)
    {
        fclose(fp);
        return -1;
    }

    png_infop info_ptr;
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == nullptr)
    {
        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, (uint32_t)this->width, (uint32_t)this->height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    uint8_t *row = new uint8_t[4 * this->width]; // RGBA
    for(int y = 0; y < this->height; y++)
    {
        for(int x = 0; x < this->width; x++)
        {
            int i = y * this->width + x;
            row[x * 4] = (uint8_t)((this->data[i] >> 16) & 0xff);;
            row[x * 4 + 1] = (uint8_t)((this->data[i] >> 8) & 0xff);
            row[x * 4 + 2] = (uint8_t)(this->data[i] & 0xff);
            row[x * 4 + 3] = (uint8_t)((this->data[i] >> 24) & 0xff);
        }

        png_write_row(png_ptr, row);
    }
    delete[] row;

    png_write_end(png_ptr, NULL);

    fclose(fp);

    return 0;
}
