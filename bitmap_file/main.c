#include "lib.c"

void main()
{
    struct Bitmap bitmap = init_bitmap(10, 10);

    fill_bitmap(bitmap, 0xFFAABB, 0, 0, 2, 2);
    fill_bitmap(bitmap, 0xFFAACC, 8, 0, 2, 2);
    fill_bitmap(bitmap, 0xFFAABB, 0, 8, 2, 2);
    fill_bitmap(bitmap, 0xFFAABB, 8, 8, 2, 2);

    fill_bitmap(bitmap, 0x0FACBE, 4, 4, 2, 2);


    write_bitmap(bitmap, "output.bmp");

    free_bitmap(bitmap);
}