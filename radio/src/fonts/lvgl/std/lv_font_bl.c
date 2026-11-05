/*******************************************************************************
 * Size: 11 px
 * Bpp: 1
 * Opts: --no-prefilter --bpp 1 --size 11 --no-compress --font ../DedSecPixel/Silkscreen-Regular.ttf -r 0x20-0x7E --font ../../thirdparty/lvgl/scripts/built_in_font/FontAwesome5-Solid+Brands+Regular.woff -r 61671,63650,63426,61453,61787,61452,61931,62087 --format lvgl -o std/lv_font_bl.c --force-fast-kern-format --no-compress
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LV_FONT_BL
#define LV_FONT_BL 1
#endif

#if LV_FONT_BL

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xf2,

    /* U+0022 "\"" */
    0x99, 0x90,

    /* U+0023 "#" */
    0x24, 0x4b, 0xf9, 0x2f, 0xe4, 0x89, 0x0,

    /* U+0024 "$" */
    0x10, 0x8e, 0x8, 0x38, 0x22, 0xf1, 0x8,

    /* U+0025 "%" */
    0xe9, 0xd3, 0xa0, 0x82, 0xe5, 0xcb, 0x80,

    /* U+0026 "&" */
    0x10, 0x9f, 0x8, 0x3a, 0x10, 0x78, 0x84,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x32, 0x48, 0x88,

    /* U+0029 ")" */
    0x8c, 0x92, 0x20,

    /* U+002A "*" */
    0x10, 0x22, 0x4b, 0xc9, 0x62, 0x4, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x0,

    /* U+002C "," */
    0x30,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x8, 0x42, 0x48, 0x42, 0x0,

    /* U+0030 "0" */
    0x74, 0x63, 0x18, 0xc9, 0xc0,

    /* U+0031 "1" */
    0xe2, 0x22, 0x22, 0xf0,

    /* U+0032 "2" */
    0xf0, 0x82, 0xe8, 0x43, 0xe0,

    /* U+0033 "3" */
    0xf0, 0x82, 0xe0, 0x83, 0xc0,

    /* U+0034 "4" */
    0x94, 0xa5, 0xf1, 0x8, 0x40,

    /* U+0035 "5" */
    0xfc, 0x21, 0xe0, 0x83, 0xc0,

    /* U+0036 "6" */
    0x74, 0x21, 0xe8, 0xc9, 0xc0,

    /* U+0037 "7" */
    0xfc, 0x10, 0x46, 0x61, 0x4, 0x0,

    /* U+0038 "8" */
    0x74, 0xa2, 0xe8, 0xc9, 0xc0,

    /* U+0039 "9" */
    0x74, 0x62, 0xf0, 0x89, 0xc0,

    /* U+003A ":" */
    0x90,

    /* U+003B ";" */
    0x20, 0x88,

    /* U+003C "<" */
    0x8, 0x9, 0x6, 0x8, 0x20,

    /* U+003D "=" */
    0xf0, 0xf,

    /* U+003E ">" */
    0x82, 0x8, 0x32, 0x2, 0x0,

    /* U+003F "?" */
    0xf8, 0x42, 0x70, 0x0, 0x80,

    /* U+0040 "@" */
    0x79, 0x2e, 0x54, 0xc8, 0x10, 0x1e, 0x0,

    /* U+0041 "A" */
    0x74, 0xa3, 0xf8, 0xc6, 0x20,

    /* U+0042 "B" */
    0xf4, 0x63, 0xf8, 0xcb, 0xc0,

    /* U+0043 "C" */
    0x70, 0xa3, 0x8, 0xc1, 0xc0,

    /* U+0044 "D" */
    0xf4, 0x63, 0x18, 0xcb, 0xc0,

    /* U+0045 "E" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0046 "F" */
    0xf8, 0x8f, 0x88, 0x80,

    /* U+0047 "G" */
    0x7c, 0x21, 0x78, 0x81, 0xc0,

    /* U+0048 "H" */
    0x8c, 0x63, 0xf8, 0xc6, 0x20,

    /* U+0049 "I" */
    0xfe,

    /* U+004A "J" */
    0x8, 0x42, 0x18, 0xc1, 0xc0,

    /* U+004B "K" */
    0x8c, 0x25, 0xc9, 0x4a, 0x20,

    /* U+004C "L" */
    0x88, 0x88, 0x88, 0xf0,

    /* U+004D "M" */
    0x83, 0x7, 0xbc, 0x98, 0x30, 0x60, 0x80,

    /* U+004E "N" */
    0x83, 0x7, 0x8c, 0x98, 0xf0, 0x60, 0x80,

    /* U+004F "O" */
    0x74, 0x63, 0x18, 0xc9, 0xc0,

    /* U+0050 "P" */
    0xf4, 0xa3, 0xe8, 0x42, 0x0,

    /* U+0051 "Q" */
    0x74, 0x63, 0x18, 0xc9, 0xc1,

    /* U+0052 "R" */
    0xf4, 0xa3, 0xe9, 0x4a, 0x20,

    /* U+0053 "S" */
    0x78, 0x20, 0xe0, 0x83, 0xc0,

    /* U+0054 "T" */
    0xf2, 0x22, 0x22, 0x20,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc1, 0xc0,

    /* U+0056 "V" */
    0x83, 0x6, 0xb, 0x42, 0x85, 0x4, 0x0,

    /* U+0057 "W" */
    0x83, 0x26, 0x4c, 0x99, 0x2a, 0xa, 0x0,

    /* U+0058 "X" */
    0x82, 0x88, 0xa0, 0x82, 0x80, 0xa0, 0x80,

    /* U+0059 "Y" */
    0x82, 0x80, 0xa0, 0x81, 0x2, 0x4, 0x0,

    /* U+005A "Z" */
    0xf8, 0x42, 0x48, 0x43, 0xe0,

    /* U+005B "[" */
    0xea, 0xac,

    /* U+005C "\\" */
    0x84, 0x20, 0xc1, 0x84, 0x20,

    /* U+005D "]" */
    0xd5, 0x5c,

    /* U+005E "^" */
    0x22, 0xa2,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0x88, 0x80,

    /* U+0061 "a" */
    0x74, 0xa3, 0xf8, 0xc6, 0x20,

    /* U+0062 "b" */
    0xf4, 0x63, 0xf8, 0xcb, 0xc0,

    /* U+0063 "c" */
    0x70, 0xa3, 0x8, 0xc1, 0xc0,

    /* U+0064 "d" */
    0xf4, 0x63, 0x18, 0xcb, 0xc0,

    /* U+0065 "e" */
    0xf8, 0x8f, 0x88, 0xf0,

    /* U+0066 "f" */
    0xf8, 0x8f, 0x88, 0x80,

    /* U+0067 "g" */
    0x7c, 0x21, 0x78, 0x81, 0xc0,

    /* U+0068 "h" */
    0x8c, 0x63, 0xf8, 0xc6, 0x20,

    /* U+0069 "i" */
    0xfe,

    /* U+006A "j" */
    0x8, 0x42, 0x18, 0xc1, 0xc0,

    /* U+006B "k" */
    0x8c, 0x25, 0xc9, 0x4a, 0x20,

    /* U+006C "l" */
    0x88, 0x88, 0x88, 0xf0,

    /* U+006D "m" */
    0x83, 0x7, 0xbc, 0x98, 0x30, 0x60, 0x80,

    /* U+006E "n" */
    0x83, 0x7, 0x8c, 0x98, 0xf0, 0x60, 0x80,

    /* U+006F "o" */
    0x74, 0x63, 0x18, 0xc9, 0xc0,

    /* U+0070 "p" */
    0xf4, 0xa3, 0xe8, 0x42, 0x0,

    /* U+0071 "q" */
    0x74, 0x63, 0x18, 0xc9, 0xc1,

    /* U+0072 "r" */
    0xf4, 0xa3, 0xe9, 0x4a, 0x20,

    /* U+0073 "s" */
    0x78, 0x20, 0xe0, 0x83, 0xc0,

    /* U+0074 "t" */
    0xf2, 0x22, 0x22, 0x20,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xc1, 0xc0,

    /* U+0076 "v" */
    0x83, 0x6, 0xb, 0x42, 0x85, 0x4, 0x0,

    /* U+0077 "w" */
    0x83, 0x26, 0x4c, 0x99, 0x2a, 0xa, 0x0,

    /* U+0078 "x" */
    0x82, 0x88, 0xa0, 0x82, 0x80, 0xa0, 0x80,

    /* U+0079 "y" */
    0x82, 0x80, 0xa0, 0x81, 0x2, 0x4, 0x0,

    /* U+007A "z" */
    0xf8, 0x42, 0x48, 0x43, 0xe0,

    /* U+007B "{" */
    0x32, 0x2c, 0x62, 0x30,

    /* U+007C "|" */
    0xff, 0xc0,

    /* U+007D "}" */
    0xc4, 0x43, 0x64, 0xc0,

    /* U+007E "~" */
    0x2b, 0x24,

    /* U+F00C "" */
    0x0, 0x60, 0x1c, 0x7, 0x71, 0xcf, 0x70, 0xfc,
    0xf, 0x0, 0xc0,

    /* U+F00D "" */
    0xc3, 0xe7, 0x7e, 0x3c, 0x7e, 0xe7, 0xc3,

    /* U+F0E7 "" */
    0x78, 0xf3, 0xc7, 0xff, 0xff, 0x86, 0xc, 0x30,
    0x60, 0x80,

    /* U+F15B "" */
    0xfa, 0xfb, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff,

    /* U+F1EB "" */
    0x0, 0x0, 0x7f, 0x87, 0x87, 0xb8, 0x7, 0xf,
    0xc0, 0x61, 0x80, 0x0, 0x0, 0xc0, 0x7, 0x0,
    0xc, 0x0,

    /* U+F287 "" */
    0x1, 0x80, 0x1e, 0xe, 0x80, 0xbf, 0xff, 0xe2,
    0x8, 0x8, 0x0, 0x16, 0x0, 0x38,

    /* U+F7C2 "" */
    0x3f, 0x55, 0xd5, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff,

    /* U+F8A2 "" */
    0x0, 0x24, 0xd, 0x81, 0xff, 0xf6, 0x0, 0x40,
    0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 88, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 66, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 110, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 4, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 132, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 18, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 25, .adv_w = 132, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 32, .adv_w = 66, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 33, .adv_w = 88, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 88, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 88, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 54, .adv_w = 110, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 55, .adv_w = 66, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 80, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 132, .box_w = 6, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 66, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 112, .adv_w = 88, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 110, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 121, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 138, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 148, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 66, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 132, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 220, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 246, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 260, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 88, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 279, .adv_w = 88, .box_w = 2, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 110, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 283, .adv_w = 132, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 284, .adv_w = 88, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 286, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 301, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 314, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 319, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 324, .adv_w = 66, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 330, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 339, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 346, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 353, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 132, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 368, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 373, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 132, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 110, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 420, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 66, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 426, .adv_w = 110, .box_w = 4, .box_h = 7, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 132, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 432, .adv_w = 176, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 443, .adv_w = 121, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 450, .adv_w = 110, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 460, .adv_w = 132, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 470, .adv_w = 220, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 488, .adv_w = 220, .box_w = 14, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 502, .adv_w = 132, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 512, .adv_w = 177, .box_w = 11, .box_h = 6, .ofs_x = 0, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x1, 0xdb, 0x14f, 0x1df, 0x27b, 0x7b6, 0x896
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 61452, .range_length = 2199, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 8, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_font_bl = {
#else
lv_font_t lv_font_bl = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_BL*/

