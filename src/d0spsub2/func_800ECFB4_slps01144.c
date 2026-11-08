// VERSION SLPS_01144 dspsub2 800ECFB4 300
#if defined(VERSION_SLPS_01144)
// JP memCardDrawMessage (PAL d0spsub2 800eb25c): the frame centred on its own rect, no yes/no tile, no footer
typedef struct { short x, y, w, h; } R;
typedef struct { short x0, x1, x2, x3; short a, b, c, d; short m; } L;
extern R D_800F2BE4[];            /* message frames */
extern int D_800F2AC0[];          /* message strings */
/* the draw params x, y, tpage, clut: arrays (in-struct), so the stores keep their order with the frame loads */
extern short D_800F3080[], D_800F3082[], D_800F3084[], D_800F3086[];
extern int D_800F3090;            /* draw string */
extern void *D_800F3094;          /* draw font */
extern char D_800EEE54[];         /* font */
extern unsigned short func_8005AD6C(int x, int y); /* GetClut */
extern void func_800ED0E0(int a); /* memCardDrawString */
extern void func_800498AC(L *l, int c, int tp); /* drawBalloonFrame */

void func_800ECFB4(int n)
{
    L l;
    register R *e asm("$16") = &D_800F2BE4[n];
    short x, y;
    {short w = e->w, xx = e->x, h = e->h, yy = e->y;
    x = xx - (short)(w / 2 - 0xA0);
    y = yy - (short)(h / 2 - 0x70);}
    D_800F3084[0] = 4;
    D_800F3080[0] = x + 8;
    D_800F3082[0] = y + 8;
    D_800F3086[0] = func_8005AD6C(0x150, 0x1F0);
    D_800F3094 = D_800EEE54;
    D_800F3090 = D_800F2AC0[n];
    func_800ED0E0(1);
    l.m = -1;
    l.a = x;
    l.b = y;
    l.c = e->w;
    l.d = e->h;
    func_800498AC(&l, 0, 0x14);
}
#endif
