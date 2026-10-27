// FUNC 80016e34 396 MAIN0
// MATCHING 80016e34 396
typedef struct { short x, y, w, h; } RECT;
typedef struct { RECT disp; RECT screen; unsigned char isinter, isrgb24, pad0, pad1; } DISPENV;
typedef struct {
    RECT clip; short ofs[2]; RECT tw; unsigned short tpage;
    unsigned char dtd, dfe, isbg, r0, g0, b0;
    unsigned long dr_env[16];
} DRAWENV;
typedef struct { DISPENV disp; DRAWENV draw; char rest[0xd10 - 0x14 - 0x5c]; } DB;
extern DB DISPLAY_BUFFERS[2];
extern DRAWENV *SetDefDrawEnv(DRAWENV *, int, int, int, int);
extern DISPENV *SetDefDispEnv(DISPENV *, int, int, int, int);
extern DISPENV *PutDispEnv(DISPENV *);
extern DRAWENV *PutDrawEnv(DRAWENV *);

void initWideDisplayEnv(unsigned char r, unsigned char g, unsigned char b, unsigned char y)
{
    SetDefDrawEnv(&DISPLAY_BUFFERS[0].draw, 0x180, 0, 0x280, 0x200);
    SetDefDispEnv(&DISPLAY_BUFFERS[0].disp, 0x180, 0, 0x280, 0x200);
    SetDefDrawEnv(&DISPLAY_BUFFERS[1].draw, 0x180, 0, 0x280, 0x200);
    SetDefDispEnv(&DISPLAY_BUFFERS[1].disp, 0x180, 0, 0x280, 0x200);
    DISPLAY_BUFFERS[0].disp.screen.x = 0;
    DISPLAY_BUFFERS[0].disp.screen.y = y;
    DISPLAY_BUFFERS[0].disp.screen.w = 0x100;
    DISPLAY_BUFFERS[0].disp.screen.h = 0x100;
    DISPLAY_BUFFERS[1].disp.screen.x = 0;
    DISPLAY_BUFFERS[1].disp.screen.y = y;
    DISPLAY_BUFFERS[1].disp.screen.w = 0x100;
    DISPLAY_BUFFERS[1].disp.screen.h = 0x100;
    DISPLAY_BUFFERS[1].draw.isbg = 1;
    DISPLAY_BUFFERS[0].draw.isbg = 1;
    DISPLAY_BUFFERS[1].draw.dtd = 1;
    DISPLAY_BUFFERS[0].draw.dtd = 1;
    DISPLAY_BUFFERS[1].draw.dfe = 1;
    DISPLAY_BUFFERS[0].draw.dfe = 1;
    DISPLAY_BUFFERS[0].draw.r0 = r;
    DISPLAY_BUFFERS[0].draw.g0 = g;
    DISPLAY_BUFFERS[0].draw.b0 = b;
    DISPLAY_BUFFERS[1].draw.r0 = r;
    DISPLAY_BUFFERS[1].draw.g0 = g;
    DISPLAY_BUFFERS[1].draw.b0 = b;
    PutDispEnv(&DISPLAY_BUFFERS[0].disp);
    PutDrawEnv(&DISPLAY_BUFFERS[0].draw);
}
