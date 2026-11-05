/*******************************************************************************
 * Size: 13 px
 * Bpp: 1
 * Opts: --no-prefilter --bpp 1 --size 13 --no-compress --font ../DedSecPixel/Tiny5-Regular.ttf -r 0x20-0x7F --font ../../thirdparty/lvgl/scripts/built_in_font/FontAwesome5-Solid+Brands+Regular.woff -r 61671,63650,63426,61453,61787,61452,61931,62087 --format lvgl -o sml/lv_font_bl.c --force-fast-kern-format --no-compress
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
    0xff, 0xf3, 0xc0,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xb0,

    /* U+0023 "#" */
    0x26, 0x26, 0xff, 0xff, 0x26, 0xff, 0xff, 0x26,
    0x26,

    /* U+0024 "$" */
    0x20, 0x82, 0xcb, 0xe3, 0x80, 0xf8, 0xe0, 0x82,
    0x0,

    /* U+0025 "%" */
    0xc7, 0x8c, 0x61, 0x2, 0x18, 0xf1, 0x80,

    /* U+0026 "&" */
    0x20, 0x43, 0x66, 0xc2, 0x1b, 0x36, 0x13, 0x26,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x3b, 0x6d, 0xb6, 0xd8, 0x90,

    /* U+0029 ")" */
    0xc4, 0x92, 0x49, 0x27, 0x60,

    /* U+002A "*" */
    0x20, 0x8e, 0xfb, 0x20, 0x8c, 0xf3,

    /* U+002B "+" */
    0x20, 0x8e, 0xfb, 0x20, 0x80,

    /* U+002C "," */
    0xff,

    /* U+002D "-" */
    0xef, 0xb0,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x24, 0x9d, 0xb6, 0xc0,

    /* U+0030 "0" */
    0x20, 0x8c, 0xf3, 0xcf, 0x3c, 0xc8, 0x20,

    /* U+0031 "1" */
    0x27, 0xf2, 0x49, 0x20,

    /* U+0032 "2" */
    0xe3, 0x80, 0xc3, 0x23, 0xc, 0x3b, 0xec,

    /* U+0033 "3" */
    0xef, 0xb0, 0xc3, 0x20, 0x30, 0xf8, 0xe0,

    /* U+0034 "4" */
    0xc, 0x32, 0xcb, 0xcf, 0xbe, 0xc3, 0xc,

    /* U+0035 "5" */
    0xef, 0xbc, 0x30, 0xe0, 0x30, 0xf8, 0xe0,

    /* U+0036 "6" */
    0x2c, 0xbc, 0x30, 0xe3, 0x3c, 0xc8, 0x20,

    /* U+0037 "7" */
    0xef, 0xb0, 0xc3, 0x20, 0x82, 0x8, 0x20,

    /* U+0038 "8" */
    0x20, 0x8c, 0xf3, 0x23, 0x3c, 0xc8, 0x20,

    /* U+0039 "9" */
    0x20, 0x8c, 0xf3, 0x2c, 0x30, 0xf8, 0xe0,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0xf0, 0x3f, 0xc0,

    /* U+003C "<" */
    0xc, 0x32, 0x8, 0xc0, 0x82, 0x3, 0xc,

    /* U+003D "=" */
    0xef, 0xb0, 0x0, 0xef, 0xb0,

    /* U+003E ">" */
    0xc3, 0x2, 0x8, 0xc, 0x82, 0x30, 0xc0,

    /* U+003F "?" */
    0xe3, 0x80, 0xc3, 0x20, 0x80, 0x8, 0x20,

    /* U+0040 "@" */
    0x38, 0x73, 0x1e, 0x3d, 0xfb, 0xf7, 0xe0, 0xc0,
    0x70, 0xe0,

    /* U+0041 "A" */
    0x38, 0x73, 0x1e, 0x3f, 0xf8, 0xf1, 0xe3, 0xc6,

    /* U+0042 "B" */
    0xf9, 0xf3, 0x1e, 0x3f, 0x98, 0xf1, 0xfc, 0xf8,

    /* U+0043 "C" */
    0x3e, 0x7f, 0x6, 0xc, 0x18, 0x30, 0x1f, 0x3e,

    /* U+0044 "D" */
    0xf9, 0xf3, 0x1e, 0x3c, 0x78, 0xf1, 0xfc, 0xf8,

    /* U+0045 "E" */
    0xff, 0xff, 0x6, 0xf, 0x98, 0x30, 0x7f, 0xfe,

    /* U+0046 "F" */
    0xff, 0xff, 0x6, 0xf, 0x98, 0x30, 0x60, 0xc0,

    /* U+0047 "G" */
    0x3e, 0x7f, 0x6, 0xd, 0xf8, 0xf1, 0x9f, 0x3e,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3f, 0xf8, 0xf1, 0xe3, 0xc6,

    /* U+0049 "I" */
    0xff, 0xff, 0xc0,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x78, 0xf1, 0x9c, 0x38,

    /* U+004B "K" */
    0xc7, 0x8f, 0x66, 0xce, 0x1b, 0x36, 0x63, 0xc6,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x7f, 0xfe,

    /* U+004D "M" */
    0xc1, 0xc1, 0xe7, 0xe7, 0xd9, 0xc1, 0xc1, 0xc1,
    0xc1,

    /* U+004E "N" */
    0xc7, 0x8f, 0x9f, 0x3d, 0xf8, 0xf1, 0xe3, 0xc6,

    /* U+004F "O" */
    0x38, 0x73, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,

    /* U+0050 "P" */
    0xf9, 0xf3, 0x1e, 0x3f, 0x98, 0x30, 0x60, 0xc0,

    /* U+0051 "Q" */
    0x38, 0x73, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,
    0x3c, 0x78,

    /* U+0052 "R" */
    0xf9, 0xf3, 0x1e, 0x3f, 0x98, 0xf1, 0xe3, 0xc6,

    /* U+0053 "S" */
    0x3e, 0x7f, 0x6, 0x3, 0x80, 0xc1, 0xfc, 0xf8,

    /* U+0054 "T" */
    0xef, 0xb2, 0x8, 0x20, 0x82, 0x8, 0x20,

    /* U+0055 "U" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0x9c, 0x38,

    /* U+0056 "V" */
    0xc1, 0xc1, 0xc1, 0xc1, 0x26, 0x26, 0x26, 0x18,
    0x18,

    /* U+0057 "W" */
    0xd9, 0xd9, 0xd9, 0xd9, 0xd9, 0x26, 0x26, 0x26,
    0x26,

    /* U+0058 "X" */
    0xc1, 0xc1, 0x26, 0x26, 0x18, 0x26, 0x26, 0xc1,
    0xc1,

    /* U+0059 "Y" */
    0xc1, 0xc1, 0x26, 0x26, 0x18, 0x18, 0x18, 0x18,
    0x18,

    /* U+005A "Z" */
    0xff, 0xfc, 0x60, 0xc2, 0x18, 0x30, 0x7f, 0xfe,

    /* U+005B "[" */
    0xfb, 0x6d, 0xb6, 0xdb, 0xf0,

    /* U+005C "\\" */
    0xdb, 0x62, 0x49, 0x20,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x27, 0xf0,

    /* U+005E "^" */
    0x20, 0x8c, 0xf3,

    /* U+005F "_" */
    0xef, 0xb0,

    /* U+0060 "`" */
    0xd8, 0x90,

    /* U+0061 "a" */
    0x2c, 0xbc, 0xf3, 0xcc, 0xb2, 0xc0,

    /* U+0062 "b" */
    0xc3, 0xe, 0x38, 0xcf, 0x3c, 0xf8, 0xe0,

    /* U+0063 "c" */
    0x2c, 0xbc, 0x30, 0xc0, 0xb2, 0xc0,

    /* U+0064 "d" */
    0xc, 0x32, 0xcb, 0xcf, 0x3c, 0xcb, 0x2c,

    /* U+0065 "e" */
    0x20, 0x8e, 0xf0, 0xc0, 0xb2, 0xc0,

    /* U+0066 "f" */
    0x27, 0x6f, 0xb6, 0xc0,

    /* U+0067 "g" */
    0x2c, 0xbc, 0xf3, 0xcc, 0xb2, 0xf8, 0xe0,

    /* U+0068 "h" */
    0xc3, 0xe, 0x38, 0xcf, 0x3c, 0xf3, 0xcc,

    /* U+0069 "i" */
    0xf0, 0xff, 0xfc,

    /* U+006A "j" */
    0x24, 0x12, 0x49, 0x27, 0x60,

    /* U+006B "k" */
    0xc3, 0xc, 0xf3, 0xe3, 0x3c, 0xf3, 0xcc,

    /* U+006C "l" */
    0xff, 0xff, 0xc0,

    /* U+006D "m" */
    0xfe, 0xfe, 0xd9, 0xd9, 0xd9, 0xd9, 0xd9,

    /* U+006E "n" */
    0xe3, 0x8c, 0xf3, 0xcf, 0x3c, 0xc0,

    /* U+006F "o" */
    0x20, 0x8c, 0xf3, 0xcc, 0x82, 0x0,

    /* U+0070 "p" */
    0xe3, 0x8c, 0xf3, 0xcf, 0x8e, 0x30, 0xc0,

    /* U+0071 "q" */
    0x2c, 0xbc, 0xf3, 0xcc, 0xb2, 0xc3, 0xc,

    /* U+0072 "r" */
    0xff, 0x6d, 0xb0,

    /* U+0073 "s" */
    0x2c, 0xbe, 0x3, 0xf, 0x8e, 0x0,

    /* U+0074 "t" */
    0xdb, 0xfd, 0xb1, 0x20,

    /* U+0075 "u" */
    0xcf, 0x3c, 0xf3, 0xcc, 0xb2, 0xc0,

    /* U+0076 "v" */
    0xcf, 0x3c, 0xc8, 0x20, 0x82, 0x0,

    /* U+0077 "w" */
    0xd9, 0xd9, 0xd9, 0x26, 0x26, 0x26, 0x26,

    /* U+0078 "x" */
    0xcf, 0x32, 0x33, 0xcf, 0x3c, 0xc0,

    /* U+0079 "y" */
    0xcf, 0x3c, 0xf3, 0xcc, 0x82, 0x30, 0xc0,

    /* U+007A "z" */
    0xef, 0xb2, 0x30, 0xc3, 0xbe, 0xc0,

    /* U+007B "{" */
    0x2c, 0x82, 0x8, 0x23, 0xc, 0x8, 0x20, 0x82,
    0xcb,

    /* U+007C "|" */
    0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xe0, 0x82, 0x8, 0x20, 0x30, 0xc8, 0x20, 0x8e,
    0x38,

    /* U+007E "~" */
    0x26, 0x4f, 0x66, 0xc0,

    /* U+F00C "" */
    0x0, 0x10, 0x1, 0xc0, 0x1e, 0x1, 0xee, 0x1e,
    0x79, 0xe1, 0xfe, 0x7, 0xe0, 0x1e, 0x0, 0x60,
    0x0,

    /* U+F00D "" */
    0x41, 0x71, 0xdd, 0xc7, 0xc1, 0xc1, 0xf1, 0xdd,
    0xc7, 0x41, 0x0,

    /* U+F0E7 "" */
    0x7c, 0x78, 0x78, 0xf8, 0xff, 0xff, 0xfe, 0x1c,
    0x1c, 0x18, 0x18, 0x10, 0x0,

    /* U+F15B "" */
    0xfd, 0x3f, 0x6f, 0xdf, 0xf0, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0,

    /* U+F1EB "" */
    0xf, 0xf0, 0x3f, 0xfc, 0x78, 0x1e, 0xe0, 0x7,
    0x47, 0xe2, 0x1f, 0xf8, 0x1c, 0x38, 0x0, 0x0,
    0x1, 0x80, 0x3, 0xc0, 0x1, 0x80,

    /* U+F287 "" */
    0x0, 0xc0, 0x3, 0xc0, 0x4, 0x0, 0xe4, 0x6,
    0xff, 0xff, 0xe1, 0x6, 0x1, 0x0, 0x0, 0xb0,
    0x0, 0x70,

    /* U+F7C2 "" */
    0x1f, 0xca, 0xb6, 0xaf, 0xab, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0,

    /* U+F8A2 "" */
    0x0, 0x8, 0x0, 0xcc, 0x6, 0xe0, 0x3f, 0xff,
    0xff, 0xfd, 0xc0, 0x6, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 52, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 104, .box_w = 5, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 7, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 104, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 25, .adv_w = 130, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 32, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 40, .adv_w = 52, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 41, .adv_w = 78, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 46, .adv_w = 78, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 51, .adv_w = 104, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 57, .adv_w = 104, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 62, .adv_w = 52, .box_w = 2, .box_h = 4, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 63, .adv_w = 104, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 65, .adv_w = 52, .box_w = 2, .box_h = 2, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 66, .adv_w = 78, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 70, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 104, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 81, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 102, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 109, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 52, .box_w = 2, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 142, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 104, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 154, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 168, .adv_w = 130, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 178, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 186, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 210, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 234, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 242, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 253, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 269, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 278, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 302, .adv_w = 130, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 312, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 343, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 361, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 370, .adv_w = 156, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 78, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 392, .adv_w = 78, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 396, .adv_w = 78, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 401, .adv_w = 104, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 404, .adv_w = 104, .box_w = 6, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 406, .adv_w = 78, .box_w = 3, .box_h = 4, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 408, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 427, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 440, .adv_w = 78, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 444, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 451, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 458, .adv_w = 52, .box_w = 2, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 78, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 466, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 156, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 483, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 495, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 502, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 509, .adv_w = 78, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 512, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 78, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 528, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 534, .adv_w = 156, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 104, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 554, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 104, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 569, .adv_w = 52, .box_w = 2, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 572, .adv_w = 104, .box_w = 6, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 581, .adv_w = 130, .box_w = 7, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 585, .adv_w = 208, .box_w = 13, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 602, .adv_w = 143, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 130, .box_w = 8, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 626, .adv_w = 156, .box_w = 10, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 643, .adv_w = 260, .box_w = 16, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 665, .adv_w = 260, .box_w = 16, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 683, .adv_w = 156, .box_w = 10, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 700, .adv_w = 209, .box_w = 13, .box_h = 8, .ofs_x = 0, .ofs_y = 1}
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

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 2, 2, 0, 3,
    0, 0, 0, 2, 0, 1, 0, 0,
    2, 4, 0, 0, 2, 3, 2, 3,
    5, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 6, 0, 0, 7, 2,
    0, 7, 0, 0, 0, 0, 7, 7,
    6, 6, 0, 4, 2, 0, 0, 5,
    5, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 3,
    4, 0, 3, 0, 0, 0, 0, 0,
    0, 0, 5, 0, 5, 5, 5, 0,
    5, 0, 0, 6, 0, 0, 0, 0,
    5, 0, 5, 0, 5, 0, 0, 4,
    4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 0, -26, -26, 0, 0, 0, 0,
    0, 0, 0, -26, -26, -26, 0, 0,
    -26, -26, -26, -26, 0, 0, 0, -26,
    -26, 0, 0, 0, 0, -26, 0, 0,
    -26, 0, 0, -26, 0, 0, -26, 0,
    0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 7,
    .right_class_cnt     = 6,
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
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 1,
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
    .line_height = 13,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LV_FONT_BL*/

