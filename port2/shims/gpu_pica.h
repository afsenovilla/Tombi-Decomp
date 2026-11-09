/* The device interface of the "pica" GPU backend (shims/gpu_pica.c): what a fixed-function GPU with the PICA200's
 * limits is asked to do.  shims/gpu_pica.c has the CPU reference (pd_ref); port2/n3ds/pica_c3d.c the 3DS one
 * (citro3d).  Coordinates are VRAM pixels (1024 x 512); colours RGBA8 as one word, red in the low byte. */
#ifndef PORT2_GPU_PICA_H
#define PORT2_GPU_PICA_H

#define PICA_A_OPAQUE 0x80                 /* texel alpha classes: drawn, bit 15 clear */
#define PICA_A_SEMI 0xFF                   /*                      drawn, bit 15 set (semi-transparent when the primitive is) */
#define PICA_SLOTS 32                      /* texture cache slots, 256 x 256 each */
enum { AT_OFF, AT_NOTZERO, AT_OPAQUE, AT_SEMI };   /* alpha test: off, alpha != 0, == PICA_A_OPAQUE, == PICA_A_SEMI */
typedef struct { float x, y, u, v; unsigned char r, g, b, a; } PV;
/* tex: slot or -1; mod: texture x colour x 2 (else the texture, raw); blend: -1 or the PS1 mode 0..3;
   x1..y2: the drawing area (inclusive) */
typedef struct { int tex, mod, atest, blend, x1, y1, x2, y2; } PState;
typedef struct {
    int (*start)(void);                                                    /* 0 = ready */
    void (*tex_upload)(int slot, const unsigned *rgba);                   /* 256 x 256 */
    void (*tris)(const PState *s, const PV *v, int n);                    /* n vertices, 3 per triangle */
    void (*fill)(int x, int y, int w, int h, unsigned rgba);
    void (*read)(int x, int y, int w, int h, unsigned *dst, int stride);
    void (*write)(int x, int y, int w, int h, const unsigned *src, int stride);
    int (*present)(int x, int y, int w, int h, int masked);               /* show the display area; 0 = not handled */
} PicaDev;
#endif
