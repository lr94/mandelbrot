#ifndef PNG_BITMAP_H
#define PNG_BITMAP_H

#include <cstdint>
#include <string>

struct pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class png_bitmap
{
	private:
		int width;
		int height;

	public:
		png_bitmap(int width, int height);
		~png_bitmap();

		int get_width();
		int get_height();

		void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void set_pixel(int x, int y, struct pixel color);
		struct pixel get_pixel(int x, int y);

		int save(std::string filename);

		uint32_t *data; // ARGB
};

#endif
