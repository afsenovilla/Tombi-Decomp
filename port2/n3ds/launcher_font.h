/* The 3DS start screen's fonts (port2/tools/launcher_font.py writes launcher_font.c): DejaVu Sans, Latin-1 32..255,
   4-bit coverage, two pixels a byte, low nibble first */
#ifndef LAUNCHER_FONT_H
#define LAUNCHER_FONT_H
typedef struct { short adv, x, y, w, h; int off; } LfGlyph;   /* x, y: the bitmap's corner from the pen at the line's top */
typedef struct { int size, line; const LfGlyph *g; const unsigned char *data; } LfFont;
enum { LF_BOLD24, LF_BOLD17, LF_BOLD15, LF_BOLD13, LF_BOLD10, LF_BOOK12, LF_BOOK11, LF_BOOK10, LF_BOOK9, LF_N };
extern const LfFont lf_fonts[LF_N];
#endif
