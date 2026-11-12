/* The device interface of the "pica" GPU backend (shims/gpu_pica.c): what a fixed-function GPU with the PICA200's
 * limits is asked to do.  shims/gpu_pica.c has the CPU reference (pd_ref); port2/n3ds/pica_c3d.c the 3DS one
 * (citro3d).  Coordinates are VRAM pixels (1024 x 512); colours RGBA8 as one word, red in the low byte. */
#ifndef PORT2_GPU_PICA_H
#define PORT2_GPU_PICA_H

#define PICA_A_OPAQUE 0x80                 /* texel alpha classes: drawn, bit 15 clear */
#define PICA_A_SEMI 0xFF                   /*                      drawn, bit 15 set (semi-transparent when the primitive is) */
/* texture cache slots: square blocks of a texture page + CLUT, in three sizes (a sprite's primitives rarely need more
   than a 64 x 64 block of their page, and an area uses many more page + CLUT pairs than whole pages fit in memory),
   in two atlases, so a change of texture seldom ends a batch of triangles:
   atlas 0 (1024 x 1024): N128 slots of 128 x 128 in its upper half, the first 128 of 64 x 64 in its lower half;
   atlas 1 (1024 x 1024): N256 slots of 256 x 256 in its upper half, the other 128 of 64 x 64 in its lower half (the
   first area draws with more than 128 small blocks in a frame in places: each took the slot of one in use, 63
   decodes and uploads a frame on an Old 3DS).  Slots are numbered 256s first, then 128s, then 64s. */
#define PICA_ATLAS 1024                    /* (both atlases are this square) */
#define PICA_N256 8
#define PICA_N128 32
#define PICA_N64 256
/* atlas 2 (1024 x 1024), when the device has the memory for it (pica_n128x: 64 or 0): 64 more slots of 128 x 128,
   numbered after all the others (a scene of the first area asked for more than 32: 385 blocks decoded again in 120
   frames on an Old 3DS) */
#define PICA_N128X 64
#define PICA_X128 (PICA_N256 + PICA_N128 + PICA_N64)   /* the first of them */
#define PICA_SLOTS (PICA_X128 + PICA_N128X)
extern int pica_n128x;
static inline int pica_slot_size(int slot) { return slot < PICA_N256 ? 256 : slot < PICA_N256 + PICA_N128 || slot >= PICA_X128 ? 128 : 64; }
static inline int pica_slot_atlas(int slot) { return slot >= PICA_X128 ? 2 : slot < PICA_N256 || slot >= PICA_N256 + PICA_N128 + 128; }
static inline void pica_slot_pos(int slot, int *x, int *y) {   /* its corner in its atlas */
    if (slot >= PICA_X128) { slot -= PICA_X128; *x = (slot & 7) * 128; *y = (slot >> 3) * 128; return; }
    if (slot < PICA_N256) { *x = (slot & 3) * 256; *y = (slot >> 2) * 256; return; }
    slot -= PICA_N256;
    if (slot < PICA_N128) { *x = (slot & 7) * 128; *y = (slot >> 3) * 128; return; }
    slot -= PICA_N128;
    *x = (slot & 15) * 64; *y = 512 + ((slot >> 4) & 7) * 64;
}
enum { AT_OFF, AT_NOTZERO, AT_OPAQUE, AT_SEMI };   /* alpha test: off, alpha != 0, == PICA_A_OPAQUE, == PICA_A_SEMI */
typedef struct { float x, y, u, v; unsigned char r, g, b, a; } PV;   /* u, v: texels of the atlas */
/* tex: the atlas (0, 1, 2) or -1; mod: texture x colour x 2 (else the texture, raw); blend: -1 or the PS1 mode 0..3;
   x1..y2: the drawing area (inclusive) */
typedef struct { int tex, mod, atest, blend, x1, y1, x2, y2, wide; } PState;   /* wide: the target, 0 VRAM's, 1 the wide plane (--widescreen), 2 the right eye's (--stereo) */
typedef struct {
    int (*start)(void);                                                    /* 0 = ready */
    /* a slot's texels (pica_slot_size(slot) squared); busy: triangles since the last present sample this slot */
    void (*tex_upload)(int slot, const unsigned *rgba, int busy);
    void (*tris)(const PState *s, const PV *v, int n);                    /* n vertices, 3 per triangle */
    void (*fill)(int x, int y, int w, int h, unsigned rgba);
    void (*read)(int x, int y, int w, int h, unsigned *dst, int stride);
    void (*write)(int x, int y, int w, int h, const unsigned *src, int stride);
    int (*present)(int x, int y, int w, int h, int masked);               /* show the display area; 0 = not handled */
    /* optional: memory of the device's for the next batch (*room vertices of it): tris() then gets that pointer and
       has nothing to copy.  Asked before each batch; valid until that batch's tris() */
    PV *(*vbuf)(int *room);
    /* optional: write() from the PS1's own words (VRAM's rows, stride words apart): no pass to RGBA8 before it */
    void (*write15)(int x, int y, int w, int h, const unsigned short *src, int stride);
    /* optional, --widescreen: a second target of 1024 x 512, where the frame is drawn wider (PState.wide), 0 = ready;
       a fill of it, a write from the PS1's words, its rectangle copied into the first target (the 4:3 middle, after
       every ordering table), shown (0 = not handled), read (the reference device: the frame dumps) */
    int (*wide_start)(void);
    void (*wide_fill)(int x, int y, int w, int h, unsigned rgba);
    void (*wide_write)(int x, int y, int w, int h, const unsigned short *src, int stride);
    void (*wide_down)(int wx, int wy, int x, int y, int w, int h);
    int (*wide_present)(int x, int y, int w, int h);
    void (*wide_read)(int x, int y, int w, int h, unsigned *dst, int stride);
    /* optional: what was recorded goes to the GPU now (the end of a drawn ordering table), not at the present */
    void (*submit)(void);
    /* optional, --stereo: the right eye's target, of the first one's size (PState.wide 2), 0 = ready, and a fill of it;
       the present shows its display area to the right eye */
    int (*eye_start)(void);
    void (*eye_fill)(int x, int y, int w, int h, unsigned rgba);
} PicaDev;
#endif
