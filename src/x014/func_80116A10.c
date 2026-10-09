// FUNC 80116a10 512 X014
// MATCHING 80116a10 512
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    unsigned char b0, b1, b2, b3;
    int pc;
    char pad[0x164 - 8];
    unsigned char b164, b165, b166;
} G;
extern int LoadImage(RECT *, void *);
extern void func_80116900(short, short);
extern unsigned short D_8009C962;
extern unsigned short D_1F8001F8;
extern unsigned short D_80125978[];
extern unsigned short D_80125B64[];
extern char D_801257B8[], D_80125898[], D_80125984[], D_80125A24[], D_80125AC4[];

void func_80116A10(G *g, char *s)
{
    RECT r;
    int *q = (int *)(s + 4);
    int i;
    short t;

    if (D_8009C962 == 6) {
        if (g->b164 == 0) {
            func_80116900(0, 0);
            func_80116900(1, 0);
            func_80116900(2, 0);
            func_80116900(3, 0);
            g->b165 = 0x3c;
            g->b164++;
        }
        if (g->b165-- <= 0) {
            i = g->b166;
            g->b165 = D_80125978[i];
            g->b166++;
            if (g->b166 >= 6) g->b166 = 0;
            r.x = 0x120;
            r.y = 0x1e0;
            r.w = 0x10;
            r.h = 7;
            if (!(i & 1)) LoadImage(&r, D_801257B8);
            else LoadImage(&r, D_80125898);
        }
    } else if (D_8009C962 == 3 && !(D_1F8001F8 & 7)) {
        i = g->b165;
        t = D_80125B64[i];
        g->b165++;
        if (g->b165 >= 6) g->b165 = 0;
        r.x = 0x130;
        r.y = 0x1f0;
        r.w = 0x10;
        r.h = 5;
        if (t == 0) LoadImage(&r, D_80125984);
        else if (t == 1) LoadImage(&r, D_80125A24);
        else LoadImage(&r, D_80125AC4);
    }
    g->pc = (int)s + *q;
    g->b3 = 1;
}
