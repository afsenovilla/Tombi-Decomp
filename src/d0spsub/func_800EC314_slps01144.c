// VERSION SLPS_01144 dspsub 800EC314 644
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800EAC9C (subScreenDrawItemNames.c), from the USA one: 16 items per page
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[8]; short sel; char p20[0x3C]; int *p5c; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; int pC; int str; void *p14; } Text;
extern Slot D_80108E94[];
extern Text D_80111408;
extern unsigned short D_8009EE4C;
extern unsigned char D_8010A38C[];
extern unsigned char D_80109980[];
extern int D_80109A48[];
extern unsigned char D_80109DA4[];
extern unsigned char D_8009EC4C[];
extern char D_8010580C[];
extern void func_800ED0DC(int pri);
extern void func_800EC9D8(int n);

void func_800EC314(Menu *m)
{
    Slot *s;
    for (s = D_80108E94; s->x != -1; s++) {
        int i;
        int n;
        D_80111408.x = s->x + 0x18;
        D_80111408.y = s->y + 5;
        D_80111408.w = m->w10 + 1;
        i = m->page * 16 + s->idx;
        if (i >= D_8009EE4C) continue;
        D_80111408.str = D_80109A48[D_80109980[D_8010A38C[i]]];
        D_80111408.p14 = D_8010580C;
        if (i == m->sel) D_80111408.attr = 0x7C15;
        else D_80111408.attr = (((D_80109DA4[D_8010A38C[i]] & 0x7F) + 0x1F0) << 6) | 0x15;
        func_800ED0DC(5);
        n = D_8009EC4C[D_8010A38C[i]];
        if (D_80109DA4[D_8010A38C[i]] & 0x80) {
            D_80111408.x += 0x58;
            D_80111408.y += 0xA;
            D_80111408.w = m->w10;
            D_80111408.img = m->p5c[10];
            func_800EC9D8(1);
            if (n >= 10) {
                D_80111408.x += 0xA;
                D_80111408.img = m->p5c[n / 10 % 10];
                func_800EC9D8(1);
            }
            D_80111408.x += 0xA;
            D_80111408.img = m->p5c[n % 10];
            func_800EC9D8(1);
        }
    }
}
#endif
