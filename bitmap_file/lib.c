#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef unsigned int color_t;

struct Bitmap
{
    int width;
    int height;
    color_t *pixels;
};

struct Bitmap init_bitmap(int width, int height)
{
    return (struct Bitmap){
        .height = height,
        .width = width,
        .pixels = (color_t *)malloc(width * height * sizeof(color_t)),
    };
}

void set_pixel(struct Bitmap bitmap, int x, int y, color_t color)
{
    if (x < 0 || x >= bitmap.width || y < 0 || y >= bitmap.height)
    {
        return;
    }
    bitmap.pixels[bitmap.width * y + x] = color;
}

int calc_row_length(int width)
{
    int row_length = width * 3;
    // Add padding to make a 4-byte chunk
    row_length += (4 - row_length % 4);
    assert(row_length % 4 == 0);
    return row_length;
}

int calc_image_size(int width, int height)
{
    return calc_row_length(width) * height;
}

void write_file_header(FILE *file, int width, int height)
{
    int file_size = 14 + 40 + calc_image_size(width, height);
    int reserved = 0;
    int offset = 14 + 40;

    fputs("BM", file);
    fwrite(&file_size, 4, 1, file);
    fwrite(&reserved, 4, 1, file);
    fwrite(&offset, 4, 1, file);
}

void write_info_header(FILE *file, int width, int height)
{
    int header_size = 40;
    int planes = 1;
    int bits_per_pixel = 24;
    int compression = 0;
    int image_size = calc_image_size(width, height);
    int x_pixels_per_meter = 0;
    int y_pixels_per_meter = 0;
    int colors_used = 0;
    int important_colors_used = 0;

    fwrite(&header_size, 4, 1, file);
    fwrite(&width, 4, 1, file);
    fwrite(&height, 4, 1, file);
    fwrite(&planes, 2, 1, file);
    fwrite(&bits_per_pixel, 2, 1, file);
    fwrite(&compression, 4, 1, file);
    fwrite(&image_size, 4, 1, file);
    fwrite(&x_pixels_per_meter, 4, 1, file);
    fwrite(&y_pixels_per_meter, 4, 1, file);
    fwrite(&colors_used, 4, 1, file);
    fwrite(&important_colors_used, 4, 1, file);
}

void write_pixels(FILE *file, struct Bitmap bitmap)
{
    int row_length = calc_row_length(bitmap.width);
    for (int y = bitmap.height - 1; y >= 0; y--)
    {
        for (int x = 0; x < bitmap.width; x++)
        {
            color_t color = bitmap.pixels[bitmap.width * y + x];

            unsigned char r = (color & 0xFF0000) >> 16;
            unsigned char g = (color & 0x00FF00) >> 8;
            unsigned char b = (color & 0x0000FF);

            fwrite(&b, 1, 1, file);
            fwrite(&g, 1, 1, file);
            fwrite(&r, 1, 1, file);
        }
        for (int i = bitmap.width * 3; i < row_length; i++)
        {
            fputc(0, file);
        }
    }
}

void write_bitmap(struct Bitmap bitmap, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        return;
    }

    write_file_header(file, bitmap.width, bitmap.height);
    write_info_header(file, bitmap.width, bitmap.height);
    write_pixels(file, bitmap);

    fclose(file);
}

void fill_bitmap(struct Bitmap bitmap, color_t color, int x, int y, int width, int height)
{
    for (int i = y; i < y + height && i < bitmap.height; i++) {
        for (int j = x; j < x + width && j < bitmap.width; j++) {
            set_pixel(bitmap, j, i, color);
        }
    }
}

void free_bitmap(struct Bitmap bitmap)
{
    free(bitmap.pixels);
    bitmap.pixels = NULL;
}