#ifndef PNG_BITMAP_H
#define PNG_BITMAP_H

#include <cstdint>
#include <string>

class png_bitmap
{
	private:
		int width;
		int height;
		uint32_t *data;

	public:
		png_bitmap(int width, int height);
		~png_bitmap();

		int get_width();
		int get_height();

		void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		int save(std::string filename);
};

#endif
