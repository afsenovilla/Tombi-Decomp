/* GPU backends of the native-pointer port (docs/PORT.md "M3: the OpenGL renderer").
 *
 * shims/gpu.c owns the emulated VRAM (1024x512 16-bit words), the GP0 drawing state, the ordering-table walk and the
 * GP0 packet decoder, the libgpu API and the frame dumps.  Every decoded primitive goes to the selected backend:
 *   - port_gpu_soft (gpu.c): the software rasteriser, writes vram[] directly;
 *   - port_gpu_gl (gpu_gl.c): OpenGL 2.1.  VRAM is one RGBA8 texture that is also the colour attachment of an FBO;
 *     primitives are batched by state and drawn by a fragment shader that samples the 4/8/16-bit texture pages and
 *     the CLUTs from that same texture.  vram[] is the CPU mirror: the backend uploads the rectangles the host writes
 *     (vram_written) and reads back the ones the host wants to read (vram_read) before gpu.c touches them.
 * Every coordinate is a VRAM pixel; the drawing offset is already applied and the drawing area is port_gs. */
#ifndef PORT2_GPU_BACKEND_H
#define PORT2_GPU_BACKEND_H

typedef unsigned char gpu_u8; typedef unsigned short gpu_u16; typedef unsigned int gpu_u32;

typedef struct { int x, y, r, g, b, u, v, sh; } GpuVtx;         /* x, y: VRAM pixels (offset applied); r, g, b 0..255; sh: --stereo, the right eye's x is x + 2 sh */
/* --texture-pack (texpack.c): a replacement image (GL texture) for the texels (ou .. ou + w - 1, ov .. ov + h - 1) of the page */
typedef struct PortTexRep { unsigned gltex; int ou, ov, w, h; } PortTexRep;
typedef struct { int tx, ty, depth, cx, cy; const PortTexRep *rep; } GpuTex;   /* texture page base, 0 = 4-bit / 1 = 8-bit / 2 = 15-bit, CLUT x, y; rep: 0 = none */
/* the port-only CLUTs (prompts.c "port CLUT": the controller button glyphs): a primitive with CLUT id PORT_CLUT_ID - r
   (r < PORT_CLUT_ROWS; bit 15 set: never a GetClut value) reads its 16 colours from row r of port_prompt_clut, not VRAM;
   GpuTex.cy = PORT_CLUT_Y marks it and GpuTex.cx = 16 * r picks the row */
#define PORT_CLUT_ID 0xFFFF
#define PORT_CLUT_ROWS 13
#define PORT_CLUT_IS(id) ((unsigned)(id) > 0xFFFFu - PORT_CLUT_ROWS && (unsigned)(id) <= 0xFFFFu)
#define PORT_CLUT_Y 512
extern gpu_u16 port_prompt_clut[PORT_CLUT_ROWS * 16];   /* rows 9..12 set at start-up (--prompt-colors) */

typedef struct {
    int abr;                        /* semi-transparency mode 0..3 */
    int dither, dfe;
    int twmx, twmy, twox, twoy;     /* texture window mask / offset in 8-pixel units */
    int x1, y1, x2, y2;             /* drawing area, inclusive, inside VRAM */
    int ox, oy;                     /* drawing offset */
    int set_mask, check_mask;
    int wide;                       /* --widescreen: this primitive goes to the wide plane (gpu.c ws_begin), its x/clip in wide columns */
} GpuState;
extern GpuState port_gs;
/* --fps replays (gpu.c hfr.inc): the sub-pixel part of the interpolated positions of the vertices of the primitive being
   drawn (tri: a, b, c; rect: the corner; line: both ends), in pixels; only while port_gpu_subpx_on.  The OpenGL backend
   adds them at internal resolutions above 1x; the software rasteriser works in whole pixels and ignores them. */
extern float port_gpu_subpx[3][2];
extern int port_gpu_subpx_on;            /* the current GP0 E1..E6 state (gpu.c); the backends read it per primitive */

typedef struct {
    const char *name;
    int (*start)(void);             /* 0 = ready; < 0 = unavailable (gpu.c falls back to the software backend) */
    void (*stop)(void);
    /* primitives (the primitive-size limit of the GPU is applied by the decoder) */
    void (*tri)(const GpuVtx *a, const GpuVtx *b, const GpuVtx *c, int shade, const GpuTex *tex, int semi, int raw);
    void (*rect)(int x, int y, int w, int h, gpu_u32 col, const GpuTex *tex, int u0, int v0, int semi, int raw);
    void (*line)(int x0, int y0, gpu_u32 c0, int x1, int y1, gpu_u32 c1, int shade, int semi);
    void (*fill)(int x, int y, int w, int h, gpu_u16 c);                  /* GP0 02 (no clip, no mask) and the DRAWENV isbg clear */
    void (*copy)(int sx, int sy, int dx, int dy, int w, int h);           /* GP0 80: VRAM to VRAM */
    /* coherence with the CPU mirror vram[] (gpu.c) */
    void (*vram_written)(int x, int y, int w, int h);   /* the host changed vram[] in this rectangle */
    void (*vram_read)(int x, int y, int w, int h);      /* the host is about to read vram[] in this rectangle */
    void (*list_end)(void);                             /* end of DrawOTag / DrawPrim: submit the batched work */
    int (*present)(int dx, int dy, int dw, int dh, int masked);   /* show the display area in the window; 0 = not handled */
    int (*present_rgb)(const gpu_u8 *rgb, int w, int h);          /* show an RGB8 frame (the 24-bit display mode); 0 = not handled, may be null */
    /* --widescreen wide plane (2048x512, gpu.c "widescreen"); null on the software backend, which uses gpu.c's wvram[] */
    void (*ws_fill)(int x, int y, int w, int h, gpu_u16 c);                      /* wide-plane rectangle */
    void (*ws_copy_down)(int wx, int wy, int vx, int vy, int w, int h);         /* wide plane -> VRAM */
    void (*ws_upload)(int wx, int wy, int w, int h, const gpu_u16 *src, int stride);
    void (*ws_read)(int wx, int wy, int w, int h, gpu_u16 *dst, int stride);
    int (*ws_present)(int wx, int wy, int w, int h);                            /* show a wide frame; 0 = not handled */
    /* optional (null: two tri calls): the quad v[0..3] as the triangles (0, 1, 2) and (1, 2, 3) in one go; 0 = not
       taken, the decoder then draws the two triangles */
    int (*quad)(const GpuVtx *v, int shade, const GpuTex *tex, int semi, int raw);
} GpuBackend;

extern const GpuBackend port_gpu_soft, port_gpu_gl, port_gpu_pica;
extern const GpuBackend *port_gpu_be;
extern int port_renderer;           /* --renderer: 1 = gl (default), 0 = soft, 2 = pica (gpu_pica.c), -1 = not given (headless picks soft) */
gpu_u16 *port_vram(void);           /* &vram[0][0], 1024 words per row */
#endif
