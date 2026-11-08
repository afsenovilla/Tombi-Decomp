// VERSION SLPS_01144 dspsub 800EB648 544
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800e9ea0 (subScreenDrawTabs.c): tab frames + cursor arrows, no FT4 extras
typedef struct { char p[0x10]; short w10; short mode; char p14[2]; short cursor; char p18[0x30]; int *p48; char p4c[0xC]; int *p58; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w, attr; int img; } Text;
extern Slot D_801094F0[], D_801095A0[], D_80109558[];
extern short D_801095B0[][12];
extern Text D_80111408;
extern void func_800EC9D8(int n);

void func_800EB648(Menu *m)
{
    Slot *s;
    switch (m->mode) {
    case 0:
    case 1:
    case 2:
        s = D_801094F0;
        break;
    case 3:
        s = D_801095A0;
        break;
    case 7:
        s = D_80109558;
        break;
    }
    for (; s->x != -1; s++) {
        short id;
        D_80111408.x = s->x;
        D_80111408.y = s->y;
        D_80111408.w = m->w10 + s->dw;
        D_80111408.attr = 0x7C15;
        id = D_801095B0[m->mode][s->idx];
        if (id != -1) {
            D_80111408.img = m->p58[id];
            func_800EC9D8(1);
            if (s->idx == m->cursor) {
                D_80111408.x = s->x - 10;
                D_80111408.y = s->y - 5;
                D_80111408.w = m->w10;
                D_80111408.attr = 0x79D2;
                D_80111408.img = m->p48[5];
                func_800EC9D8(1);
                if (m->mode == 3) D_80111408.x = s->x + 22;
                else D_80111408.x = s->x + 30;
                D_80111408.y = s->y - 5;
                D_80111408.w = m->w10;
                D_80111408.attr = 0x7A12;
                D_80111408.img = m->p48[6];
                func_800EC9D8(1);
            }
        }
    }
}
#endif
