// VERSION SLPS_01144 dspsub3 800EB7B0 496
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ea168 (subScreenDrawEventIcons.c), from the USA one
typedef struct { char p[0x10]; short w10; char p12[2]; short page; char p16[0x4e]; unsigned short **icons; } Menu;
typedef struct { short x, y, dw, idx; } Slot;
typedef struct { short x, y, w; } Text;
extern Slot D_800FE0BC[];
extern unsigned char D_800FDFBC[];
extern Text D_800FF57C;
extern short D_800FF582;
extern unsigned short *D_800FF584;
extern unsigned char SCRIPT_FLAGS[];
extern void func_800EC920(int n);

void func_800EB7B0(Menu *m)
{
    Slot *s;
    for (s = D_800FE0BC; s->x != -1; s++) {
        int id;
        short k;
        D_800FF57C.x = s->x;
        D_800FF57C.y = s->y;
        D_800FF57C.w = m->w10 + s->dw + 2;
        id = D_800FDFBC[m->page * 16 + s->idx];
        k = id - 1;
        if (id - 1 >= 0xFF) continue;
        switch (id) {
        case 0x57: D_800FF582 = 0x7D12; break;
        case 0x58: D_800FF582 = 0x7D52; break;
        case 0x59: D_800FF582 = 0x7D92; break;
        case 0x75: D_800FF582 = 0x7DD2; break;
        case 0x98: D_800FF582 = 0x7A93; break;
        case 0x9E: D_800FF582 = 0x7AD3; break;
        case 0xA0: D_800FF582 = 0x7B13; break;
        case 0xA5: D_800FF582 = 0x7B53; break;
        default: D_800FF582 = 0; break;
        }
        if (k < 0) k = 0;
        if (SCRIPT_FLAGS[k] == 0xFF) {
            D_800FF584 = m->icons[k];
            func_800EC920(1);
        }
    }
}
#endif
