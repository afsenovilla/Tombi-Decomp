// VERSION SLPS_01144 dspsub3 800EB590 544
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800e9ea0 (dspsub3 copy; subScreenDrawTabs.c): tab frames + cursor arrows, no FT4 extras
typedef struct { char p[0x10]; short w10; short mode; char p14[2]; short cursor; char p18[0x30]; int *p48; char p4c[0xC]; int *p58; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_800FE7A0[], D_800FE850[], D_800FE808[];
extern short D_800FE860[][12];
extern Text D_800FF57C;
extern void func_800EC920(int n);

void func_800EB590(Menu *m)
{
    Slot *s;
    switch (m->mode) {
    case 0:
    case 1:
    case 2:
        s = D_800FE7A0;
        break;
    case 3:
        s = D_800FE850;
        break;
    case 7:
        s = D_800FE808;
        break;
    }
    for (; s->x != -1; s++) {
        short id;
        D_800FF57C.x = s->x;
        D_800FF57C.y = s->y;
        D_800FF57C.w = m->w10 + s->dw;
        D_800FF57C.attr = 0x7C15;
        id = D_800FE860[m->mode][s->idx];
        if (id != -1) {
            D_800FF57C.img = m->p58[id];
            func_800EC920(1);
            if (s->idx == m->cursor) {
                D_800FF57C.x = s->x - 10;
                D_800FF57C.y = s->y - 5;
                D_800FF57C.w = m->w10;
                D_800FF57C.attr = 0x79D2;
                D_800FF57C.img = m->p48[5];
                func_800EC920(1);
                if (m->mode == 3) D_800FF57C.x = s->x + 22;
                else D_800FF57C.x = s->x + 30;
                D_800FF57C.y = s->y - 5;
                D_800FF57C.w = m->w10;
                D_800FF57C.attr = 0x7A12;
                D_800FF57C.img = m->p48[6];
                func_800EC920(1);
            }
        }
    }
}
#endif
